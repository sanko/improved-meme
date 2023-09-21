#include "lexer.hh"
#include <string_view>

bool is_letter(char32_t ch) {
    return 'a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z' || ch == '_';
}

std::u32string Rocken::Lexer::readIdentifier() {
    std::u32string ret;
    while (is_letter(peekChar())){
        ret += input_.at(pos_++);
    }
    return ret;
}

Rocken::Token Rocken::Lexer::NextToken() {
    if (pos_ >= input_.size()) {
        return Rocken::Token{Rocken::TokenType::_EOF, std::u32string_view{nullptr, 0}};
    }
    ch_ = input_.at(pos_);
    std::u32string_view literal{reinterpret_cast<const char32_t *>(&ch_), 1};
    ++pos_;

    switch (ch_) {
    case '=':
        return Rocken::Token{Rocken::TokenType::ASSIGN, literal};
    case '+':
        return Rocken::Token{Rocken::TokenType::PLUS, literal};
    case '-':
        return Rocken::Token{Rocken::TokenType::MINUS, literal};
    case '!':
        return Rocken::Token{Rocken::TokenType::BANG, literal};
    case '/':
        return Rocken::Token{Rocken::TokenType::SLASH, literal};
    case '*':
        return Rocken::Token{Rocken::TokenType::ASTERISK, literal};
    case '<':
        return Rocken::Token{Rocken::TokenType::LT, literal};
    case '>':
        return Rocken::Token{Rocken::TokenType::GT, literal};
    case '(':
        return Rocken::Token{Rocken::TokenType::LPAREN, literal};
    case ')':
        return Rocken::Token{Rocken::TokenType::RPAREN, literal};
    case '{':
        return Rocken::Token{Rocken::TokenType::LBRACE, literal};
    case '}':
        return Rocken::Token{Rocken::TokenType::RBRACE, literal};
    case ',':
        return Rocken::Token{Rocken::TokenType::COMMA, literal};
    case ';':
        return Rocken::Token{Rocken::TokenType::SEMICOLON, literal};

    default: {
        if (!is_letter(ch_)) { return Rocken::Token{TokenType::_INVALID, literal}; }
        return Rocken::Token{Rocken::TokenType::IDENT, readIdentifier()};
    }
    }
}
