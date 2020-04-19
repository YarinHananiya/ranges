#include <vector> // std::vector
#include <iostream> // std::cout

#include "transform_range.hpp"

int double_num(int i) {
    return i * 2;
}

int main() {
    std::vector v{1, 2, 3, 4, 5};
    std::vector result{2, 4, 6, 8, 10};
    auto range = v | my_view::transform(double_num); 
    auto result_iter = result.begin();
    for (auto i : range) {
        if (i != *(result_iter++)) {
            std::cout << "error\n";
            break;
        }
    }

    return 0;
}