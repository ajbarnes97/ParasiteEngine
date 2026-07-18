#pragma once

#include "algorithm"
#include "any"
#include "functional"
#include "stdexcept"
#include "string"
#include "typeindex"
#include "utility"
#include "vector"


struct SPropertyInfo
{
	std::string Name;
	std::type_index Type;
	bool bIsArray = false;
	int ArrayCount = -1; // -1 if unknown/dynamic; otherwise the fixed C-array size
	std::function<std::any(const void*)> Get;
	std::function<void(void*, const std::any&)> Set;
};

struct SFunctionInfo
{
	std::string Name;
	std::type_index ReturnType;
	std::vector<std::type_index> ParamTypes;
	// Throws std::runtime_error on argument-count mismatch; std::bad_any_cast
	// on argument-type mismatch. Void-returning functions yield an empty std::any.
	std::function<std::any(void*, const std::vector<std::any>&)> Invoke;
};

struct SClassInfo
{
	std::string Name;
	const SClassInfo* Super = nullptr;
	std::vector<SPropertyInfo> Properties;
	std::vector<SFunctionInfo> Functions;

	const SPropertyInfo* FindProperty(const std::string& InPropertyName) const
	{
		for (const auto& Property : Properties)
		{
			if (Property.Name == InPropertyName)
			{
				return &Property;
			}
		}
		return Super ? Super->FindProperty(InPropertyName) : nullptr;
	}

	const SFunctionInfo* FindFunction(const std::string& InFunctionName) const
	{
		for (const auto& Funciton : Functions)
		{
			if (Funciton.Name == InFunctionName)
			{
				return &Funciton;
			}
		}
		return Super ? Super->FindFunction(InFunctionName) : nullptr;
	}

	void ForEachProperty(const std::function<void(const SPropertyInfo&)>& InFunc) const
	{
		if (Super)
		{
			Super->ForEachProperty(InFunc);
		}
		for (const auto& Property : Properties)
		{
			InFunc(Property);
		}
	}

	void ForEachFunction(const std::function<void(const SFunctionInfo&)>& InFunc) const
	{
		if (Super)
		{
			Super->ForEachFunction(InFunc);
		}
		for (const auto& Function : Functions)
		{
			InFunc(Function);
		}
	}

	bool IsA(const SClassInfo* InOther) const
	{
		for (const SClassInfo* Class = this; Class; Class = Class->Super)
		{
			if (Class == InOther)
			{
				return true;
			}
		}
		return false;
	}
};

class CObject
{
public:
	virtual ~CObject() = default;
	virtual const SClassInfo* GetClass() const = 0;
};


/**
 * UENUM() marked enums get an explicit specialization of
 * StaticEnum<T>() emitted into their file's .generated.h -- there's no
 * per-enum body to inject into (unlike classes with PE_GENERATED_BODY()), so
 * the specialization is emitted directly rather than behind a pasted macro.
 */
struct SEnumInfo
{
	std::string Name;
	std::string UnderlyingType;
	std::vector<std::pair<std::string, long long>> Values;

	const std::pair<std::string, long long>* FindByName(const std::string& InEnumeratorName) const
	{
		for (const auto& Value : Values)
		{
			if (Value.first == InEnumeratorName)
			{
				return &Value;
			}
		}
		return nullptr;
	}

	const std::pair<std::string, long long>* FindByValue(long long InValue) const
	{
		for (const auto& Value : Values)
		{
			if (Value.second == InValue)
			{
				return &Value;
			}
		}
		return nullptr;
	}
};


/**
 * Primary template intentionally left undefined, only PE_ENUM()-reflected
 * enum types get a generated explicit specialization. Instantiating this for
 * an unreflected type is a compile error, same as Unreal's own StaticEnum<T>().
 */
template<typename T>
const SEnumInfo* StaticEnum();


/**
 * PE_CLASS() / PE_ENUM() / PE_PROPERTY() / PE_FUNCTION() are pure markers. The
 * compiler sees them as nothing at all. Only the offline generator
 * reads the source text and cares that they're there.
 */
#define PE_CLASS(...)
#define PE_ENUM(...)
#define PE_PROPERTY(...)
#define PE_FUNCTION(...)


/**
 * PE_GENERATED_BODY() resolves, via token pasting, to a per-line, per-file
 * unique identifier: <FILE_ID>_<LINE>_GENERATED_BODY. The generated header
 * (included above this macro's use) #defines that exact identifier with the
 * real member declarations for that class.
 */
#define UHT_PASTE_INNER(a, b) a##b
#define UHT_PASTE(a, b) UHT_PASTE_INNER(a, b)
#define PE_GENERATED_BODY() \
    UHT_PASTE(UHT_PASTE(CURRENT_FILE_ID, UHT_PASTE(_, __LINE__)), _GENERATED_BODY)
