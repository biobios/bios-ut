#include "bios/ut.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace bios::ut {
namespace {
    struct test_case {
        std::string name;
        bool done = false;
        runner::test_case_func func;
        size_t expect_fail_count = 0;
        bool fatal_fail = false;

        void run() {
            if (done) {
                return;
            }
            done = true;
            func();
        }
    };

    struct test_suite {
        std::string name;
        std::vector<test_case> cases;

        void test_all() {
            for (auto& c : cases) {
                c.run();
            }
        }
    };
}

struct runner::impl_t {
    std::vector<test_suite> suites;
    bool done = false;
    int current_suite_index = -1;
    int current_case_index = -1;
    int current_suite_to_register_index = -1;
    
    /**
     * @brief 現在実行中のテストスイートを取得する
     */
    test_suite& current_suite() {
        return suites[current_suite_index];
    }

    /**
     * @brief 現在実行中のテストケースを取得する
     */
    test_case& current_case() {
        return current_suite().cases[current_case_index];
    }

    /**
     * @brief テストケースのexpect失敗を記録する
     */
    void expect_fail() {
        current_case().expect_fail_count++;
    }

    /**
     * @brief すべての未実行のテストケースを実行する
     */
    void run() {
        for (int i = 0; i < suites.size(); ++i) {
            current_suite_index = i;
            run(suites[i]);
        }
        done = true;
    }

    /**
     * @brief テストスイートの未実行のテストケースを実行する
     */
    void run(test_suite& suite) {
        std::cout << "Running test suite: " << suite.name << std::endl;
        for (int i = 0; i < suite.cases.size(); ++i) {
            current_case_index = i;
            run(suite.cases[i]);
        }
        print_result_of(suite);
    }

    /**
     * @brief テストケースを実行する
     */
    void run(test_case& case_) {
        std::cout << "Running test case: " << case_.name << std::endl;
        try {
            case_.run();
        } catch (...) {
            case_.fatal_fail = true; // fatal_fail()由来以外の例外が発生した場合もfatal_failとする
        }
        print_result_of(case_);
    }

    impl_t() = default;
    ~impl_t() {
        if (!done) {
            run();
        }
        print_result();
    }

    /**
     * @brief テストスイートの結果を出力する
     */
    void print_result_of(const test_suite& suite) {
        std::cout << "Suite: " << suite.name << std::endl;
        size_t total_cases = 0;
        size_t total_expect_fail = 0;
        size_t total_fatal_fail = 0;
        for (auto& c : suite.cases) {
            total_cases++;
            if (c.fatal_fail) {
                total_fatal_fail++;
            } else if (c.expect_fail_count > 0) {
                total_expect_fail++;
            }
        }
        std::cout
            << "Total: " << total_cases
            << ", Expect Fail: " << total_expect_fail
            << ", Fatal Fail: " << total_fatal_fail << std::endl;
    }

    /**
     * @brief テストケースの結果を出力する
     */
    void print_result_of(const test_case& case_) {
        std::cout << "Test case: " << case_.name << std::endl;
        if (case_.fatal_fail) {
            std::cout << "FATAL FAIL: " << case_.name << std::endl;
        } else if (case_.expect_fail_count > 0) {
            std::cout << "EXPECT FAIL: " << case_.name << " (" << case_.expect_fail_count << ")" << std::endl;
        } else {
            std::cout << "PASS: " << case_.name << std::endl;
        }
    }

    /**
     * @brief 最終結果を出力する
     */
    void print_result() {
        size_t total_cases = 0;
        size_t total_expect_fail = 0;
        size_t total_fatal_fail = 0;
        for (auto& s : suites) {
            for (auto& c : s.cases) {
                total_cases++;
                if (c.fatal_fail) {
                    total_fatal_fail++;
                } else if (c.expect_fail_count > 0) {
                    total_expect_fail++;
                }
                print_result_of(c);
            }
        }
        std::cout
            << "Total: " << total_cases
            << ", Expect Fail: " << total_expect_fail
            << ", Fatal Fail: " << total_fatal_fail << std::endl;
    }

    void set_suite_to_register(const std::string& suite_name) {
        for (int i = 0; i < suites.size(); ++i) {
            if (suites[i].name == suite_name) {
                current_suite_to_register_index = i;
                return;
            }
        }
        // 新しいスイートを追加する
        test_suite new_suite;
        new_suite.name = suite_name;
        suites.push_back(new_suite);
        current_suite_to_register_index = suites.size() - 1;
    }

    void register_test_case(const std::string& case_name, test_case_func func) {
        if (current_suite_to_register_index < 0) {
            throw std::runtime_error("No suite set to register");
        }
        auto& suite = suites[current_suite_to_register_index];
        test_case new_case;
        new_case.name = case_name;
        new_case.func = func;
        suite.cases.push_back(new_case);
    }
};

runner::runner() : impl(new impl_t()) {
    impl->set_suite_to_register("default");
}
runner::~runner() = default;

void runner::expect_fail() {
    auto& inst = instance();
    inst.impl->expect_fail();
}

void runner::fatal_fail() {
    throw std::runtime_error("Fatal fail");
}

void runner::set_suite_to_register(const char* suite_name) {
    auto& inst = instance();
    std::string name(suite_name);
    inst.impl->set_suite_to_register(name);
}

void runner::register_test_case(const char* case_name, test_case_func func) {
    auto& inst = instance();
    std::string name(case_name);
    inst.impl->register_test_case(name, func);
}

}