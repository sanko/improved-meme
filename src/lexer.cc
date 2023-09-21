//~ #include "config.hh"
#include "lexer.hh"

#include <cwctype>

char32_t Rocken::Lexer::readChar() {
    if (pos_ >= input_.length()) { return 0; }
    char32_t ch = input_.at(pos_++);
    if (ch == U'\n') pos_line_++;
    return ch;
}

char32_t Rocken::Lexer::peekChar() {
    if (pos_ >= input_.length()) { return 0; }
    return input_.at(pos_);
}

char32_t Rocken::Lexer::peekChar(size_t pos) {
    if (pos >= input_.length()) { return 0; }
    return input_.at(pos);
}

bool isLetter(char32_t ch) {
    // effected by locale so set std::setlocale(LC_ALL, "en_US.utf8") for Unicode support
    if (std::iswalnum(ch) || ch == U'_' // Typical
        || ch == U':'                   // :: - package var; : - is invalid
        || ch == U'^'                   // internal var
        || ch == U'$'                   // scalar
        || ch == U'%'                   // hash
        || ch == U'@'                   // array
        || ch == U'*'                   // GLOB
    )
        return true;
    return false;
}

bool isWhitespace(char32_t ch) {
    return std::iswblank(ch);
}

void Rocken::Lexer::eatWhitespace() {
    while (isWhitespace(peekChar())) {
        pos_++;
    }
}

bool isNumeric(char32_t ch) {
    return std::iswdigit(ch);
}

Rocken::Token Rocken::Lexer::lookupIdentifier(std::u32string identifier) {
    if (keywords_.contains(identifier)) {
        auto it = keywords_.find(identifier);
        if (it != keywords_.end()) { /// This should be false if .contains(...) worked
            // TODO: these are built-in keywords!!!
            return Rocken::Token{it->second, identifier};
        }
    }

    return Rocken::Token{Rocken::TokenType::IDENT, identifier};
}

Rocken::Token Rocken::Lexer::readIdentifier() {
    size_t length = 0;
    while (isLetter(peekChar(pos_ + length))) {
        length++;
    }
    auto ret = lookupIdentifier(input_.substr(pos_, length));
    pos_ += length;
    return ret;
}

Rocken::Token Rocken::Lexer::readVariable() {
    //~ fmt::print("readVariable [0, {}]\n", pos_);
    size_t length = 0;
    int type = 0; // default;
    switch (readChar()) {
    case U'$':
        type = 1;
        break;
    case U'%':
        type = 2;
        break;
    case U'@':
        type = 3;
        break;
    case U'*':
        type = 4;
        break;
    case U'\\':
        type = 5;
        break;
    }

    while (isLetter(peekChar(pos_ + length))) {
        length++;
    }

    auto ret = Rocken::Token{Rocken::TokenType::VARIABLE, input_.substr(pos_, length)};
    pos_ += length;
    return ret;
}

Rocken::Token Rocken::Lexer::readNumber() {
    size_t pos = pos_;
    bool isOct = false; // readChar() == U'$';
    bool isHex = false;
    bool isFloat = false;
    while (isNumeric(peekChar())) {
        pos_++;
    }
    std::u32string identifier = input_.substr(pos, pos_ - pos);
    return Rocken::Token{Rocken::TokenType::NUMERIC, identifier};
}

Rocken::Token Rocken::Lexer::nextToken() {
    if (pos_ >= input_.length()) { return Rocken::Token{Rocken::TokenType::_EOF, U""}; }
    eatWhitespace();
    char32_t ch = peekChar();
    switch (ch) {
    case U'=': {
        if (peekChar(pos_ + 1) == U'=') {
            Rocken::Token tok = {Rocken::TokenType::EQ_NUMERIC, input_.substr(pos_, 2)};
            pos_ += 2;
            return tok;
        }
        return Rocken::Token{Rocken::TokenType::ASSIGN, input_.substr(++pos_, 1)};
    }
    case U'+':
        return Rocken::Token{Rocken::TokenType::PLUS, input_.substr(++pos_, 1)};
    case U'-':
        return Rocken::Token{Rocken::TokenType::MINUS, input_.substr(++pos_, 1)};
    case U'!':
        if (peekChar(pos_ + 1) == U'=') {
            Rocken::Token tok = {Rocken::TokenType::NE_NUMERIC, input_.substr(pos_, 2)};
            pos_ += 2;
            return tok;
        }
        return Rocken::Token{Rocken::TokenType::BANG, input_.substr(++pos_, 1)};
    case U'/':
        return Rocken::Token{Rocken::TokenType::FSLASH, input_.substr(++pos_, 1)};
    case U'\\':
        return Rocken::Token{Rocken::TokenType::BSLASH, input_.substr(++pos_, 1)};
    case U'*':
        return Rocken::Token{Rocken::TokenType::ASTERISK, input_.substr(++pos_, 1)};
    case U'<':
        return Rocken::Token{Rocken::TokenType::LT, input_.substr(++pos_, 1)};
    case U'>':
        return Rocken::Token{Rocken::TokenType::GT, input_.substr(++pos_, 1)};
    case U'(':
        return Rocken::Token{Rocken::TokenType::LPAREN, input_.substr(++pos_, 1)};
    case U')':
        return Rocken::Token{Rocken::TokenType::RPAREN, input_.substr(++pos_, 1)};
    case U'{':
        return Rocken::Token{Rocken::TokenType::LBRACE, input_.substr(++pos_, 1)};
    case U'}':
        return Rocken::Token{Rocken::TokenType::RBRACE, input_.substr(++pos_, 1)};
    case ',':
        return Rocken::Token{Rocken::TokenType::COMMA, input_.substr(++pos_, 1)};
    case U';':
        return Rocken::Token{Rocken::TokenType::SEMICOLON, input_.substr(++pos_, 1)};
    case U'$':
    case U'%':
        return readVariable();
    default: {
        if (isNumeric(ch)) { return readNumber(); }
        if (isLetter(ch)) { return readIdentifier(); }
        return Rocken::Token{TokenType::_ILLEGAL, input_.substr(pos_, 1)};
    }
    }
}
