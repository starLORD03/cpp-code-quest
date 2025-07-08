/**
 * Level 2: The Lambda Sanctuary
 * 
 * This example demonstrates advanced lambda features including
 * generalized capture (init capture) introduced in C++14.
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>

int main() {
    std::cout << "🌟 Welcome to the Lambda Sanctuary!\n";
    std::cout << "===================================\n\n";
    
    // Traditional lambda captures
    int multiplier = 3;
    auto traditional_lambda = [multiplier](int x) {
        return x * multiplier;
    };
    
    std::cout << "Traditional capture:\n";
    std::cout << "3 * 5 = " << traditional_lambda(5) << "\n\n";
    
    // Generalized capture (init capture) - C++14
    auto ptr = std::make_unique<int>(42);
    auto lambda_with_move = [p = std::move(ptr)](auto multiplier) {
        return *p * multiplier;
    };
    
    std::cout << "Generalized capture with move:\n";
    std::cout << "42 * 2 = " << lambda_with_move(2) << "\n\n";
    
    // Creating new variables in capture
    auto counter_lambda = [count = 0](auto increment) mutable {
        count += increment;
        return count;
    };
    
    std::cout << "Init capture with new variable:\n";
    std::cout << "Counter: " << counter_lambda(5) << "\n";
    std::cout << "Counter: " << counter_lambda(3) << "\n";
    std::cout << "Counter: " << counter_lambda(2) << "\n\n";
    
    // Complex example: capturing by move and creating new state
    auto data = std::make_unique<std::vector<int>>(std::vector<int>{1, 2, 3, 4, 5});
    
    auto processor = [vec = std::move(data), sum = 0](auto operation) mutable {
        sum = 0;
        for (const auto& item : *vec) {
            sum += operation(item);
        }
        return sum;
    };
    
    std::cout << "Complex generalized capture:\n";
    std::cout << "Sum of squares: " << processor([](auto x) { return x * x; }) << "\n";
    std::cout << "Sum of cubes: " << processor([](auto x) { return x * x * x; }) << "\n\n";
    
    // Lambda with generic parameters and init capture
    auto factory = [](auto initial_value) {
        return [value = initial_value](auto transformer) mutable {
            value = transformer(value);
            return value;
        };
    };
    
    auto int_processor = factory(10);
    auto string_processor = factory(std::string("Hello"));
    
    std::cout << "Generic lambda factory:\n";
    std::cout << "Int: " << int_processor([](auto x) { return x * 2; }) << "\n";
    std::cout << "String: " << string_processor([](auto s) { return s + " World!"; }) << "\n\n";
    
    // Recursive lambda with init capture
    auto fibonacci = [](int n) {
        auto fib_impl = [](auto& self, int n, int a = 0, int b = 1) -> int {
            return n == 0 ? a : self(self, n - 1, b, a + b);
        };
        return fib_impl(fib_impl, n);
    };
    
    std::cout << "Recursive lambda:\n";
    for (int i = 0; i < 10; ++i) {
        std::cout << "fib(" << i << ") = " << fibonacci(i) << "\n";
    }
    
    std::cout << "\n🎉 You've mastered advanced lambda features!\n";
    std::cout << "🏅 Reward: Lambda Mastery Badge\n";
    
    return 0;
}