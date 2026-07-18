#pragma once

#include "Lexer.h"
#include "Model.h"


/**
 * Walks a token stream recognizing:
 *   namespace X { ... }
 *   PE_CLASS() class/struct Name [: public Base] { ... PE_GENERATED_BODY() ... };
 *   PE_ENUM() enum [class] Name [: UnderlyingType] { A, B = 2, C };
 *   PE_PROPERTY() <Type> Name[N]? = Default?;      (inside a UCLASS body)
 *   PE_FUNCTION() <ReturnType> Name(ParamType...); (inside a UCLASS body, may have a body)
 * 
 * Anything else -- unmarked members, nested types, using-declarations,
 * unrelated function bodies -- is skipped as an opaque statement or
 * brace-balanced block, so it can never desynchronize scope tracking.
 */
SParsedFile Parse(const std::vector<EToken>& InTokens);