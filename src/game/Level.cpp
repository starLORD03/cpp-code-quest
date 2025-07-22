#include "Level.hpp"
#include "../utils/StringUtils.hpp"
#include <iostream>
#include <thread>
#include <chrono>

Level::Level(const std::string& title,
             const std::string& story,
             const std::string& character,
             const std::string& dialogue,
             const std::string& conceptName,
             const std::string& conceptExplanation,
             const std::string& challenge,
             const std::string& reward,
             ValidationFunction validator)
    : title_(title), story_(story), character_(character), dialogue_(dialogue),
      concept_(conceptName), conceptExplanation_(conceptExplanation), challenge_(challenge),
      reward_(reward), validator_(validator), completed_(false) {}

void Level::play() {
    displayStory();
    displayConcept();
    showChallenge();
    
    int attempts = 0;
    const int maxAttempts = 3;
    
    while (attempts < maxAttempts && !completed_) {
        std::cout << "\n" << std::string(50, '=') << "\n";
        std::cout << "⚔️ Attempt " << (attempts + 1) << "/" << maxAttempts << "\n";
        std::cout << std::string(50, '=') << "\n";
        
        std::string userCode = getUserCode();
        
        if (validateSolution(userCode)) {
            showFeedback(true, "🎉 Excellent! You've mastered " + concept_ + "!");
            completed_ = true;
            return;
        } else {
            attempts++;
            if (attempts < maxAttempts) {
                showFeedback(false, "🔧 Not quite right. Try again!");
                
                std::cout << "\nWould you like:\n";
                std::cout << "1. Try again\n";
                std::cout << "2. Get a hint\n";
                std::cout << "3. See the solution\n";
                std::cout << "Choose (1-3): ";
                
                std::string choice;
                std::getline(std::cin, choice);
                
                if (choice == "2") {
                    showHint();
                } else if (choice == "3") {
                    showSolution();
                    completed_ = true;
                    return;
                }
            } else {
                showFeedback(false, "🤔 Don't worry! Let's see the solution.");
                showSolution();
                completed_ = true;
            }
        }
    }
}

void Level::displayStory() const {
    std::cout << "\n" << std::string(60, '═') << "\n";
    std::cout << "📖 " << title_ << "\n";
    std::cout << std::string(60, '═') << "\n";
    std::cout << story_ << "\n\n";
    std::cout << character_ << ": \"" << dialogue_ << "\"\n";
    std::cout << std::string(60, '═') << "\n";
    
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void Level::displayConcept() const {
    std::cout << "\n🧠 C++ Concept: " << concept_ << "\n";
    std::cout << std::string(50, '-') << "\n";
    std::cout << conceptExplanation_ << "\n";
}

void Level::showChallenge() const {
    std::cout << "\n⚔️ Your Challenge:\n";
    std::cout << std::string(30, '-') << "\n";
    std::cout << challenge_ << "\n";
}

void Level::showHint() const {
    std::cout << "\n💡 Hint: " << getHintText() << "\n";
}

void Level::showSolution() const {
    std::cout << "\n🔍 Solution:\n";
    std::cout << std::string(30, '-') << "\n";
    std::cout << getSolutionText() << "\n";
}

bool Level::validateSolution(const std::string& code) const {
    return validator_(code);
}

std::string Level::getUserCode() const {
    std::cout << "\n📝 Enter your C++ code (type 'DONE' on a new line when finished):\n";
    std::cout << std::string(50, '-') << "\n";
    
    std::string code;
    std::string line;
    
    while (std::getline(std::cin, line) && line != "DONE") {
        code += line + "\n";
    }
    
    return code;
}

void Level::showFeedback(bool success, const std::string& message) const {
    std::cout << "\n" << std::string(40, (success ? '✨' : '🔧')) << "\n";
    std::cout << message << "\n";
    std::cout << std::string(40, (success ? '✨' : '🔧')) << "\n";
}

std::string Level::getHintText() const {
    if (title_ == "The Temple of Auto") {
        return "Use 'auto' for variable declarations and create a lambda like: auto lambda = [](auto x) { return x * 2; };";
    } else if (title_ == "The Lambda Sanctuary") {
        return "Use generalized capture: [p = std::move(ptr)](auto x) { return *p * x; }";
    } else if (title_ == "The Smart Pointer Forge") {
        return "Use std::make_unique<int>(42) and std::make_shared<string>(\"Hello\")";
    } else if (title_ == "The Valley of Move Semantics") {
        return "Create a template function with T&& parameter and use std::forward<T>(arg)";
    } else if (title_ == "The Citadel of Structured Bindings") {
        return "Use auto [a, b] = std::make_pair(42, \"Hello\"); and if constexpr (condition)";
    }
    return "Think about the modern C++ features introduced in C++14/17!";
}

std::string Level::getSolutionText() const {
    if (title_ == "The Temple of Auto") {
        return R"(#include <iostream>
#include <string>

int main() {
    auto number = 42;
    auto text = "Hello C++14";
    auto lambda = [](auto x) { return x * 2; };
    auto result = lambda(5);
    
    std::cout << "Number: " << number << std::endl;
    std::cout << "Text: " << text << std::endl;
    std::cout << "Lambda result: " << result << std::endl;
    
    return 0;
})";
    } else if (title_ == "The Lambda Sanctuary") {
        return R"(#include <iostream>
#include <memory>

int main() {
    auto ptr = std::make_unique<int>(42);
    auto lambda = [p = std::move(ptr)](auto multiplier) {
        return *p * multiplier;
    };
    auto result = lambda(3);
    
    std::cout << "Result: " << result << std::endl;
    
    return 0;
})";
    } else if (title_ == "The Smart Pointer Forge") {
        return R"(#include <iostream>
#include <memory>
#include <string>

int main() {
    auto unique = std::make_unique<int>(42);
    auto shared1 = std::make_shared<std::string>("Hello");
    auto shared2 = shared1; // shared ownership
    
    std::cout << *unique << " " << *shared1 << std::endl;
    std::cout << "Shared count: " << shared1.use_count() << std::endl;
    
    return 0;
})";
    } else if (title_ == "The Valley of Move Semantics") {
        return R"(#include <iostream>
#include <utility>
#include <string>

template<typename T>
auto wrapper(T&& arg) {
    return std::forward<T>(arg);
}

int main() {
    auto result = wrapper(std::string("moved"));
    auto moved = std::move(result);
    
    std::cout << "Forwarded: " << moved << std::endl;
    
    return 0;
})";
    } else if (title_ == "The Citadel of Structured Bindings") {
        return R"(#include <iostream>
#include <utility>
#include <type_traits>

template<typename T>
auto process(T value) {
    if constexpr (std::is_integral_v<T>) {
        return value * 2;
    } else {
        return value;
    }
}

int main() {
    auto pair = std::make_pair(42, "Hello");
    auto [number, text] = pair;
    
    std::cout << "Number: " << number << std::endl;
    std::cout << "Text: " << text << std::endl;
    
    auto processed = process(number);
    std::cout << "Processed: " << processed << std::endl;
    
    return 0;
})";
    }
    return "// Solution not available for this level";
}