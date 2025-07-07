#pragma once

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include "Level.hpp"

class GameEngine {
public:
    GameEngine();
    ~GameEngine() = default;
    
    // Core game loop
    void run();
    
    // Level management
    void initializeLevels();
    void playLevel(size_t levelIndex);
    bool isGameComplete() const;
    
    // Player progress
    void addToInventory(const std::string& item);
    void showInventory() const;
    double getProgressPercentage() const;
    
    // Utility functions
    void showWelcome() const;
    void showVictory() const;
    void clearScreen() const;
    
private:
    std::vector<std::unique_ptr<Level>> levels_;
    std::vector<std::string> inventory_;
    size_t currentLevel_;
    
    // Helper methods
    void waitForInput() const;
    bool askYesNo(const std::string& question) const;
    std::string getUserInput(const std::string& prompt) const;
};