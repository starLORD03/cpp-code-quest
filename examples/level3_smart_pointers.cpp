/**
 * Level 3: The Smart Pointer Forge
 * 
 * This example demonstrates smart pointers and std::make_unique
 * introduced in C++14, along with best practices for memory management.
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Example class for demonstration
class Weapon {
public:
    Weapon(const std::string& name, int damage) 
        : name_(name), damage_(damage) {
        std::cout << "⚔️ Forged weapon: " << name_ << " (damage: " << damage_ << ")\n";
    }
    
    ~Weapon() {
        std::cout << "🔥 Weapon " << name_ << " returns to the forge\n";
    }
    
    void use() const {
        std::cout << "💥 Using " << name_ << " for " << damage_ << " damage!\n";
    }
    
    const std::string& getName() const { return name_; }
    int getDamage() const { return damage_; }
    
private:
    std::string name_;
    int damage_;
};

class Warrior {
public:
    Warrior(const std::string& name) : name_(name) {
        std::cout << "🛡️ Warrior " << name_ << " enters the battlefield\n";
    }
    
    ~Warrior() {
        std::cout << "⚰️ Warrior " << name_ << " retires\n";
    }
    
    void equip(std::unique_ptr<Weapon> weapon) {
        weapon_ = std::move(weapon);
        std::cout << "🎯 " << name_ << " equipped " << weapon_->getName() << "\n";
    }
    
    void attack() const {
        if (weapon_) {
            weapon_->use();
        } else {
            std::cout << "👊 " << name_ << " attacks with bare hands!\n";
        }
    }
    
private:
    std::string name_;
    std::unique_ptr<Weapon> weapon_;
};

int main() {
    std::cout << "🔨 Welcome to the Smart Pointer Forge!\n";
    std::cout << "=====================================\n\n";
    
    // std::make_unique (C++14) - preferred over new
    std::cout << "=== Forging Unique Weapons ===\n";
    auto sword = std::make_unique<Weapon>("Excalibur", 50);
    auto bow = std::make_unique<Weapon>("Elven Bow", 35);
    
    // Unique ownership demonstration
    std::cout << "\n=== Unique Ownership ===\n";
    auto warrior = std::make_unique<Warrior>("Sir Lancelot");
    
    // Transfer ownership with std::move
    warrior->equip(std::move(sword));
    warrior->attack();
    
    // sword is now nullptr after move
    if (!sword) {
        std::cout << "✅ Sword ownership successfully transferred\n";
    }
    
    // Shared ownership with std::make_shared
    std::cout << "\n=== Shared Ownership ===\n";
    auto shared_weapon = std::make_shared<Weapon>("Mjolnir", 100);
    
    std::cout << "Reference count: " << shared_weapon.use_count() << "\n";
    
    {
        auto shared_copy = shared_weapon;
        std::cout << "Reference count after copy: " << shared_weapon.use_count() << "\n";
        shared_copy->use();
    } // shared_copy destroyed here
    
    std::cout << "Reference count after scope: " << shared_weapon.use_count() << "\n";
    
    // Array smart pointers
    std::cout << "\n=== Smart Pointer Arrays ===\n";
    //auto weapon_array = std::make_unique<Weapon[]>(3); // Removed: Weapon has no default constructor
    // Note: Array version doesn't call constructor with parameters
    // Better to use std::vector with smart pointers
    std::vector<std::unique_ptr<Weapon>> weapon_collection;
    weapon_collection.push_back(std::make_unique<Weapon>("Sword", 30));
    weapon_collection.push_back(std::make_unique<Weapon>("Axe", 40));
    weapon_collection.push_back(std::make_unique<Weapon>("Spear", 25));
    
    std::cout << "Weapon collection:\n";
    for (const auto& weapon : weapon_collection) {
        weapon->use();
    }
    
    // Custom deleter example
    std::cout << "\n=== Custom Deleter ===\n";
    auto custom_weapon = std::unique_ptr<Weapon, void(*)(Weapon*)>(
        new Weapon("Legendary Blade", 999),
        [](Weapon* w) {
            std::cout << "🌟 Custom deleter: Returning legendary weapon to the gods\n";
            delete w;
        }
    );
    
    custom_weapon->use();
    
    // Weak pointer example
    std::cout << "\n=== Weak Pointers ===\n";
    std::weak_ptr<Weapon> weak_weapon;
    
    {
        auto temp_weapon = std::make_shared<Weapon>("Temporary Sword", 20);
        weak_weapon = temp_weapon;
        
        if (auto locked = weak_weapon.lock()) {
            std::cout << "Weak pointer is valid: " << locked->getName() << "\n";
        }
    } // temp_weapon destroyed here
    
    if (weak_weapon.expired()) {
        std::cout << "✅ Weak pointer correctly detected object destruction\n";
    }
    
    // Factory function returning smart pointer
    auto create_weapon = [](const std::string& name, int damage) {
        return std::make_unique<Weapon>(name, damage);
    };
    
    std::cout << "\n=== Factory Pattern ===\n";
    auto factory_weapon = create_weapon("Factory Sword", 45);
    factory_weapon->use();
    
    std::cout << "\n🎉 You've mastered smart pointer memory management!\n";
    std::cout << "🛡️ Reward: Memory Guardian Shield\n";
    
    return 0;
    
    // All smart pointers automatically cleaned up here
    // No memory leaks, no explicit delete calls needed!
}