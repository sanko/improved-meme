#include <catch2/catch_test_macros.hpp>

#include "config.hh"
#include "lexer.hh"
#include "rocken.hh"
#include "token.hh"
#ifdef BROCKEN_BENCHMARKS
#include <catch2/benchmark/catch_benchmark.hpp>
#endif // BROCKEN_BENCHMARKS

#include <cstdint>
#include <string>
#include <vector>

#include <fmt/core.h>

#if 0
uint64_t fibonacci(uint64_t number) {
    return number < 2 ? number : fibonacci(number - 1) + fibonacci(number - 2);
}

#ifdef BROCKEN_BENCHMARKS
TEST_CASE("Basic tokens,"
          "[!benchmark]") {
#else
TEST_CASE("Basic tokens,") {
#endif // BROCKEN_BENCHMARKS

    //~ std::string input = "=+(){},;";

    //~ std::vector<Rocken::Token> tokens;

    //~ Rocken::Lexer lexer(input);

    //~ while (true) {
    //~ Rocken::Token token = lexer.NextToken();
    //~ if (token.type == Rocken::TokenType::_EOF) { break; }
    //~ tokens.push_back(token);
    //~ }

    REQUIRE(fibonacci(5) == 5);

    REQUIRE(fibonacci(20) == 6'765);
#ifdef BROCKEN_BENCHMARKS
    BENCHMARK("fibonacci 20") {
        return fibonacci(20);
    };
#endif // BROCKEN_BENCHMARKS

    REQUIRE(fibonacci(25) == 75'025);
#ifdef BROCKEN_BENCHMARKS
    BENCHMARK("fibonacci 25") {
        return fibonacci(25);
    };
#endif // BROCKEN_BENCHMARKS
}

#ifdef BROCKEN_BENCHMARKS
TEST_CASE("Basic Fibonacci,"
          "[!benchmark]") {
#else
TEST_CASE("Basic Fibonacci,") {
#endif // BROCKEN_BENCHMARKS
    REQUIRE(fibonacci(5) == 5);

    REQUIRE(fibonacci(20) == 6'765);
#ifdef BROCKEN_BENCHMARKS
    BENCHMARK("fibonacci 20") {
        return fibonacci(20);
    };
#endif // BROCKEN_BENCHMARKS

    REQUIRE(fibonacci(25) == 75'025);
#ifdef BROCKEN_BENCHMARKS
    BENCHMARK("fibonacci 25") {
        return fibonacci(25);
    };
#endif // BROCKEN_BENCHMARKS
}

TEST_CASE("vectors can be sized and resized", "[vector]") {
    std::vector<int> v(5);

    REQUIRE(v.size() == 5);
    REQUIRE(v.capacity() >= 5);

    SECTION("resizing bigger changes size and capacity") {
        v.resize(10);

        REQUIRE(v.size() == 10);
        REQUIRE(v.capacity() >= 10);
    }
    SECTION("resizing smaller changes size but not c``apacity") {
        v.resize(0);

        REQUIRE(v.size() == 0);
        REQUIRE(v.capacity() >= 5);
    }
    SECTION("reserving bigger changes capacity but not size") {
        v.reserve(10);

        REQUIRE(v.size() == 5);
        REQUIRE(v.capacity() >= 10);
    }
    SECTION("reserving smaller does not change size or capacity") {
        v.reserve(0);

        REQUIRE(v.size() == 5);
        REQUIRE(v.capacity() >= 5);
    }
}
#endif // if 0

#ifdef BROCKEN_BENCHMARKS
TEST_CASE("Lexer", "[!benchmark]") {
#else
TEST_CASE("Lexer") {
#endif // BROCKEN_BENCHMARKS
    SECTION("simple") {
        std::string_view code = "=+(){},;";
        Rocken::Lexer lex = Rocken::Lexer(code);

        //~ Rocken::Token tok = lex.NextToken();
        //~ CHECK(tok.type == Rocken::TokenType::ASSIGN);
        //
        CHECK(lex.NextToken().type == Rocken::TokenType::ASSIGN);
        CHECK(lex.NextToken().type == Rocken::TokenType::PLUS);
        CHECK(lex.NextToken().type == Rocken::TokenType::LPAREN);
        CHECK(lex.NextToken().type == Rocken::TokenType::RPAREN);
        CHECK(lex.NextToken().type == Rocken::TokenType::LBRACE);
        CHECK(lex.NextToken().type == Rocken::TokenType::RBRACE);
        CHECK(lex.NextToken().type == Rocken::TokenType::COMMA);
        CHECK(lex.NextToken().type == Rocken::TokenType::SEMICOLON);
        CHECK(lex.NextToken().type == Rocken::TokenType::_EOF);
        CHECK(lex.NextToken().type == Rocken::TokenType::_EOF);
    }
    SECTION("real") {
        std::string_view code = "let five = 5;"
                              "let ten = 10;"
                              "let add = fn(x, y) {"
                              "    x + y;"
                              "};"
                              "let result = add(five, ten);";
        Rocken::Lexer lex = Rocken::Lexer(code);

        //~ Rocken::Token tok = lex.NextToken();
        //~ CHECK(tok.type == Rocken::TokenType::ASSIGN);
        //
        CHECK(lex.NextToken().type == Rocken::TokenType::LET);
        CHECK(lex.NextToken().type == Rocken::TokenType::IDENT);
        CHECK(lex.NextToken().type == Rocken::TokenType::ASSIGN);
        CHECK(lex.NextToken().type == Rocken::TokenType::INT);
        CHECK(lex.NextToken().type == Rocken::TokenType::SEMICOLON);


        Rocken::Token tok = lex.NextToken();







    CHECK(tok.literal.compare(U"let") == 0);
//~ auto sub = tok.literal.substr(0, 5);
//~ fmt::print("={}=\n", static_cast<int>(sub.length()), sub.data());
std::printf(
    "[%d:%.*s]",
    tok.literal.length(),
    static_cast<int>(tok.literal.length()),
    tok.literal.data());


    //~ CHECK(tok.literal.compare(L"let") > 0);
    //~ CHECK(tok.literal.compare(L"let") == 0);

        CHECK(lex.NextToken().type == Rocken::TokenType::LET);
        CHECK(lex.NextToken().type == Rocken::TokenType::IDENT);
        CHECK(lex.NextToken().type == Rocken::TokenType::ASSIGN);
        CHECK(lex.NextToken().type == Rocken::TokenType::INT);
        CHECK(lex.NextToken().type == Rocken::TokenType::INT);
        CHECK(lex.NextToken().type == Rocken::TokenType::INT);
        CHECK(lex.NextToken().type == Rocken::TokenType::INT);
        CHECK(lex.NextToken().type == Rocken::TokenType::INT);
        CHECK(lex.NextToken().type == Rocken::TokenType::INT);

        CHECK(lex.NextToken().type == Rocken::TokenType::_EOF);
        CHECK(lex.NextToken().type == Rocken::TokenType::_EOF);
    }
}
