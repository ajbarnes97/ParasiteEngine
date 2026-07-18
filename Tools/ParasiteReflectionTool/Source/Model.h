#pragma once

#include "optional"
#include "string"
#include "utility"
#include "vector"


struct SReflectedProperty
{
    std::string Name;
    // element type if bIsArray
    std::string Type;
    bool bIsArray = false;
    // -1 if the array size couldn't be resolved to a literal
    int ArrayCount = -1;
};


struct SReflectedFunction
{
    std::string Name;
    // "void" if none found
    std::string ReturnType;
    std::vector<std::string> ParamTypes;
};


struct SReflectedEnum
{
    // e.g. "Parasite::EExampleState"
    std::string QualifiedName;        
    // "enum class"/"enum struct" vs plain "enum"
    bool bIsEnumClass = false;            
    std::string UnderlyingType;
    std::vector<std::pair<std::string, long long>> Values;
};


struct SReflectedClass
{
    // e.g. "Parasite::SEditorSettings"
    std::string QualifiedName;      
    // only single public inheritance is reflected
    std::optional<std::string> Base;
    // line of PE_GENERATED_BODY(); nullopt => not generated
    std::optional<int> GenbodyLine;
	std::vector<SReflectedProperty> Properties;
	std::vector<SReflectedFunction> Functions;
};


struct SParsedFile
{
	std::vector<SReflectedClass> Classes;
	std::vector<SReflectedEnum> Enums;
};