//#include "config.hh"
#include "lexer.hh"
#include "parser.hh"
//#include "rocken.hh"
#include "test.hh"
//#include "token.hh"

#include <cstdint>
#include <string>
#include <vector>

#include <fmt/core.h>

plan(2);

subtest(4, "Parser [1.3.0]", {
    std::u32string code = U"my $x = 5;"
                          U"my $y = 10;"
                          U"my $foobar = 838383;";
    auto lex = Rocken::Lexer(code);
    auto par = Rocken::Parser(lex);
    pass("ugh");
    pass("ugh");

    auto prg = par.parseProgram();
    ok(prg != nullptr, "parserProgram()");

    is(prg.get()->statements_.size(), 3, "program has 3 statements");
});


subtest(2, "Parser [1.3.0]", {
    std::u32string code = U"my $x = 5;"
                          U"my $y = 10;"
                          U"my $foobar = 838383;";
    auto lex = Rocken::Lexer(code);
    auto par = Rocken::Parser(lex);
    pass("ugh");
    pass("ugh");
    /*
    auto prg = par.parseProgram();
    ok(prg != nullptr, "parserProgram()");

    is(prg.get()->statements_.size(), 3, "program has 3 statements");
*/
    //~ is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::ASSIGN, U"="}), "=");
    //~ is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::PLUS, U"+"}), "+");
    //~ is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LPAREN, U"("}), "(");
    //~ is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::RPAREN, U")"}), ")");
    //~ is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::LBRACE, U"{"}), "{");
    //~ is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::RBRACE, U"}"}), "}");
    //~ is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::COMMA, U","}), ",");
    //~ is(lex.nextToken(), (Rocken::Token{Rocken::TokenType::SEMICOLON, U";"}), ";");
    //~ is(lex.nextToken().type(), Rocken::TokenType::_EOF, "...eof");
});

done_testing;
