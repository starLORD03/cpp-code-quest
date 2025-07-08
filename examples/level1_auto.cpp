/**
 * Level 1: The Temple of Auto
 * 
 * This example demonstrates auto type deduction and generic lambdas
 * introduced in C++14.
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

int main() {
    std::cout << "🏛️ Welcome to the Temple of Auto!\n";
    std::cout << "==================================\n\n";
    
    // Auto type deduction for variables
    auto number = 42;                    // int
    auto pi = 3.14159;                   // double
    auto text = "Hello C++14";           // const char*
    auto message = std::string("Auto!"); // std::string
    
    std::cout << "Auto deduced types:\n";
    std::cout << "number: " << number << " (int)\n";
    std::cout << "pi: " << pi << " (double)\n";
    std::cout << "text: " << text << " (const char*)\n";
    std::cout << "message: " << message << " (std::string)\n\n";
    
    // Generic lambda with auto parameter (C++14)
    auto multiply = [](auto x, auto y) {
        if constexpr (std::is_same_v<decltype(x), std::string> && std::is_integral_v<decltype(y)>) {
            std::string result;
            for (int i = 0; i < y; ++i) result += x;
            return result;
        } else {
            return x * y;
        }
    };
    
    std::cout << "Generic lambda examples:\n";
    std::cout << "multiply(5, 3): " << multiply(5, 3) << "\n";
    std::cout << "multiply(2.5, 4.0): " << multiply(2.5, 4.0) << "\n";
    std::cout << "multiply(message, 2): " << multiply(message, 2) << "\n\n";
    
    // Auto with containers
    auto numbers = std::vector<int>{1, 2, 3, 4, 5};
    
    std::cout << "Auto with algorithms:\n";
    std::cout << "Original: ";
    for (const auto& n : numbers) {
        std::cout << n << " ";
    }
    std::cout << "\n";
    
    // Transform using generic lambda
    std::transform(numbers.begin(), numbers.end(), numbers.begin(),
                   [](auto n) { return n * n; });
    
    std::cout << "Squared:  ";
    for (const auto& n : numbers) {
        std::cout << n << " ";
    }
    std::cout << "\n\n";
    
    // Auto with function return type deduction (C++14)
    auto create_multiplier = [](auto factor) {
        return [factor](auto value) {
            return value * factor;
        };
    };
    
    auto double_it = create_multiplier(2);
    auto triple_it = create_multiplier(3);
    
    std::cout << "Function return type deduction:\n";
    std::cout << "double_it(10): " << double_it(10) << "\n";
    std::cout << "triple_it(7): " << triple_it(7) << "\n\n";
    
    std::cout << "🎉 You've mastered auto type deduction!\n";
    std::cout << "📜 Reward: Auto Deduction Scroll\n";
    
    return 0;
}