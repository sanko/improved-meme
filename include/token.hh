#ifndef ROCKEN_INCLUDE_TOKEN__H_
#define ROCKEN_INCLUDE_TOKEN__H_

#include "rocken.hh"
#include <string_view>

namespace Rocken
{
enum class TokenType
{
    _INVALID,
    _EOF,
    // Identifiers + literals
    IDENT, // add, foobar, x, y, ...
    INT,   // 1343456

    // Operators
    ASSIGN,   // =
    PLUS,     // +
    MINUS, // -
    BANG,     // !
    ASTERISK, // *
    SLASH,    // /
    LT,       // <
    GT,       // >

    // Delimiters
    COMMA,     // ,
    SEMICOLON, // ;
    //
    LPAREN, // (
    RPAREN, // )
    LBRACE, // {
    RBRACE, // }

    // Keywords
    FUNCTION, // "sub"
    LET    ,   // "LET"

};
class Token
{
  public:
    TokenType type;
    std::u32string_view literal; // raw
  public:
    //~ Token(TokenType type, std::u32string_view literal) : type(type), literal(literal){};
    //~ Token(TokenType type, std::string_view literal)
    //~ : type(type), literal(Rocken::to_utf32(literal)){};

    Token(TokenType type, std::u32string_view literal) : type(type), literal(literal){};
};
}; // namespace Rocken

#endif // ROCKEN_INCLUDE_TOKEN__H_
