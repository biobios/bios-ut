#include <iostream>
#include <cstdarg>

namespace bios {
namespace ut {
namespace injector {

template <typename T>
void print_object(const T& value) {
    std::cout << value;
}

template void print_object<int>(const int& value);
template void print_object<long>(const long& value);
template void print_object<long long>(const long long& value);
template void print_object<unsigned int>(const unsigned int& value);
template void print_object<unsigned long>(const unsigned long& value);
template void print_object<unsigned long long>(const unsigned long long& value);

void printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

}
}
}