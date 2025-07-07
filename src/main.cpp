#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include "game/GameEngine.hpp"

int main() {
    try {
        std::cout << "🏰⚔️ Welcome to C++ Code Quest! 🏰⚔️\n";
        std::cout << "═══════════════════════════════════════\n";
        std::cout << "Learn modern C++14/17 through epic adventures!\n\n";
        
        auto game = std::make_unique<GameEngine>();
        game->run();
        
    } catch (const std::exception& e) {
        std::cerr << "❌ Game Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}