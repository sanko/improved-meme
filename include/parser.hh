#ifndef ROCKEN_INCLUDE_PARSER__H_
#define ROCKEN_INCLUDE_PARSER__H_

#include "ast.hh"
#include "lexer.hh"
#include "token.hh"

#include <fmt/core.h>

#include <memory>
#include <string_view>
#include <vector>


namespace Rocken
{

class _API Parser
{
  public:
    //Parser() = delete;
    Parser(Rocken::Lexer lexer) : lexer_(lexer) {
        // prime the pump
        nextToken();
        nextToken();
    };
    ~Parser() {}
#if BROCKEN_TESTS
    bool operator==(const Parser &exp) {
        //~ if (type_ == tok.type_ /* && literal_ == tok.literal_*/) return true;
        return false;
    }
#endif // BROCKEN_TESTS

    void nextToken() {
        current_ = peek_;
        peek_ = lexer_.nextToken();
    }

    std::unique_ptr<Rocken::Program> parseProgram();
    std::unique_ptr<Rocken::Statement> parseStatement();

    std::unique_ptr<Rocken::LetStatement> parseLetStatement();

    bool peekTokenIs(Rocken::TokenType type);
    bool expectPeek(Rocken::TokenType type);

    std::vector<std::string_view> errors() {}
    void peekError(Token token) {
        std::string_view msg = fmt::format("Expected next token to be {}, got {} instead",
                                           (int)token.type(), (int)peek_.type());
        errors_.push_back(msg);
    }

  private:
    Lexer lexer_;
    Token current_;
    Token peek_;
    Program program_;
    std::vector<std::string_view> errors_;
};

} // namespace Rocken

#endif // ROCKEN_INCLUDE_PARSER__H_
