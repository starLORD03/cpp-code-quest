#include "GameEngine.hpp"
#include "../utils/StringUtils.hpp"
#include "../utils/FileUtils.hpp"
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

GameEngine::GameEngine() : currentLevel_(0) {
    initializeLevels();
}

void GameEngine::run() {
    showWelcome();
    
    while (!isGameComplete()) {
        playLevel(currentLevel_);
        
        if (currentLevel_ < levels_.size()) {
            std::cout << "\n🎯 Progress: " << getProgressPercentage() << "%\n";
            showInventory();
            
            if (askYesNo("Continue to next level?")) {
                currentLevel_++;
                clearScreen();
            } else {
                std::cout << "💾 Game saved! Thanks for playing!\n";
                return;
            }
        }
    }
    
    showVictory();
}

void GameEngine::initializeLevels() {
    // Level 1: Temple of Auto
    levels_.push_back(std::make_unique<Level>(
        "The Temple of Auto",
        "🏛️ You enter an ancient temple where the Oracle of Types dwells. "
        "The walls are covered with cryptic C++ symbols, and the air shimmers with template magic.",
        "🔮 Oracle of Types",
        "Welcome, young programmer! The age of verbose type declarations is ending. "
        "I shall teach you the power of 'auto' - let the compiler deduce types for you!",
        "Auto Type Deduction (C++14)",
        "The 'auto' keyword lets the compiler automatically deduce variable types. "
        "C++14 extended this to function return types and lambda parameters.",
        "Create variables using auto and show a generic lambda with auto parameters.",
        "📜 Auto Deduction Scroll",
        [](const std::string& code) {
            return StringUtils::containsAll(code, {"auto", "lambda", "[]"}) ||
                   StringUtils::containsAll(code, {"auto", "[", "auto"});
        }
    ));
    
    // Level 2: Lambda Sanctuary
    levels_.push_back(std::make_unique<Level>(
        "The Lambda Sanctuary",
        "🌟 Deep in the Lambda Sanctuary, you find a mysterious altar surrounded by floating code fragments. "
        "The Guardian of Closures materializes before you.",
        "👻 Guardian of Closures",
        "Ah, a seeker of functional wisdom! Lambdas are the soul of modern C++. "
        "Show me you understand capture by value, reference, and generalized capture!",
        "Advanced Lambdas (C++14)",
        "C++14 introduced generalized capture (init capture) allowing you to move variables into lambdas.",
        "Create a lambda with generalized capture that moves a unique_ptr.",
        "🏅 Lambda Mastery Badge",
        [](const std::string& code) {
            return StringUtils::containsAll(code, {"auto", "std::move", "unique_ptr"}) ||
                   StringUtils::contains(code, "= std::move");
        }
    ));
    
    // Level 3: Smart Pointer Forge
    levels_.push_back(std::make_unique<Level>(
        "The Smart Pointer Forge",
        "🔨 You arrive at an ancient forge where Smart Pointers are crafted. "
        "The Master Smith challenges you to prove your worth.",
        "🧙‍♂️ Master Smith",
        "Raw pointers are the bane of C++! Here we craft smart pointers that manage memory automatically. "
        "Show me you can wield unique_ptr, shared_ptr, and make_unique!",
        "Smart Pointers & make_unique (C++14)",
        "C++14 introduced std::make_unique. Smart pointers automatically manage memory.",
        "Create and use smart pointers with make_unique and make_shared.",
        "🛡️ Memory Guardian Shield",
        [](const std::string& code) {
            return StringUtils::containsAll(code, {"make_unique", "make_shared"}) ||
                   StringUtils::contains(code, "std::make_unique");
        }
    ));
    
    // Level 4: Valley of Move Semantics
    levels_.push_back(std::make_unique<Level>(
        "The Valley of Move Semantics",
        "🏔️ In the Valley of Move Semantics, you encounter the Spirit of Efficiency. "
        "Ancient runes speak of perfect forwarding and std::forward.",
        "⚡ Spirit of Efficiency",
        "Performance is everything! Learn to move resources instead of copying them. "
        "Master std::move, std::forward, and perfect forwarding!",
        "Move Semantics & Perfect Forwarding (C++14/17)",
        "Move semantics transfer resources instead of copying. Perfect forwarding preserves value categories.",
        "Implement a function template with perfect forwarding using std::forward.",
        "🚀 Move Semantics Mastery",
        [](const std::string& code) {
            return StringUtils::containsAll(code, {"std::forward", "&&"}) ||
                   StringUtils::containsAll(code, {"forward", "template"});
        }
    ));
    
    // Level 5: Citadel of Structured Bindings
    levels_.push_back(std::make_unique<Level>(
        "The Citadel of Structured Bindings",
        "🏰 At the peak of your journey, you reach the Citadel of Structured Bindings. "
        "The C++17 Archmaster awaits with the most modern features.",
        "👑 C++17 Archmaster",
        "Welcome to the pinnacle of modern C++! Here we unpack tuples, decompose pairs, "
        "and use structured bindings with elegant syntax!",
        "Modern C++17 Features",
        "C++17 introduced structured bindings, if constexpr, and fold expressions.",
        "Use structured bindings to unpack a pair and if constexpr for compile-time conditionals.",
        "👑 C++17 Grandmaster Crown",
        [](const std::string& code) {
            return StringUtils::containsAll(code, {"auto [", "] ="}) ||
                   StringUtils::contains(code, "if constexpr");
        }
    ));
}

void GameEngine::playLevel(size_t levelIndex) {
    if (levelIndex >= levels_.size()) {
        return;
    }
    
    auto& level = levels_[levelIndex];
    level->play();
    
    if (level->isCompleted()) {
        addToInventory(level->getReward());
        std::cout << "\n🎉 Level completed! You earned: " << level->getReward() << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    }
}

bool GameEngine::isGameComplete() const {
    return currentLevel_ >= levels_.size();
}

void GameEngine::addToInventory(const std::string& item) {
    inventory_.push_back(item);
}

void GameEngine::showInventory() const {
    if (inventory_.empty()) {
        return;
    }
    
    std::cout << "\n🛠️ Your C++ Arsenal:\n";
    for (const auto& item : inventory_) {
        std::cout << "  ✨ " << item << "\n";
    }
}

double GameEngine::getProgressPercentage() const {
    if (levels_.empty()) return 0.0;
    return (static_cast<double>(currentLevel_) / levels_.size()) * 100.0;
}

void GameEngine::showWelcome() const {
    std::cout << R"(
    🏰⚔️ C++ CODE QUEST ⚔️🏰
    ═══════════════════════════
    
    Welcome, brave programmer!
    
    Embark on an epic journey through the realms of modern C++!
    Master the ancient arts of:
    
    🏛️  Auto Type Deduction
    🌟  Advanced Lambdas  
    🔨  Smart Pointers
    🏔️  Move Semantics
    🏰  Structured Bindings
    
    Your quest begins now...
    
)";
    waitForInput();
}

void GameEngine::showVictory() const {
    std::cout << R"(
    🏆 CONGRATULATIONS! 🏆
    ═══════════════════════
    
    You have become a C++ GRANDMASTER!
    
    Your Arsenal:
)";
    
    for (const auto& item : inventory_) {
        std::cout << "    ✨ " << item << "\n";
    }
    
    std::cout << R"(
    You've mastered the advanced concepts of modern C++!
    Now go forth and build amazing applications!
    
    🚀 The adventure continues in your own projects! 🚀
    
)";
}

void GameEngine::clearScreen() const {
    // Simple screen clearing (works on most terminals)
    std::cout << "\033[2J\033[1;1H";
}

void GameEngine::waitForInput() const {
    std::cout << "Press Enter to continue...";
    std::cin.ignore();
}

bool GameEngine::askYesNo(const std::string& question) const {
    std::string response;
    std::cout << question << " (y/n): ";
    std::getline(std::cin, response);
    return !response.empty() && (response[0] == 'y' || response[0] == 'Y');
}

std::string GameEngine::getUserInput(const std::string& prompt) const {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}