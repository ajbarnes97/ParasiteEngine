#include "CodeGen.h"

#include "cctype"
#include "fstream"
#include "iostream"


std::string SanitizeFileId(const std::filesystem::path& InPath)
{
	std::string Name = InPath.filename().string();
	for (char& Character : Name)
	{
		if (!std::isalnum(static_cast<unsigned char>(Character)) && Character != '_')
		{
			Character = '_';
		}
	}
	return Name;
}

/**
 * The unqualified class name, e.g. "Parasite::SEditorSettings" -> "SEditorSettings".
 * Used for ThisClass/Super and for static_cast<> inside Get/Set/Invoke lambdas,
 * which run *inside* the class's own body (via the pasted PE_GENERATED_BODY
 * macro), where the class's injected-class-name is visible unqualified.
 */
std::string UnqualifiedName(const std::string& InQualifiedName)
{
	const size_t Pos = InQualifiedName.rfind("::");
	return Pos == std::string::npos ? InQualifiedName : InQualifiedName.substr(Pos + 2);
}

/**
 * Splits "A::B::Name" into ({"A","B"}, "Name").
 */
std::pair<std::vector<std::string>, std::string> SplitNamespaces(const std::string& InQualifiedName)
{
	std::vector<std::string> Parts;
	size_t Pos = 0;
	while (true)
	{
		const size_t Separator = InQualifiedName.find("::", Pos);
		if (Separator == std::string::npos)
		{
			Parts.push_back(InQualifiedName.substr(Pos));
			break;
		}
		Parts.push_back(InQualifiedName.substr(Pos, Separator - Pos));
		Pos = Separator + 2;
	}
	const std::string ShortName = Parts.back();
	Parts.pop_back();
	return { Parts, ShortName };
}

std::string PropertyLine(const std::string& InClass, const SReflectedProperty& InProperty)
{
	std::string Get, Set;
	if (InProperty.bIsArray)
	{
		Get = "[](const void* InObject) -> std::any { "
			"const auto& Array = static_cast<const " + InClass + "*>(InObject)->" + InProperty.Name + "; "
			"return std::vector<" + InProperty.Type + ">(std::begin(Array), std::end(Array)); }";
		Set = "[](void* InObject, const std::any& InValue) { "
			"auto& Array = static_cast<" + InClass + "*>(InObject)->" + InProperty.Name + "; "
			"const auto& Vector = std::any_cast<const std::vector<" + InProperty.Type + ">&>(InValue); "
			"const size_t Count = std::min(Vector.size(), std::size(Array)); "
			"std::copy_n(Vector.begin(), Count, std::begin(Array)); }";
	}
	else
	{
		Get = "[](const void* InObject) -> std::any { return static_cast<const " + InClass + "*>(InObject)->" + InProperty.Name + "; }";
		Set = "[](void* InObject, const std::any& InValue) { static_cast<" + InClass + "*>(InObject)->" + InProperty.Name +
			" = std::any_cast<" + InProperty.Type + ">(InValue); }";
	}

	return "InInfo.Properties.push_back(SPropertyInfo{"
		"\"" + InProperty.Name + "\", "
		"std::type_index(typeid(" + InProperty.Type + ")), " +
		(InProperty.bIsArray ? std::string("true, ") + std::to_string(InProperty.ArrayCount) : "false, -1") + ", " +
		Get + ", " + Set + "});";
}

std::string FunctionLines(const std::string& InClass, const SReflectedFunction& InFunction)
{
	std::string ParamTypeIndices;
	for (size_t Index = 0; Index < InFunction.ParamTypes.size(); ++Index)
	{
		if (Index != 0)
		{
			ParamTypeIndices += ", ";
		}
		ParamTypeIndices += "std::type_index(typeid(" + InFunction.ParamTypes[Index] + "))";
	}

	std::string Args;
	for (size_t Index = 0; Index < InFunction.ParamTypes.size(); ++Index)
	{
		if (Index != 0)
		{
			Args += ", ";
		}
		Args += "std::any_cast<" + InFunction.ParamTypes[Index] + ">(InArgs[" + std::to_string(Index) + "])";
	}

	const std::string CountCheck =
		"if (InArgs.size() != " + std::to_string(InFunction.ParamTypes.size()) +
		") { throw std::runtime_error(\"" + InFunction.Name + ": expected " + std::to_string(InFunction.ParamTypes.size()) +
		" argument(s)\"); }";

	std::string Call;
	if (InFunction.ReturnType == "void")
	{
		Call = "Self->" + InFunction.Name + "(" + Args + "); return std::any{};";
	}
	else
	{
		Call = "return std::any(Self->" + InFunction.Name + "(" + Args + "));";
	}

	return "InInfo.Functions.push_back(SFunctionInfo{"
		"\"" + InFunction.Name + "\", "
		"std::type_index(typeid(" + InFunction.ReturnType + ")), "
		"{" + ParamTypeIndices + "}, "
		"[](void* InObject, const std::vector<std::any>& InArgs) -> std::any { "
		"auto* Self = static_cast<" + InClass + "*>(InObject); " + CountCheck + " " + Call + " }"
		"});";
}


std::optional<std::filesystem::path> GenerateOutput(const std::filesystem::path& InSourcePath, const SParsedFile& InParsed, const std::filesystem::path& InOutputDir)
{
    if (InParsed.Classes.empty() && InParsed.Enums.empty())
    {
        return std::nullopt;
    }

    const std::string FileId = SanitizeFileId(InSourcePath);

    std::vector<std::string> Out;
    Out.push_back("// AUTO-GENERATED Parasite Engine reflected file -- DO NOT EDIT");
    Out.push_back("#pragma once");
    Out.push_back("#define CURRENT_FILE_ID " + FileId);
    Out.push_back("");

    bool bWroteAnything = false;

    for (const SReflectedClass& Class : InParsed.Classes)
    {
        if (!Class.GenbodyLine.has_value())
        {
            continue;
        }

        bWroteAnything = true;
        const std::string ShortName = UnqualifiedName(Class.QualifiedName);
        const std::string MacroName = FileId + "_" + std::to_string(*Class.GenbodyLine) + "_GENERATED_BODY";

        std::vector<std::string> Body;
        Body.push_back("public:");
        Body.push_back("using ThisClass = " + ShortName + ";");

        if (Class.Base.has_value())
        {
            Body.push_back("using Super = " + *Class.Base + ";");
        }

        Body.push_back("static const SClassInfo* StaticClass() {");
        Body.push_back("static const SClassInfo Info = BuildClassInfo();");
        Body.push_back("return &Info;");
        Body.push_back("}");
        Body.push_back(Class.Base.has_value() ? "const SClassInfo* GetClass() const override {" : "virtual const SClassInfo* GetClass() const {");
        Body.push_back("return StaticClass();");
        Body.push_back("}");
        Body.push_back("private:");
        Body.push_back("static SClassInfo BuildClassInfo() {");
        Body.push_back("SClassInfo Info;");
        Body.push_back("Info.Name = \"" + Class.QualifiedName + "\";");
        Body.push_back(Class.Base.has_value() ? ("Info.Super = " + *Class.Base + "::StaticClass();") : "Info.Super = nullptr;");
        Body.push_back("RegisterMembers(Info);");
        Body.push_back("return Info;");
        Body.push_back("}");
        Body.push_back("static void RegisterMembers(SClassInfo& InInfo) {");

        for (const SReflectedProperty& Property : Class.Properties)
        {
            Body.push_back(PropertyLine(ShortName, Property));
        }
        for (const SReflectedFunction& Function : Class.Functions)
        {
            Body.push_back(FunctionLines(ShortName, Function));
        }

        Body.push_back("}");

        Out.push_back("#define " + MacroName + " \\");
        for (size_t Index = 0; Index < Body.size(); ++Index)
        {
            const std::string Suffix = (Index != Body.size() - 1) ? " \\" : "";
            Out.push_back("    " + Body[Index] + Suffix);
        }
        Out.push_back("");
    }

    for (const SReflectedEnum& Enum : InParsed.Enums)
    {
        // A plain (non-"enum class") enum with no explicit underlying type
        // can't legally be forward-declared in C++, and the PE_GENERATED_BODY
        // convention requires this header to be included *before* the real
        // enum definition later in the source file -- so without a forward
        // declaration we have no way to name the type yet. Skip it rather
        // than emit code that won't compile; note the fix (add "enum class"
        // or an explicit ": UnderlyingType") so it's discoverable.
        if (!Enum.bIsEnumClass && Enum.UnderlyingType.empty())
        {
            std::cerr << "warning: " << Enum.QualifiedName
                << " needs 'enum class' or an explicit underlying type to be reflected -- skipped\n";
            continue;
        }

        bWroteAnything = true;

        const auto [Namespaces, ShortName] = SplitNamespaces(Enum.QualifiedName);
        std::string ForwardDecl;
        for (const std::string& Namespace : Namespaces)
        {
            ForwardDecl += "namespace " + Namespace + " { ";
        }
        ForwardDecl += std::string(Enum.bIsEnumClass ? "enum class " : "enum ") + ShortName;
        if (!Enum.UnderlyingType.empty())
        {
            ForwardDecl += " : " + Enum.UnderlyingType;
        }
        ForwardDecl += ";";
        for (size_t Index = 0; Index < Namespaces.size(); ++Index)
        {
            ForwardDecl += " }";
        }
        Out.push_back(ForwardDecl);

        std::string ValuesInit;
        for (size_t Index = 0; Index < Enum.Values.size(); ++Index)
        {
            if (Index != 0)
            {
                ValuesInit += ", ";
            }
            ValuesInit += "{\"" + Enum.Values[Index].first + "\", " + std::to_string(Enum.Values[Index].second) + "}";
        }

        Out.push_back("template<> inline const SEnumInfo* StaticEnum<" + Enum.QualifiedName + ">() {");
        Out.push_back("    static const SEnumInfo Info = []{");
        Out.push_back("        SEnumInfo Info;");
        Out.push_back("        Info.Name = \"" + Enum.QualifiedName + "\";");
        Out.push_back("        Info.UnderlyingType = \"" + Enum.UnderlyingType + "\";");
        Out.push_back("        Info.Values = { " + ValuesInit + " };");
        Out.push_back("        return Info;");
        Out.push_back("    }();");
        Out.push_back("    return &Info;");
        Out.push_back("}");
        Out.push_back("");
    }

    if (!bWroteAnything)
    {
        return std::nullopt;
    }

    std::error_code ErrorCode;
    std::filesystem::create_directories(InOutputDir, ErrorCode);

    const std::filesystem::path OutputFile = InOutputDir / (InSourcePath.stem().string() + ".generated.h");
    std::ofstream Ofs(OutputFile, std::ios::binary);
    for (size_t Index = 0; Index < Out.size(); ++Index)
    {
        Ofs << Out[Index];
        if (Index != Out.size() - 1)
        {
            Ofs << "\n";
        }
    }

    return OutputFile;
}