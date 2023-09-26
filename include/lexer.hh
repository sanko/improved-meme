#ifndef ROCKEN_INCLUDE_LEXER__H_
#define ROCKEN_INCLUDE_LEXER__H_
#include "token.hh"
#include <string>
namespace Rocken
{
class Lexer
{
public:
  Lexer(const std::string &input) : input_(input), pos_(0) {}

  Rocken::Token NextToken() {
    if (pos_ >= input_.length()) { return Rocken::Token{Rocken::TokenType::_EOF, ""}; }
    char c = input_[pos_];
    ++pos_;

    switch (c) {
    case '=':
      return Rocken::Token{Rocken::TokenType::ASSIGN, "="};
    case '+':
      return Rocken::Token{Rocken::TokenType::PLUS, "+"};
    case '(':
      return Rocken::Token{Rocken::TokenType::LPAREN, "("};
    case ')':
      return Rocken::Token{Rocken::TokenType::RPAREN, ")"};
    case '{':
      return Rocken::Token{Rocken::TokenType::LBRACE, "{"};
    case '}':
      return Rocken::Token{Rocken::TokenType::RBRACE, "}"};
    case ',':
      return Rocken::Token{Rocken::TokenType::COMMA, ","};
    case ';':
      return Rocken::Token{Rocken::TokenType::SEMICOLON, ";"};
    default:
      // TODO: Handle other characters
      return Rocken::Token{TokenType::_ILLEGAL, ""};
    }
  }

private:
  const std::string &input_;
  size_t pos_;
  size_t pos_line_;
  // Store line, file, etc.
};
};     // namespace Rocken
#endif // ROCKEN_INCLUDE_LEXER__H_
