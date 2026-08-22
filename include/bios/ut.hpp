#pragma once

namespace bios {
namespace ut {
namespace helper {

template <typename T>
class unique_ptr {
    T* ptr;
public:
    unique_ptr() : ptr(nullptr) {}
    unique_ptr(T* p) : ptr(p) {}
    unique_ptr(const unique_ptr&) = delete;
    unique_ptr(unique_ptr&& other) : ptr(other.ptr) {
        other.ptr = nullptr;
    }
    unique_ptr& operator=(const unique_ptr&) = delete;
    unique_ptr& operator=(unique_ptr&& other) {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }
    ~unique_ptr() {
        delete ptr;
    }
    T* operator->() {
        return ptr;
    }
    T* get() {
        return ptr;
    }
};
template <decltype(sizeof(char)) N>
struct const_char_array {
    char value[N];
    constexpr const_char_array(const char(&arr)[N]) {
        for (decltype(N) i = 0; i < N; ++i) {
            value[i] = arr[i];
        }
    }
};
}

class runner {
public:
    using test_case_func = void(*)();
    /**
     * テストケース実行中に呼び出される関数群
     */
    /**
     * @brief テストケースのexpect失敗を記録する
     */
    static void expect_fail();
    /**
     * @brief テストケースを中断する。
     */
    static void fatal_fail();
    
    /**
     * @brief テストケースを登録するスイートを指定する
     */
    static void set_suite_to_register(const char* suite_name);
    
    /**
     * @brief テストケースを登録する
     */
    static void register_test_case(const char* case_name, test_case_func func);

private:
    static runner& instance() {
        static runner inst;
        return inst;
    }

    runner();
    ~runner();

    // @brief 最終結果を出力する
    void print_result();

    class impl_t;
    helper::unique_ptr<impl_t> impl;
};

template <typename T, typename U>
struct compare_result_eq {
    T lhs;
    U rhs;
    bool result;
    compare_result_eq(const T& l, const U& r) : lhs(l), rhs(r), result(l == r) {}
    bool is_success() const { return result; }
};

template <typename T, typename U>
struct compare_result_ne {
    T lhs;
    U rhs;
    bool result;
    compare_result_ne(const T& l, const U& r) : lhs(l), rhs(r), result(l != r) {}
    bool is_success() const { return result; }
};

template <typename T, typename U>
struct compare_result_lt {
    T lhs;
    U rhs;
    bool result;
    compare_result_lt(const T& l, const U& r) : lhs(l), rhs(r), result(l < r) {}
    bool is_success() const { return result; }
};

template <typename T, typename U>
struct compare_result_gt {
    T lhs;
    U rhs;
    bool result;
    compare_result_gt(const T& l, const U& r) : lhs(l), rhs(r), result(l > r) {}
    bool is_success() const { return result; }
};

template <typename T, typename U>
struct compare_result_le {
    T lhs;
    U rhs;
    bool result;
    compare_result_le(const T& l, const U& r) : lhs(l), rhs(r), result(l <= r) {}
    bool is_success() const { return result; }
};

template <typename T, typename U>
struct compare_result_ge {
    T lhs;
    U rhs;
    bool result;
    compare_result_ge(const T& l, const U& r) : lhs(l), rhs(r), result(l >= r) {}
    bool is_success() const { return result; }
};

template <typename T>
struct fatal_result {
    T result;
    fatal_result(const T& res) : result(res) {}
    bool is_success() const { return result.result; }
};

template <typename T>
class compare_value {
    T value;
    struct result {

    };
public:
    compare_value(const T& val) : value(val) {}

    template <typename U>
    friend compare_result_eq<T, U> operator==(const compare_value& lhs, const U& rhs) {
        return compare_result_eq<T, U>(lhs.value, rhs);
    }
    template <typename U>
    friend compare_result_ne<T, U> operator!=(const compare_value& lhs, const U& rhs) {
        return compare_result_ne<T, U>(lhs.value, rhs);
    }
    template <typename U>
    friend compare_result_lt<T, U> operator<(const compare_value& lhs, const U& rhs) {
        return compare_result_lt<T, U>(lhs.value, rhs);
    }
    template <typename U>
    friend compare_result_gt<T, U> operator>(const compare_value& lhs, const U& rhs) {
        return compare_result_gt<T, U>(lhs.value, rhs);
    }
    template <typename U>
    friend compare_result_le<T, U> operator<=(const compare_value& lhs, const U& rhs) {
        return compare_result_le<T, U>(lhs.value, rhs);
    }
    template <typename U>
    friend compare_result_ge<T, U> operator>=(const compare_value& lhs, const U& rhs) {
        return compare_result_ge<T, U>(lhs.value, rhs);
    }
    template <typename U>
    friend compare_result_eq<U, T> operator==(const U& lhs, const compare_value& rhs) {
        return compare_result_eq<U, T>(lhs, rhs.value);
    }
    template <typename U>
    friend compare_result_ne<U, T> operator!=(const U& lhs, const compare_value& rhs) {
        return compare_result_ne<U, T>(lhs, rhs.value);
    }
    template <typename U>
    friend compare_result_lt<U, T> operator<(const U& lhs, const compare_value& rhs) {
        return compare_result_lt<U, T>(lhs, rhs.value);
    }
    template <typename U>
    friend compare_result_gt<U, T> operator>(const U& lhs, const compare_value& rhs) {
        return compare_result_gt<U, T>(lhs, rhs.value);
    }
    template <typename U>
    friend compare_result_le<U, T> operator<=(const U& lhs, const compare_value& rhs) {
        return compare_result_le<U, T>(lhs, rhs.value);
    }
    template <typename U>
    friend compare_result_ge<U, T> operator>=(const U& lhs, const compare_value& rhs) {
        return compare_result_ge<U, T>(lhs, rhs.value);
    }
};

class test_register {
    const char* case_name;
public:
    constexpr test_register(const char* name) : case_name(name) {}
    void operator=(runner::test_case_func func) {
        runner::register_test_case(case_name, func);
    }
};

namespace literal {
    inline compare_value<int> operator""_i(unsigned long long value) {
        return compare_value<int>(static_cast<int>(value));
    }
    inline compare_value<long> operator""_l(unsigned long long value) {
        return compare_value<long>(static_cast<long>(value));
    }
    inline compare_value<long long> operator""_ll(unsigned long long value) {
        return compare_value<long long>(static_cast<long long>(value));
    }
    inline compare_value<unsigned int> operator""_u(unsigned long long value) {
        return compare_value<unsigned int>(static_cast<unsigned int>(value));
    }
    inline compare_value<unsigned long> operator""_ul(unsigned long long value) {
        return compare_value<unsigned long>(static_cast<unsigned long>(value));
    }
    inline compare_value<unsigned long long> operator""_ull(unsigned long long value) {
        return compare_value<unsigned long long>(static_cast<unsigned long long>(value));
    }
    constexpr test_register operator""_test(const char* name, decltype(sizeof(name))) {
        return test_register(name);
    }
};

namespace injector {
template <typename T>
void print_object(const T& value);

extern template void print_object<int>(const int& value);
extern template void print_object<long>(const long& value);
extern template void print_object<long long>(const long long& value);
extern template void print_object<unsigned int>(const unsigned int& value);
extern template void print_object<unsigned long>(const unsigned long& value);
extern template void print_object<unsigned long long>(const unsigned long long& value);

void printf(const char* format, ...);
}

template <typename T>
void expect(const T& value) {
    if (value.is_success()) {
        return;
    }
    runner::expect_fail();
}

template <typename T>
void expect(const fatal_result<T>& value) {
    if (value.is_success()) {
        return;
    }
    expect(value.result);
    runner::fatal_fail();
}

template <typename T>
fatal_result<T> fatal(const T& value) {
    return fatal_result<T>(value);
}

template <typename T>
fatal_result<T> fatal(T&& value) {
    return fatal_result<T>(static_cast<T&&>(value));
}

constexpr char default_suite_name[] = "default";

template <helper::const_char_array suite_name = default_suite_name>
class suite {
public:
    suite(void(*register_func)()) {
        runner::set_suite_to_register(suite_name.value);
        register_func();
        runner::set_suite_to_register(default_suite_name);
    }
};

}
}