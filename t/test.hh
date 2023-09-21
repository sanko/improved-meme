#ifndef ROCKEN_T_TEST__H_
#define ROCKEN_T_TEST__H_

#include <functional>
#include <string>
#include <vector>

#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/xchar.h> // My strings are wide

namespace Brocken
{
// https://testanything.org/tap-version-14-specification.html
// TODO:
//   - Escaping
//   - SKIP directive
//   - TODO directive
//   - BAILOUT directive
//   - running without a plan
//   - automatic diagnostics on test failure
//   - quiet subtests
using Subtest = std::function<void()>;

class Test
{
  public:
    Test(int plan, int indent) : plan_(plan), indent_(indent), test_number_(0), pass__(true) {
        //~ if (indent_ == 0) fmt::print("TAP version 14\n");
        if (indent_ == 0) fmt::print("TAP version 13\n");
        fmt::print("{:{}}1..{}\n", "", indent_ * 4, plan_);
    }
    ~Test() {}

    bool __ok(bool value, const std::string &desc) {
        test_number_++;
        if (value) {
            fmt::print("{:{}}ok {} - {}\n", "", indent_ * 4, test_number_, desc);
            pass_.push_back(test_number_);
            return true;
        }
        pass__ = false;
        fail_.push_back(test_number_);
        fmt::print("{:{}}not ok {} - {}\n", "", indent_ * 4, test_number_,
                   fmt::styled(desc, fmt::fg(fmt::color::red)));
        return false;
    }

    bool _ok(bool value, const std::string &desc) { return __ok(value, desc); }
    bool _is(auto left, auto right, const std::string &desc) {
        bool okay = __ok(left == right, desc);
        //~ if (!okay) {
        //~ fmt::print("{:{}}---\n", "", (indent_ * 4) + 2);
        //~ fmt::print("{:{}}left: {}\n", "", (indent_ * 4) + 2, left);
        //~ fmt::print("{:{}}right: {}\n", "", (indent_ * 4) + 2, right);
        //~ fmt::print("{:{}}...\n", "", (indent_ * 4) + 2);
        //~ }
        return okay;
    }
    bool _isnt(auto left, auto right, const std::string &desc) { return __ok(left != right, desc); }
    bool _subtest(const std::string &desc, int count, Subtest tests) {
        desc_ = desc;
        fmt::print("{:{}}# Subtest: {}\n", "", indent_ * 4, desc);
        tests();
        return fail_.empty();
    }

    bool _diag(const std::string &desc) {
        fmt::print(fg(fmt::color::beige), "{:{}}# {}\n", "", indent_ * 4, desc);
        return true;
    }
    void _test_count_check() {
        if (test_number_ != plan_) {
            fmt::print(fg(fmt::color::beige), "{:{}}# Looks like you planned {} tests but ran {}\n",
                       "", indent_ * 4, plan_, test_number_);
            pass__ = false;
        }
    }

    int _done_testing() {
        fmt::print("{:-<20}\n", "");
        _test_count_check();
        if (!pass__) return 255;
        if (fail_.size() > 0) {
            fmt::print(fmt::fg(fmt::color::red) | fmt::emphasis::bold,
                       "FAILED {} test{}:", fail_.size(), fail_.size() == 1 ? "" : "s");
            fmt::print(fmt::emphasis::bold, " {}\n", fmt::join(fail_, ", "));
            return fail_.size(); // exit with error code
        }
        fmt::print(fmt::fg(fmt::color::green) | fmt::emphasis::bold, "\nAll tests passed\n");
        return 0;
    }

  private:
    std::vector<int> fail_;
    std::vector<int> pass_;

  public:
    int test_number_, plan_;
    int indent_;
    bool pass__;
    std::string desc_;
};

} // namespace Brocken

// Nice API
#define plan(tests)                                                                                \
    int main() {                                                                                   \
        Brocken::Test t = { tests, 0 }
#define diag(desc) t._diag(desc)
#define ok(test, desc) t._ok(test, desc)
#define pass(desc) t._ok(true, desc)
#define is(left, right, desc) t._is(left, right, desc)
#define isnt(left, right, desc) t._is(left, right, desc)
#define subtest(tests, desc, code_block)                                                           \
    do {                                                                                           \
        t._subtest(desc, tests, [&]() {                                                            \
            Brocken::Test *parent = &t, t = {tests, parent->indent_ + 1};                          \
            code_block;                                                                            \
            t._test_count_check();                                                                 \
            if (parent != nullptr) { parent->_ok(t.pass__, desc); }                                \
        });                                                                                        \
    } while (false)
#define done_testing                                                                               \
    return t._done_testing();                                                                      \
    }

#endif // ROCKEN_T_TEST__H_
