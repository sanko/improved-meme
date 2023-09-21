#ifndef ROCKEN_INCLUDE_TOKEN__H_
#define ROCKEN_INCLUDE_TOKEN__H_

#include <string>

namespace Rocken
{
enum class TokenType
{
    _ILLEGAL,
    _EOF,
    // Identifiers + literals
    IDENT,   // add, foobar, x, y, ...
    NUMERIC, // 1343456

    // Operators
    ASSIGN,   // =
    PLUS,     // +
    MINUS,    // -
    BANG,     // !
    ASTERISK, // *
    FSLASH,   // /

    // Dereference
    BSLASH, // \

    // Inequalities
    LT,         // <
    GT,         // >
    EQ_NUMERIC, // ==
    NE_NUMERIC, // !=

    // Delimiters
    COMMA,     // ,
    SEMICOLON, // ;
    //
    LPAREN, // (
    RPAREN, // )
    LBRACE, // {
    RBRACE, // }

    // Keywords
    FUNCTION, // sub
    LET,      // my

    // Jumping ahead
    VARIABLE, // $..., %..., @..., *... (split glob off from this)

    // Bool
    TRUE,
    FALSE,

    // Control flow
    IF,
    ELSIF,
    ELSE,
    RETURN // return
};
class Token
{
  public:
    Token() {}
    Token(TokenType type, std::u32string literal) : type_(type), literal_(literal) {}
    ~Token(){};
    TokenType type() { return type_; };

    std::u32string literal() { return literal_; }

    bool operator==(const Token &tok) {
        if (type_ == tok.type_ /* && literal_ == tok.literal_*/) return true;
        return false;
    }

  private:
    TokenType type_;
    std::u32string literal_; // raw
};
}; // namespace Rocken

#endif // ROCKEN_INCLUDE_TOKEN__H_
