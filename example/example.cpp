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

int main() {
    // Tests are automatically registered and will run upon static destruction
    // of the runner instance. Returning 0 here lets the framework do its job.
    return 0;
}
