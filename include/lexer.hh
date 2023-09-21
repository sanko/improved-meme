#ifndef ROCKEN_INCLUDE_LEXER__H_
#define ROCKEN_INCLUDE_LEXER__H_

#include "rocken.hh"
#include "token.hh"

#include <string>
#include <unordered_map>

#ifdef __cplusplus
extern "C" {
#endif

namespace Rocken
{

enum class LexerState
{
    _INVALID,
    Sstring, // single quoted string
    Dstring, // double-quoted string (interpolation)
    Identifier,
    Number,
    Comment,
    Block,
    Heredoc
};

class Lexer
{
  public:
    Lexer(std::u32string &input) : input_(input), pos_(0) {
        keywords_.emplace(U"sub", Rocken::TokenType::FUNCTION);
        keywords_.emplace(U"my", Rocken::TokenType::LET);
        keywords_.emplace(U"true", Rocken::TokenType::TRUE);
        keywords_.emplace(U"false", Rocken::TokenType::FALSE);
        keywords_.emplace(U"if", Rocken::TokenType::IF);
        keywords_.emplace(U"elsif", Rocken::TokenType::ELSIF);
        keywords_.emplace(U"else", Rocken::TokenType::ELSE);
        keywords_.emplace(U"return", Rocken::TokenType::RETURN);
    }
    ~Lexer(){};
    EXPORT Rocken::Token nextToken();

  protected:
    char32_t readChar();       // Moves carriage
    char32_t peekChar();       // Does not move carriage
    char32_t peekChar(size_t); // Does not move carriage

    Rocken::Token readIdentifier();
    Rocken::Token readVariable();
    Rocken::Token readNumber();
    Rocken::Token lookupIdentifier(std::u32string);
    void eatWhitespace();

  private:
    std::u32string &input_;
    size_t pos_ = 0;
    size_t pos_line_ = 1;
    LexerState state_ = Rocken::LexerState::_INVALID;
    std::unordered_map<std::u32string, Rocken::TokenType> keywords_;
    // Store line, file, etc.
};
}; // namespace Rocken
#ifdef __cplusplus
} // extern "C"
#endif
  // namespace Rocken
#endif // ROCKEN_INCLUDE_LEXER__H_
