#include "config.hh"
#include "lexer.hh"
#include "rocken.hh"
#include "test.hh"
#include "token.hh"

#include <cstdint>
#include <string>
#include <vector>

plan(5);

subtest(9, "Lexer [1.3.0]", {
    std::u32string code = U"=+(){},;";
    Rocken::Lexer lex = Rocken::Lexer(code);
    is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::ASSIGN, U"="}), "=");
    is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::PLUS, U"+"}), "+");
    is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LPAREN, U"("}), "(");
    is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::RPAREN, U")"}), ")");
    is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LBRACE, U"{"}), "{");
    is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::RBRACE, U"}"}), "}");
    is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::COMMA, U","}), ",");
    is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    is(lex.nextToken().type(), Rocken::TokenType::_EOF, "...eof");
});

subtest(7, "Lexer [1.3.1]", {
    std::u32string code = U"my $five = 5;"
                          U"my $ten = 10;"
                          U"my $add = sub($x, $y) {"
                          U"   $x + $y;"
                          U"};"
                          U"my $result = add($five, $ten);";

    Rocken::Lexer lex = Rocken::Lexer(code);

    subtest(5, "my $five = 5;", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LET, U"my"}), "my");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"five"}), "$five");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::ASSIGN, U"="}), "=");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::NUMERIC, U"5"}), "5");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    subtest(5, "my $ten = 10;", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LET, U"my"}), "my");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"ten"}), "$ten");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::ASSIGN, U"="}), "=");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::NUMERIC, U"10"}), "10");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    subtest(10, "my $add = sub($x, $y) {", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LET, U"my"}), "my");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"add"}), "$add");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::ASSIGN, U"="}), "=");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::FUNCTION, U"sub"}), "sub");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LPAREN, U"("}), "(");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"$x"}), "$x");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::COMMA, U","}), ",");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"$y"}), "$y");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::RPAREN, U")"}), ")");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LBRACE, U"{"}), "{");
    });
    subtest(4, "   $x + $y;", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"$x"}), "$x");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::PLUS, U"+"}), "+");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"$y"}), "$y");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    subtest(2, "};", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::RBRACE, U"}"}), "}");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    subtest(10, "my $result = add($five, $ten);", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LET, U"my"}), "my");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"result"}), "$result");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::ASSIGN, U"="}), "=");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::IDENT, U"add"}), "add");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LPAREN, U"("}), "(");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"$x"}), "$x");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::COMMA, U","}), ",");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"$y"}), "$y");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::RPAREN, U")"}), ")");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });

    is(lex.nextToken().type(), Rocken::TokenType::_EOF, "...eof");
});

subtest(9, "Lexer [1.3.2]", {
    std::u32string code = U"my $five = 5;"
                          U"my $ten = 10;"
                          U"my $add = sub($x, $y) {"
                          U"   $x + $y;"
                          U"};"
                          U"my $result = add($five, $ten);"
                          U"!-/*5;"
                          U"5 < 10 > 5;";

    Rocken::Lexer lex = Rocken::Lexer(code);

    subtest(5, "my $five = 5;", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LET, U"my"}), "my");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"five"}), "$five");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::ASSIGN, U"="}), "=");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::NUMERIC, U"5"}), "5");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    subtest(5, "my $ten = 10;", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LET, U"my"}), "my");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"ten"}), "$ten");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::ASSIGN, U"="}), "=");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::NUMERIC, U"10"}), "10");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    subtest(10, "my $add = sub($x, $y) {", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LET, U"my"}), "my");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"add"}), "$add");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::ASSIGN, U"="}), "=");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::FUNCTION, U"sub"}), "sub");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LPAREN, U"("}), "(");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"$x"}), "$x");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::COMMA, U","}), ",");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"$y"}), "$y");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::RPAREN, U")"}), ")");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LBRACE, U"{"}), "{");
    });
    subtest(4, "   $x + $y;", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"$x"}), "$x");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::PLUS, U"+"}), "+");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"$y"}), "$y");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    subtest(2, "};", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::RBRACE, U"}"}), "}");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    subtest(10, "my $result = add($five, $ten);", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LET, U"my"}), "my");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"result"}), "$result");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::ASSIGN, U"="}), "=");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::IDENT, U"add"}), "add");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LPAREN, U"("}), "(");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"$x"}), "$x");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::COMMA, U","}), ",");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"$y"}), "$y");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::RPAREN, U")"}), ")");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    subtest(6, "!-/*5;", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::BANG, U"!"}), "!");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::MINUS, U"-"}), "-");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::FSLASH, U"/"}), "/");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::ASTERISK, U"*"}), "*");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::NUMERIC, U"5"}), "5");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    subtest(6, "5 < 10 > 5;", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::NUMERIC, U"5"}), "5");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LT, U"<"}), "<");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::NUMERIC, U"10"}), "10");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::GT, U">"}), ">");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::NUMERIC, U"5"}), "5");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    is(lex.nextToken().type(), Rocken::TokenType::_EOF, "...eof");
});

subtest(14, "Lexer [1.3.3]", {
    std::u32string code = U"my $five = 5;"
                          U"my $ten = 10;"
                          U"my $add = sub($x, $y) {"
                          U"   $x + $y;"
                          U"};"
                          U"my $result = add($five, $ten);"
                          U"!-/*5;"
                          U"5 < 10 > 5;"
                          U"if (5 < 10) {"
                          U"    return true;"
                          U"} else {"
                          U"    return false;"
                          U"}";

    Rocken::Lexer lex = Rocken::Lexer(code);

    subtest(5, "my $five = 5;", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LET, U"my"}), "my");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"five"}), "$five");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::ASSIGN, U"="}), "=");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::NUMERIC, U"5"}), "5");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    subtest(5, "my $ten = 10;", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LET, U"my"}), "my");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"ten"}), "$ten");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::ASSIGN, U"="}), "=");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::NUMERIC, U"10"}), "10");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    subtest(10, "my $add = sub($x, $y) {", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LET, U"my"}), "my");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"add"}), "$add");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::ASSIGN, U"="}), "=");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::FUNCTION, U"sub"}), "sub");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LPAREN, U"("}), "(");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"$x"}), "$x");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::COMMA, U","}), ",");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"$y"}), "$y");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::RPAREN, U")"}), ")");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LBRACE, U"{"}), "{");
    });
    subtest(4, "   $x + $y;", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"$x"}), "$x");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::PLUS, U"+"}), "+");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"$y"}), "$y");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    subtest(2, "};", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::RBRACE, U"}"}), "}");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    subtest(10, "my $result = add($five, $ten);", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LET, U"my"}), "my");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"result"}), "$result");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::ASSIGN, U"="}), "=");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::IDENT, U"add"}), "add");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LPAREN, U"("}), "(");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"$x"}), "$x");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::COMMA, U","}), ",");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"$y"}), "$y");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::RPAREN, U")"}), ")");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    subtest(6, "!-/*5;", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::BANG, U"!"}), "!");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::MINUS, U"-"}), "-");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::FSLASH, U"/"}), "/");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::ASTERISK, U"*"}), "*");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::NUMERIC, U"5"}), "5");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    subtest(6, "5 < 10 > 5;", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::NUMERIC, U"5"}), "5");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LT, U"<"}), "<");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::NUMERIC, U"10"}), "10");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::GT, U">"}), ">");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::NUMERIC, U"5"}), "5");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    subtest(7, "if (5 < 10) {", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::IF, U"if"}), "if");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LPAREN, U"("}), "(");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::NUMERIC, U"5"}), "5");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LT, U"<"}), "<");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::NUMERIC, U"10"}), "10");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::RPAREN, U")"}), ")");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LBRACE, U"{"}), "{");
    });
    subtest(3, "    return true;", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::RETURN, U"return"}), "return");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::TRUE, U"true"}), "true");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    subtest(3, "} else {", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::RBRACE, U"}"}), "}");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::ELSE, U"else"}), "else");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LBRACE, U"{"}), "{");
    });
    subtest(3, "    return false;", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::RETURN, U"return"}), "return");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::FALSE, U"false"}), "false");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    subtest(1, "}", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::RBRACE, U"}"}), "}");
    });
    is(lex.nextToken().type(), Rocken::TokenType::_EOF, "...eof");
});

subtest(16, "Lexer [1.3.5]", {
    std::u32string code = U"my $five = 5;"
                          U"my $ten = 10;"
                          U"my $add = sub($x, $y) {"
                          U"   $x + $y;"
                          U"};"
                          U"my $result = add($five, $ten);"
                          U"!-/*5;"
                          U"5 < 10 > 5;"
                          U"if (5 < 10) {"
                          U"    return true;"
                          U"} else {"
                          U"    return false;"
                          U"}"
                          U"10 == 10;"
                          U"10 != 9;";

    Rocken::Lexer lex = Rocken::Lexer(code);

    subtest(5, "my $five = 5;", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LET, U"my"}), "my");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"five"}), "$five");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::ASSIGN, U"="}), "=");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::NUMERIC, U"5"}), "5");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    subtest(5, "my $ten = 10;", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LET, U"my"}), "my");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"ten"}), "$ten");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::ASSIGN, U"="}), "=");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::NUMERIC, U"10"}), "10");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    subtest(10, "my $add = sub($x, $y) {", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LET, U"my"}), "my");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"add"}), "$add");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::ASSIGN, U"="}), "=");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::FUNCTION, U"sub"}), "sub");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LPAREN, U"("}), "(");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"$x"}), "$x");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::COMMA, U","}), ",");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"$y"}), "$y");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::RPAREN, U")"}), ")");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LBRACE, U"{"}), "{");
    });
    subtest(4, "   $x + $y;", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"$x"}), "$x");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::PLUS, U"+"}), "+");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"$y"}), "$y");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    subtest(2, "};", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::RBRACE, U"}"}), "}");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    subtest(10, "my $result = add($five, $ten);", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LET, U"my"}), "my");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"result"}), "$result");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::ASSIGN, U"="}), "=");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::IDENT, U"add"}), "add");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LPAREN, U"("}), "(");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"$x"}), "$x");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::COMMA, U","}), ",");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::VARIABLE, U"$y"}), "$y");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::RPAREN, U")"}), ")");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    subtest(6, "!-/*5;", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::BANG, U"!"}), "!");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::MINUS, U"-"}), "-");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::FSLASH, U"/"}), "/");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::ASTERISK, U"*"}), "*");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::NUMERIC, U"5"}), "5");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    subtest(6, "5 < 10 > 5;", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::NUMERIC, U"5"}), "5");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LT, U"<"}), "<");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::NUMERIC, U"10"}), "10");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::GT, U">"}), ">");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::NUMERIC, U"5"}), "5");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    subtest(7, "if (5 < 10) {", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::IF, U"if"}), "if");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LPAREN, U"("}), "(");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::NUMERIC, U"5"}), "5");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LT, U"<"}), "<");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::NUMERIC, U"10"}), "10");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::RPAREN, U")"}), ")");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LBRACE, U"{"}), "{");
    });
    subtest(3, "    return true;", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::RETURN, U"return"}), "return");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::TRUE, U"true"}), "true");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    subtest(3, "} else {", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::RBRACE, U"}"}), "}");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::ELSE, U"else"}), "else");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LBRACE, U"{"}), "{");
    });
    subtest(3, "    return false;", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::RETURN, U"return"}), "return");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::FALSE, U"false"}), "false");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    subtest(1, "}", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::RBRACE, U"}"}), "}");
    });
    subtest(4, "10 == 10", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::NUMERIC, U"10"}), "10");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::EQ_NUMERIC, U"=="}), "==");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::NUMERIC, U"10"}), "10");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    subtest(4, "10 == 10", {
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::NUMERIC, U"10"}), "10");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::NE_NUMERIC, U"!="}), "!=");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::NUMERIC, U"9"}), "9");
        is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    });
    is(lex.nextToken().type(), Rocken::TokenType::_EOF, "...eof");
});

done_testing;
