#pragma once

#include "string"
#include "vector"


enum class ETokenKind
{
    Identifier,
    Number,
    StringLiteral,
    CharLiteral,
    // "::"
    ScopeResolution, 
    // any other single character: { } ( ) [ ] < > , ; & * = + - etc.
    Punct,           
    EndOfFile
};

struct EToken
{
    ETokenKind Kind;
    std::string Text;
    int Line;
};

std::vector<EToken> Tokenize(const std::string& InSource);