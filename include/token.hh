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
  IDENT, // add, foobar, x, y, ...
  INT,   // 1343456

  // Operators
  ASSIGN, // =
  PLUS,   // +

  // Delimiters
  COMMA,     // ,
  SEMICOLON, // ;
  //
  LPAREN,   // (
  RPAREN,   // )
  LBRACE,   // {
  RBRACE,   // }

  // Keywords
  FUNCTION, // "sub"
  LET       // "LET"
};
class Token
{
public:
  TokenType type;
  std::string literal; // raw
public:
  Token(TokenType type, std::string literal) {
    this->type = type;
    this->literal = literal;
  }
};
}; // namespace Rocken

#endif // ROCKEN_INCLUDE_TOKEN__H_
