#include <bios/ut.hpp>

using namespace bios::ut;
using namespace bios::ut::literal;

suite test_basic{[](){
    "Basic arithmetic test"_test = [] {
        bios::ut::expect(2_i == 2);
        bios::ut::expect(10_i > 5);
    };

    "Intentional fail test"_test = [] {
        bios::ut::expect(6_i == 5);
    };
}};

suite<"named suite"> test_named_suite{[](){
    "Named suite test"_test = [] {
        bios::ut::expect(3_i == 3);
    };
}};

int main() {
    "main defined test"_test = [] {
        bios::ut::expect(1_i == 1);
    };
    return 0;
}
