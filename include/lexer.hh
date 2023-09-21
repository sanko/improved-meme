#ifndef ROCKEN_INCLUDE_LEXER__H_
#define ROCKEN_INCLUDE_LEXER__H_

#include "rocken.hh"
#include "token.hh"

#include <string_view>

namespace Rocken
{
class Lexer
{
  private:
    std::u32string input_; // TODO: make this unicode aware
    size_t pos_ = 0;
    size_t pos_line_ = 0;
    char32_t ch_ = 0;

  public:
    Lexer(std::u32string_view &input) : input_(input) {}
    Lexer(std::string_view &input) : input_(to_utf32(input)) {}
    ~Lexer() {}
    Rocken::Token NextToken();

  private:
    std::u32string readIdentifier();
    char32_t peekChar(){
      if(pos_+1 >= input_.size()) return 0;
      input_.at(pos_+1) ;}
    // Store line, file, etc.
};
};     // namespace Rocken
#endif // ROCKEN_INCLUDE_LEXER__H_
