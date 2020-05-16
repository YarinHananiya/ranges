#include <iostream> // std::cout
#include <vector>   // std::vector

#include "filter_range.hpp"
#include "transform_range.hpp"

int double_num(int i) {
    return i * 2;
}

int main() {
    std::vector v{1, 2, 3, 4, 5};

    std::vector transform_result{2, 4, 6, 8, 10};
    auto transform_range = v | my_view::transform(double_num);
    auto transform_result_iter = transform_result.begin();
    for (auto i : transform_range) {
        if (i != *(transform_result_iter++)) {
            std::cout << "error\n";
            break;
        }
    }

    std::vector filter_result{2, 4};
    auto filter_range = v | my_view::filter([](int i) { return i % 2 == 0; });
    if (!std::equal(filter_range.begin(), filter_range.end(), filter_result.begin())) {
        std::cout << "error\n";
    }

    return 0;
}