#ifndef ROCKEN_INCLUDE_AST__H_
#define ROCKEN_INCLUDE_AST__H_

#include "config.hh"
#include "token.hh"

#include <memory>
#include <vector>

namespace Rocken
{
class _API Node
{
  public:
    virtual ~Node() = default;
    virtual std::u32string literal() = 0;
    virtual std::u32string string() = 0;
    // virtual std::u32string type() = 0;

  protected:
    Rocken::Token token_;
};

class Statement : public Node
{
  public:
    Statement() {}
    ~Statement() override {}

    std::u32string literal() override { return token_.literal(); }
    std::u32string string() override { return token_.literal(); }

    // virtual std::u32string type() = 0;
    //  Statement could hold many other statements
    //~ std::u32string Statement() { return token_.literal(); }
    //~ ~Statement(){};
};

class Expression : public Node
{
  public:
    virtual std::u32string literal() = 0;
    virtual std::u32string string() = 0;
    // virtual std::u32string type() = 0;

    bool operator==(const Expression &exp) {
        //~ if (type_ == tok.type_ /* && literal_ == tok.literal_*/) return true;
        return false;
    }
};

class Program : public Node
{
  public:
    std::u32string literal() {
        if (statements_.size() > 0) {
          return statements_[0].get()->literal();
          }
        return U"";
    }

    std::u32string string() {
        std::u32string info{};
        //~ for (auto &statement : statements_) {
            //~ info += statement->string();
        //~ }
        return std::move(info);
    }

    // std::u32string type() { U""; };
    std::vector<std::unique_ptr<Statement>> statements_;
};

class Identifier : public Expression
{
  public:
    Identifier(Token token) : token_(token){};
    std::u32string literal() { return token_.literal(); };
    std::u32string string() { return value; };
    // std::u32string type() { return U"Identifier"; };

  private:
    Token token_;
    std::u32string value;
};

class LetStatement : public Statement
{
  public:
    LetStatement(Token token) : token_(token){};
    ~LetStatement() = default;

    std::u32string literal() { return token_.literal(); };
    std::u32string string();
    std::u32string type() { return U"my"; };

    std::unique_ptr<Identifier> name_;
    // TODO: accept more than one identifier such as `my ($blah, $blah2)`

  private:
    Token token_;
    std::unique_ptr<Expression> value_;
};

/*
using Subtest = std::function<void()>;

class AST
{
public:
Test(int plan, int indent) : plan_(plan), indent_(indent), test_number_(0), pass__(true) {
    if (indent_ == 0) fmt::print("TAP version 14\n");
    fmt::print("{:{}}1..{}\n", "", indent_ * 4, plan_);
}
~Test() {}
}
*/
} // namespace Rocken

#endif // ROCKEN_INCLUDE_AST__H_
