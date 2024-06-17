#include <optional>
#include <string_view>
#include <iostream>

std::optional<size_t> search_string_core(
    const char* filename, std::string_view pattern
);


int main() {
    const auto res = search_string_core("setup.py", "import");
    if (res) {
        std::cout << "Found at position " << *res << std::endl;
    } else {
        std::cout << "Not found" << std::endl;
    }
    return 0;
}