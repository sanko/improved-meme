#include "parser.hh"
#include "ast.hh"
#//include "config.hh"

#include <fmt/core.h>

std::unique_ptr<Rocken::Program> Rocken::Parser::parseProgram() {
    auto program = std::make_unique<Rocken::Program>();

    while (current_.type() != Rocken::TokenType::_EOF) {
        fmt::print("current_.type() == {}\n", (int)current_.type());
        auto statement = parseStatement();
        if (statement != nullptr) {
            //program.get()->statements_.push_back(std::move(statement));
            program.get()->statements_.push_back(std::make_unique<Statement>());
        }
        nextToken();
    }

    return std::move(program);
}

std::unique_ptr<Rocken::Statement> Rocken::Parser::parseStatement() {
    switch (current_.type()) {
    case Rocken::TokenType::LET:
        return parseLetStatement();
    //~ case Rocken::TokenType::VARIABLE:
    //~ return parseVarStatement();
    default:
        return nullptr;
    }
}

std::unique_ptr<Rocken::LetStatement> Rocken::Parser::parseLetStatement() {
    auto ret = std::make_unique<Rocken::LetStatement>(current_);
    if (!expectPeek(Rocken::TokenType::VARIABLE)) { return nullptr; }

    ret->name_ = std::make_unique<Identifier>(current_);

    if (!expectPeek(Rocken::TokenType::ASSIGN)) { return nullptr; }
    // TODO: We're skipping the expressions until we
    // encounter a semicolon
    while (!expectPeek(Rocken::TokenType::SEMICOLON)) {
        nextToken();
    }

    return ret;
}

bool Rocken::Parser::peekTokenIs(Rocken::TokenType type) {
    return peek_.type() == type;
}

bool Rocken::Parser::expectPeek(Rocken::TokenType type) {
    if (peekTokenIs(type)) {
        nextToken();
        return true;
    }
    // TODO: throw an unexpected token type error
    return false;
}
