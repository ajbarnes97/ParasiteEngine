#include "Parser.h"

#include "cstdlib"
#include "stdexcept"


const EToken& At(const std::vector<EToken>& InTokens, size_t InIndex)
{
	// InTokens.back() is always EndOfFile
	return InIndex < InTokens.size() ? InTokens[InIndex] : InTokens.back();
}

bool IsPunct(const EToken & InToken, char InCharacter)
{
	return InToken.Kind == ETokenKind::Punct && InToken.Text.size() == 1 && InToken.Text[0] == InCharacter;
}

bool IsIdent(const EToken & InToken, const char* InText)
{
	return InToken.Kind == ETokenKind::Identifier && InToken.Text == InText;
}

/**
 * Scans forward from an opening bracket at index `InOpen` (whose text is
 * `InOpenCh`) to its matching close (`InCloseCh`), returning the index of the
 * matching close token. Only counts that bracket type -- safe because
 * comments/strings/chars are already gone by the time the parser sees tokens.
 */
size_t FindMatchingBracket(const std::vector<EToken>&InTokens, size_t InOpen, char InOpenCh, char InCloseCh)
{
	int Depth = 0;
	for (size_t Index = InOpen; Index < InTokens.size(); ++Index)
	{
		if (IsPunct(InTokens[Index], InOpenCh))
		{
			++Depth;
		}
		else if (IsPunct(InTokens[Index], InCloseCh))
		{
			--Depth;
			if (Depth == 0)
			{
				return Index;
			}
		}
	}
	return InTokens.size() - 1; // unmatched -- stop at EOF rather than looping forever
}

/**
 * Consumes an Identifier ("::" Identifier)* sequence starting at `InIndex`.
 * Returns the joined qualified name and advances `InIndex` past it.
 */
std::string ParseQualifiedName(const std::vector<EToken>&InTokens, size_t & InIndex)
{
	std::string Name;
	if (At(InTokens, InIndex).Kind == ETokenKind::Identifier)
	{
		Name = InTokens[InIndex].Text;
		++InIndex;
	}
	while (At(InTokens, InIndex).Kind == ETokenKind::ScopeResolution && At(InTokens, InIndex + 1).Kind == ETokenKind::Identifier)
	{
		Name += "::";
		Name += InTokens[InIndex + 1].Text;
		InIndex += 2;
	}
	return Name;
}

/**
 * Joins a run of type tokens into readable (and still valid, since C++
 * doesn't care about whitespace here) type text, e.g.
 * ["std","::","vector","<","std","::","string",">"] -> "std::vector<std::string>"
 */
std::string JoinTypeTokens(const std::vector<EToken>&InTokens, size_t InBegin, size_t InEnd)
{
	std::string Out;
	for (size_t Index = InBegin; Index < InEnd; ++Index)
	{
		const EToken& Token = InTokens[Index];
		const bool bIsScope = (Token.Kind == ETokenKind::ScopeResolution);
		const bool bTight = bIsScope || Token.Text == "," || Token.Text == "<" || Token.Text == ">" ||
			Token.Text == "*" || Token.Text == "&" || Token.Text == "::";
		const bool bPrevTight = !Out.empty() &&
			(InTokens[Index - 1].Kind == ETokenKind::ScopeResolution ||
				InTokens[Index - 1].Text == "<" || InTokens[Index - 1].Text == "*" || InTokens[Index - 1].Text == "&");
		if (!Out.empty() && !bTight && !bPrevTight)
		{
			Out += ' ';
		}
		Out += Token.Text;
	}
	return Out;
}

/**
 * Skips one opaque top-level declaration/statement/block starting at `InIndex`:
 * either "...;'  or  "...{ ... } [;]". Tracks (), [], <> nesting so commas
 * and braces inside parameter lists / template args / array sizes don't
 * terminate early. Returns the index just past what was skipped.
 */
size_t SkipStatementOrBlock(const std::vector<EToken>&InTokens, size_t InIndex)
{
	int ParenDepth = 0;
	int BracketDepth = 0;
	int AngleDepth = 0;

	for (; InIndex < InTokens.size(); ++InIndex)
	{
		const EToken& Token = InTokens[InIndex];
		if (IsPunct(Token, '(')) { ++ParenDepth; continue; }
		if (IsPunct(Token, ')')) { --ParenDepth; continue; }
		if (IsPunct(Token, '[')) { ++BracketDepth; continue; }
		if (IsPunct(Token, ']')) { --BracketDepth; continue; }
		if (IsPunct(Token, '<') && ParenDepth == 0 && BracketDepth == 0) { ++AngleDepth; continue; }
		if (IsPunct(Token, '>') && AngleDepth > 0) { --AngleDepth; continue; }

		if (ParenDepth <= 0 && BracketDepth <= 0)
		{
			if (IsPunct(Token, ';'))
			{
				return InIndex + 1;
			}
			if (IsPunct(Token, '{'))
			{
				const size_t Close = FindMatchingBracket(InTokens, InIndex, '{', '}');
				size_t Next = Close + 1;
				if (IsPunct(At(InTokens, Next), ';'))
				{
					++Next;
				}
				return Next;
			}
		}
		if (Token.Kind == ETokenKind::EndOfFile)
		{
			return InIndex;
		}
	}
	return InTokens.size() - 1;
}

/**
 * Parses a single PE_PROPERTY()-marked member declaration starting at `InIndex`
 * (the first token of the type). Returns the index just past the ';'.
 */
size_t ParsePropertyDecl(const std::vector<EToken>&InTokens, size_t InIndex, SReflectedProperty & OutProperty)
{
	const size_t Start = InIndex;
	int ParenDepth = 0;
	int AngleDepth = 0;
	size_t NameIndex = std::string::npos;

	for (; InIndex < InTokens.size(); ++InIndex)
	{
		const EToken& Token = InTokens[InIndex];
		if (IsPunct(Token, '(')) { ++ParenDepth; continue; }
		if (IsPunct(Token, ')')) { --ParenDepth; continue; }
		if (IsPunct(Token, '<') && ParenDepth == 0) { ++AngleDepth; continue; }
		if (IsPunct(Token, '>') && AngleDepth > 0) { --AngleDepth; continue; }

		if (ParenDepth == 0 && AngleDepth == 0)
		{
			if (IsPunct(Token, '[') || IsPunct(Token, '=') || IsPunct(Token, ';'))
			{
				break;
			}
			if (Token.Kind == ETokenKind::Identifier)
			{
				NameIndex = InIndex; // keep overwriting -- we want the LAST identifier before the break
			}
		}
	}

	if (NameIndex == std::string::npos)
	{
		// Malformed / unrecognized declaration -- bail out safely.
		OutProperty = {};
		return SkipStatementOrBlock(InTokens, Start);
	}

	OutProperty.Name = InTokens[NameIndex].Text;
	OutProperty.Type = JoinTypeTokens(InTokens, Start, NameIndex);
	OutProperty.bIsArray = false;
	OutProperty.ArrayCount = -1;

	// Optional array suffix: Name[N]
	if (IsPunct(At(InTokens, InIndex), '['))
	{
		const size_t Close = FindMatchingBracket(InTokens, InIndex, '[', ']');
		OutProperty.bIsArray = true;
		if (Close > InIndex + 1 && At(InTokens, InIndex + 1).Kind == ETokenKind::Number)
		{
			OutProperty.ArrayCount = std::atoi(InTokens[InIndex + 1].Text.c_str());
		}
		InIndex = Close + 1;
	}

	// Optional default value: skip everything up to the top-level ';'
	if (IsPunct(At(InTokens, InIndex), '='))
	{
		InIndex = SkipStatementOrBlock(InTokens, InIndex);
		return InIndex;
	}

	if (IsPunct(At(InTokens, InIndex), ';'))
	{
		++InIndex;
	}
	return InIndex;
}

/**
 * Parses a single PE_FUNCTION()-marked member declaration starting at `InIndex`.
 * Sets `OutReflected` to false (rather than returning std::nullopt) if this
 * looks like a constructor/destructor/operator overload or a static method,
 * none of which are reflected in this version.
 */
size_t ParseFunctionDecl(const std::vector<EToken>&InTokens, size_t InIndex, SReflectedFunction & OutFunction, bool& OutReflected)
{
	const size_t Start = InIndex;
	OutReflected = false;

	// Skip leading qualifiers. "static" methods aren't reflected (no `this`
	// to bind against with our SPropertyInfo-style Get/Set convention).
	bool bIsStatic = false;
	while (IsIdent(At(InTokens, InIndex), "virtual") || IsIdent(At(InTokens, InIndex), "static") ||
		IsIdent(At(InTokens, InIndex), "inline") || IsIdent(At(InTokens, InIndex), "explicit") ||
		IsIdent(At(InTokens, InIndex), "friend") || IsIdent(At(InTokens, InIndex), "constexpr"))
	{
		if (IsIdent(InTokens[InIndex], "static"))
		{
			bIsStatic = true;
		}
		++InIndex;
	}

	if (IsIdent(At(InTokens, InIndex), "operator") || IsPunct(At(InTokens, InIndex), '~'))
	{
		// Operator overloads / destructors: not reflected. Skip past.
		return SkipStatementOrBlock(InTokens, Start);
	}

	// Gather return-type tokens up to an Identifier immediately followed by '('.
	size_t NameIndex = std::string::npos;
	int AngleDepth = 0;
	size_t Index = InIndex;
	for (; Index < InTokens.size(); ++Index)
	{
		const EToken& Token = InTokens[Index];
		if (IsPunct(Token, '<')) { ++AngleDepth; continue; }
		if (IsPunct(Token, '>') && AngleDepth > 0) { --AngleDepth; continue; }
		if (AngleDepth == 0 && Token.Kind == ETokenKind::Identifier && IsPunct(At(InTokens, Index + 1), '('))
		{
			NameIndex = Index;
			break;
		}
		if (AngleDepth == 0 && (IsPunct(Token, ';') || IsPunct(Token, '{')))
		{
			break; // no recognizable "name(" pattern found -- give up
		}
	}

	if (NameIndex == std::string::npos)
	{
		return SkipStatementOrBlock(InTokens, Start);
	}

	const std::string ReturnType = (NameIndex > InIndex) ? JoinTypeTokens(InTokens, InIndex, NameIndex) : std::string("void");
	const std::string Name = InTokens[NameIndex].Text;

	const size_t OpenParen = NameIndex + 1;
	const size_t CloseParen = FindMatchingBracket(InTokens, OpenParen, '(', ')');

	// Split parameters at top-level commas (respecting nested (), [], <>).
	std::vector<std::string> ParamTypes;
	{
		size_t ChunkStart = OpenParen + 1;
		int ParenDepth = 0, BracketDepth = 0, AngleDepth2 = 0;
		for (size_t Cursor = OpenParen + 1; Cursor <= CloseParen; ++Cursor)
		{
			const EToken& Token = At(InTokens, Cursor);
			const bool bAtTop = (ParenDepth == 0 && BracketDepth == 0 && AngleDepth2 == 0);
			if (IsPunct(Token, '(')) { ++ParenDepth; }
			else if (IsPunct(Token, ')') && Cursor != CloseParen) { --ParenDepth; }
			else if (IsPunct(Token, '[')) { ++BracketDepth; }
			else if (IsPunct(Token, ']')) { --BracketDepth; }
			else if (IsPunct(Token, '<')) { ++AngleDepth2; }
			else if (IsPunct(Token, '>') && AngleDepth2 > 0) { --AngleDepth2; }

			if ((bAtTop && IsPunct(Token, ',')) || Cursor == CloseParen)
			{
				size_t ChunkEnd = Cursor;
				if (ChunkEnd > ChunkStart)
				{
					// Drop a default value ("= expr") if present at top level.
					size_t EqualsIndex = std::string::npos;
					int ChunkParenDepth = 0, ChunkBracketDepth = 0, ChunkAngleDepth = 0;
					for (size_t Cursor2 = ChunkStart; Cursor2 < ChunkEnd; ++Cursor2)
					{
						const EToken& ChunkToken = InTokens[Cursor2];
						if (IsPunct(ChunkToken, '(')) ++ChunkParenDepth;
						else if (IsPunct(ChunkToken, ')')) --ChunkParenDepth;
						else if (IsPunct(ChunkToken, '[')) ++ChunkBracketDepth;
						else if (IsPunct(ChunkToken, ']')) --ChunkBracketDepth;
						else if (IsPunct(ChunkToken, '<')) ++ChunkAngleDepth;
						else if (IsPunct(ChunkToken, '>') && ChunkAngleDepth > 0) --ChunkAngleDepth;
						else if (ChunkParenDepth == 0 && ChunkBracketDepth == 0 && ChunkAngleDepth == 0 &&
							IsPunct(ChunkToken, '=') && EqualsIndex == std::string::npos)
						{
							EqualsIndex = Cursor2;
						}
					}
					const size_t TypeEnd = (EqualsIndex != std::string::npos) ? EqualsIndex : ChunkEnd;
					// Drop a trailing parameter-name identifier, if present.
					size_t NameEnd = TypeEnd;
					if (NameEnd > ChunkStart && InTokens[NameEnd - 1].Kind == ETokenKind::Identifier)
					{
						// Only drop it if there's at least one token left for the type.
						if (NameEnd - 1 > ChunkStart)
						{
							NameEnd -= 1;
						}
					}
					const std::string ParamType = JoinTypeTokens(InTokens, ChunkStart, NameEnd);
					if (!ParamType.empty() && ParamType != "void")
					{
						ParamTypes.push_back(ParamType);
					}
				}
				ChunkStart = Cursor + 1;
			}
		}
	}

	// Consume trailing qualifiers: const / override / final / noexcept(...)
	size_t Cursor = CloseParen + 1;
	while (IsIdent(At(InTokens, Cursor), "const") || IsIdent(At(InTokens, Cursor), "override") ||
		IsIdent(At(InTokens, Cursor), "final") || IsIdent(At(InTokens, Cursor), "noexcept"))
	{
		const bool bIsNoexcept = IsIdent(InTokens[Cursor], "noexcept");
		++Cursor;
		if (bIsNoexcept && IsPunct(At(InTokens, Cursor), '('))
		{
			Cursor = FindMatchingBracket(InTokens, Cursor, '(', ')') + 1;
		}
	}

	size_t End;
	if (IsPunct(At(InTokens, Cursor), '=') && At(InTokens, Cursor + 1).Kind == ETokenKind::Number && At(InTokens, Cursor + 1).Text == "0")
	{
		End = Cursor + 2;
		if (IsPunct(At(InTokens, End), ';'))
		{
			++End;
		}
	}
	else if (IsPunct(At(InTokens, Cursor), ';'))
	{
		End = Cursor + 1;
	}
	else if (IsPunct(At(InTokens, Cursor), '{'))
	{
		End = FindMatchingBracket(InTokens, Cursor, '{', '}') + 1;
	}
	else
	{
		End = SkipStatementOrBlock(InTokens, Cursor);
	}

	if (bIsStatic)
	{
		return End; // parsed and skipped correctly, just not reflected
	}

	OutFunction.Name = Name;
	OutFunction.ReturnType = ReturnType;
	OutFunction.ParamTypes = std::move(ParamTypes);
	OutReflected = true;
	return End;
}

/**
 * Parses enumerators between (InStart, InEnd) exclusive of the closing '}'.
 */
void ParseEnumerators(const std::vector<EToken>&InTokens, size_t InStart, size_t InEnd, std::vector<std::pair<std::string, long long>>&OutValues)
{
	long long AutoValue = 0;
	size_t ChunkStart = InStart;
	int Depth = 0;

	for (size_t Index = InStart; Index <= InEnd; ++Index)
	{
		const EToken& Token = At(InTokens, Index);
		if (IsPunct(Token, '(') || IsPunct(Token, '[')) { ++Depth; }
		else if (IsPunct(Token, ')') || IsPunct(Token, ']')) { --Depth; }

		const bool bIsSeparator = (Depth == 0 && (IsPunct(Token, ',') || Index == InEnd));
		if (bIsSeparator)
		{
			if (Index > ChunkStart)
			{
				size_t Cursor = ChunkStart;
				if (At(InTokens, Cursor).Kind == ETokenKind::Identifier)
				{
					const std::string EnumeratorName = InTokens[Cursor].Text;
					++Cursor;
					long long Value = AutoValue;
					if (Cursor < Index && IsPunct(InTokens[Cursor], '='))
					{
						++Cursor;
						// Only a single integer literal is understood; anything
						// fancier (bit shifts, other enumerators, etc.) falls
						// back to auto-increment, same as this tool's other
						// "best effort" simplifications.
						if (Cursor < Index && InTokens[Cursor].Kind == ETokenKind::Number)
						{
							const std::string& NumberText = InTokens[Cursor].Text;
							Value = std::strtoll(NumberText.c_str(), nullptr, 0);
						}
					}
					OutValues.push_back({ EnumeratorName, Value });
					AutoValue = Value + 1;
				}
			}
			ChunkStart = Index + 1;
		}
	}
}


SParsedFile Parse(const std::vector<EToken>& InTokens)
{
    SParsedFile Result;

    enum class EScopeKind { Namespace, Class };
    struct FScope
    {
        EScopeKind Kind;
        std::optional<SReflectedClass> ClassData; // set only for reflected classes
    };

    std::vector<FScope> Scopes;
    std::vector<std::string> NamespaceStack;

    enum class EMarker { None, Class, Enum, Property, Function };
    EMarker Pending = EMarker::None;

    size_t Index = 0;
    while (Index < InTokens.size())
    {
        const EToken& Token = At(InTokens, Index);

        if (Token.Kind == ETokenKind::EndOfFile)
        {
            break;
        }

        // namespace X { ... }  or  namespace { ... }
        if (IsIdent(Token, "namespace"))
        {
            size_t Cursor = Index + 1;
            std::string Name;
            if (At(InTokens, Cursor).Kind == ETokenKind::Identifier)
            {
                Name = ParseQualifiedName(InTokens, Cursor);
            }
            if (IsPunct(At(InTokens, Cursor), '{'))
            {
                NamespaceStack.push_back(Name);
                Scopes.push_back({ EScopeKind::Namespace, std::nullopt });
                Index = Cursor + 1;
                continue;
            }
            // Not a namespace-with-body (e.g. an alias like "namespace A = B;") -- skip.
            Index = SkipStatementOrBlock(InTokens, Index);
            continue;
        }

        // PE_CLASS() / PE_ENUM() / PE_PROPERTY() / PE_FUNCTION() markers
        if ((IsIdent(Token, "PE_CLASS") || IsIdent(Token, "PE_ENUM") || IsIdent(Token, "PE_PROPERTY") || IsIdent(Token, "PE_FUNCTION")) &&
            IsPunct(At(InTokens, Index + 1), '('))
        {
            const size_t Close = FindMatchingBracket(InTokens, Index + 1, '(', ')');
            Pending = IsIdent(Token, "PE_CLASS") ? EMarker::Class
                : IsIdent(Token, "PE_ENUM") ? EMarker::Enum
                : IsIdent(Token, "PE_PROPERTY") ? EMarker::Property
                : EMarker::Function;
            Index = Close + 1;
            if (IsPunct(At(InTokens, Index), ';'))
            {
                ++Index;
            }
            continue;
        }

        // PE_GENERATED_BODY()
        if (IsIdent(Token, "PE_GENERATED_BODY") && IsPunct(At(InTokens, Index + 1), '(') && IsPunct(At(InTokens, Index + 2), ')'))
        {
            if (!Scopes.empty() && Scopes.back().Kind == EScopeKind::Class && Scopes.back().ClassData.has_value())
            {
                Scopes.back().ClassData->GenbodyLine = Token.Line;
            }
            Index += 3;
            if (IsPunct(At(InTokens, Index), ';'))
            {
                ++Index;
            }
            continue;
        }

        // class / struct
        if (IsIdent(Token, "class") || IsIdent(Token, "struct"))
        {
            // Forward declaration: "class Name ;"
            if (At(InTokens, Index + 1).Kind == ETokenKind::Identifier && IsPunct(At(InTokens, Index + 2), ';'))
            {
                Pending = EMarker::None; // a stray PE_CLASS() before a fwd-decl shouldn't leak forward
                Index += 3;
                continue;
            }

            if (At(InTokens, Index + 1).Kind == ETokenKind::Identifier)
            {
                size_t Cursor = Index + 1;
                const std::string Name = ParseQualifiedName(InTokens, Cursor);
                std::optional<std::string> Base;

                if (IsPunct(At(InTokens, Cursor), ':'))
                {
                    ++Cursor;
                    bool bSawPublic = false;
                    // First inheritance specifier only; additional
                    // comma-separated bases (multiple inheritance) are
                    // skipped over but not reflected.
                    if (IsIdent(At(InTokens, Cursor), "public") || IsIdent(At(InTokens, Cursor), "private") || IsIdent(At(InTokens, Cursor), "protected"))
                    {
                        bSawPublic = IsIdent(InTokens[Cursor], "public");
                        ++Cursor;
                    }
                    if (At(InTokens, Cursor).Kind == ETokenKind::Identifier)
                    {
                        std::string BaseName = ParseQualifiedName(InTokens, Cursor);
                        if (bSawPublic)
                        {
                            Base = BaseName;
                        }
                    }
                    // Skip past any further bases / template args until '{'.
                    while (!IsPunct(At(InTokens, Cursor), '{') && At(InTokens, Cursor).Kind != ETokenKind::EndOfFile)
                    {
                        ++Cursor;
                    }
                }

                if (IsPunct(At(InTokens, Cursor), '{'))
                {
                    FScope Scope;
                    Scope.Kind = EScopeKind::Class;
                    if (Pending == EMarker::Class)
                    {
                        SReflectedClass ReflectedClass;
                        std::string Qualified;
                        for (const std::string& Namespace : NamespaceStack)
                        {
                            if (!Namespace.empty())
                            {
                                Qualified += Namespace + "::";
                            }
                        }
                        Qualified += Name;
                        ReflectedClass.QualifiedName = Qualified;
                        ReflectedClass.Base = Base;
                        Scope.ClassData = std::move(ReflectedClass);
                    }
                    Scopes.push_back(std::move(Scope));
                    Pending = EMarker::None;
                    Index = Cursor + 1;
                    continue;
                }
            }

            // Didn't match a recognizable pattern (e.g. an in-place variable
            // declaration like "struct { int x; } anon;") -- skip safely.
            Pending = EMarker::None;
            Index = SkipStatementOrBlock(InTokens, Index);
            continue;
        }

        // enum [class] Name [: Underlying] { ... } ;
        if (IsIdent(Token, "enum"))
        {
            size_t Cursor = Index + 1;
            const bool bIsEnumClass = IsIdent(At(InTokens, Cursor), "class") || IsIdent(At(InTokens, Cursor), "struct");
            if (bIsEnumClass)
            {
                ++Cursor;
            }
            std::string Name;
            if (At(InTokens, Cursor).Kind == ETokenKind::Identifier)
            {
                Name = ParseQualifiedName(InTokens, Cursor);
            }
            std::string Underlying;
            if (IsPunct(At(InTokens, Cursor), ':'))
            {
                ++Cursor;
                size_t TypeStart = Cursor;
                while (!IsPunct(At(InTokens, Cursor), '{') && !IsPunct(At(InTokens, Cursor), ';') && At(InTokens, Cursor).Kind != ETokenKind::EndOfFile)
                {
                    ++Cursor;
                }
                Underlying = JoinTypeTokens(InTokens, TypeStart, Cursor);
            }

            if (IsPunct(At(InTokens, Cursor), '{'))
            {
                const size_t Close = FindMatchingBracket(InTokens, Cursor, '{', '}');
                if (Pending == EMarker::Enum && !Name.empty())
                {
                    SReflectedEnum ReflectedEnum;
                    std::string Qualified;
                    for (const std::string& Namespace : NamespaceStack)
                    {
                        if (!Namespace.empty())
                        {
                            Qualified += Namespace + "::";
                        }
                    }
                    Qualified += Name;
                    ReflectedEnum.QualifiedName = Qualified;
                    ReflectedEnum.bIsEnumClass = bIsEnumClass;
                    ReflectedEnum.UnderlyingType = Underlying;
                    ParseEnumerators(InTokens, Cursor + 1, Close, ReflectedEnum.Values);
                    Result.Enums.push_back(std::move(ReflectedEnum));
                }
                Pending = EMarker::None;
                Index = Close + 1;
                if (IsPunct(At(InTokens, Index), ';'))
                {
                    ++Index;
                }
                continue;
            }

            // "enum Name ;" forward declaration, or something unrecognized.
            Pending = EMarker::None;
            Index = SkipStatementOrBlock(InTokens, Index);
            continue;
        }

        // Inside a class scope: members.
        if (!Scopes.empty() && Scopes.back().Kind == EScopeKind::Class)
        {
            if (Pending == EMarker::Property)
            {
                Pending = EMarker::None;
                SReflectedProperty Property;
                const size_t Next = ParsePropertyDecl(InTokens, Index, Property);
                if (Scopes.back().ClassData.has_value() && !Property.Name.empty())
                {
                    Scopes.back().ClassData->Properties.push_back(std::move(Property));
                }
                Index = Next;
                continue;
            }

            if (Pending == EMarker::Function)
            {
                Pending = EMarker::None;
                SReflectedFunction Function;
                bool bReflected = false;
                const size_t Next = ParseFunctionDecl(InTokens, Index, Function, bReflected);
                if (bReflected && Scopes.back().ClassData.has_value())
                {
                    Scopes.back().ClassData->Functions.push_back(std::move(Function));
                }
                Index = Next;
                continue;
            }

            if ((IsIdent(Token, "public") || IsIdent(Token, "private") || IsIdent(Token, "protected")) && IsPunct(At(InTokens, Index + 1), ':'))
            {
                Index += 2;
                continue;
            }

            if (IsPunct(Token, '}'))
            {
                if (Scopes.back().ClassData.has_value())
                {
                    Result.Classes.push_back(std::move(*Scopes.back().ClassData));
                }
                Scopes.pop_back();
                ++Index;
                if (IsPunct(At(InTokens, Index), ';'))
                {
                    ++Index;
                }
                continue;
            }

            // Unmarked member (field, method, nested type, using-decl, ...): skip it.
            Pending = EMarker::None;
            Index = SkipStatementOrBlock(InTokens, Index);
            continue;
        }

        // Inside a namespace scope, not otherwise handled above.
        if (!Scopes.empty() && Scopes.back().Kind == EScopeKind::Namespace && IsPunct(Token, '}'))
        {
            NamespaceStack.pop_back();
            Scopes.pop_back();
            ++Index;
            continue;
        }

        // Top-level junk: using-declarations, free functions, forward decls, etc.
        Pending = EMarker::None;
        Index = SkipStatementOrBlock(InTokens, Index);
    }

    return Result;
}