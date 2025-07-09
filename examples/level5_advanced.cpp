#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <optional>
#include <variant>
#include <type_traits>
#include <memory>
#include <algorithm>
#include <numeric>

// Level 5: The Citadel of Structured Bindings - Advanced C++17 Features
// This example demonstrates the final boss level concepts

namespace cpp17_features {

    // Structured Bindings with custom types
    struct Player {
        std::string name;
        int level;
        double experience;
        
        // C++17: Structured binding support
        template<std::size_t I>
        auto get() const {
            if constexpr (I == 0) return name;
            else if constexpr (I == 1) return level;
            else if constexpr (I == 2) return experience;
        }
    };

    // C++17: if constexpr for compile-time branching
    template<typename T>
    auto process_value(T&& value) {
        if constexpr (std::is_integral_v<std::decay_t<T>>) {
            std::cout << "Processing integer: " << value << std::endl;
            return value * 2;
        } else if constexpr (std::is_floating_point_v<std::decay_t<T>>) {
            std::cout << "Processing float: " << value << std::endl;
            return value * 1.5;
        } else if constexpr (std::is_same_v<std::decay_t<T>, std::string>) {
            std::cout << "Processing string: " << value << std::endl;
            return value + " (processed)";
        } else {
            std::cout << "Processing unknown type" << std::endl;
            return value;
        }
    }

    // C++17: Fold expressions for variadic templates
    template<typename... Args>
    auto sum_all(Args... args) {
        return (args + ...); // Unary right fold
    }

    template<typename... Args>
    void print_all(Args... args) {
        ((std::cout << args << " "), ...); // Fold expression with comma operator
        std::cout << std::endl;
    }

    // C++17: std::optional for safe value handling
    std::optional<int> safe_divide(int a, int b) {
        if (b == 0) {
            return std::nullopt; // No value
        }
        return a / b;
    }

    // C++17: std::variant for type-safe unions
    using GameValue = std::variant<int, double, std::string>;

    class GameInventory {
    private:
        std::vector<GameValue> items;
        
    public:
        void add_item(const GameValue& item) {
            items.push_back(item);
        }
        
        void display_inventory() const {
            std::cout << "=== Game Inventory ===" << std::endl;
            for (const auto& item : items) {
                std::visit([](const auto& value) {
                    std::cout << "Item: " << value << std::endl;
                }, item);
            }
        }
        
        // C++17: Structured bindings with containers
        auto get_stats() const {
            int count = items.size();
            int int_items = 0;
            int double_items = 0;
            int string_items = 0;
            
            for (const auto& item : items) {
                std::visit([&](const auto& value) {
                    using T = std::decay_t<decltype(value)>;
                    if constexpr (std::is_same_v<T, int>) {
                        int_items++;
                    } else if constexpr (std::is_same_v<T, double>) {
                        double_items++;
                    } else if constexpr (std::is_same_v<T, std::string>) {
                        string_items++;
                    }
                }, item);
            }
            
            return std::make_tuple(count, int_items, double_items, string_items);
        }
    };

    // C++17: Class template argument deduction
    template<typename T>
    class GameResource {
    private:
        T resource;
        std::string name;
        
    public:
        GameResource(T res, std::string n) : resource(res), name(std::move(n)) {}
        
        const T& get() const { return resource; }
        const std::string& get_name() const { return name; }
    };

    // C++17: Deduction guides (optional, but helpful)
    template<typename T>
    GameResource(T, std::string) -> GameResource<T>;

} // namespace cpp17_features

// Namespace alias for convenience
namespace features = cpp17_features;

int main() {
    std::cout << "🏰 Welcome to the Citadel of Structured Bindings! 🏰" << std::endl;
    std::cout << "=== Advanced C++17 Features Demo ===" << std::endl << std::endl;
    
    // 1. Structured Bindings with pairs and tuples
    std::cout << "📦 1. Structured Bindings:" << std::endl;
    auto player_data = std::make_tuple("Hero", 42, 99.5);
    auto [name, level, experience] = player_data;
    std::cout << "Player: " << name << ", Level: " << level << ", XP: " << experience << std::endl;
    
    // Structured bindings with maps
    std::vector<std::pair<std::string, int>> inventory = {
        {"Sword", 1}, {"Shield", 1}, {"Potion", 5}
    };
    
    for (const auto& [item, count] : inventory) {
        std::cout << "Item: " << item << ", Count: " << count << std::endl;
    }
    std::cout << std::endl;
    
    // 2. if constexpr demonstrations
    std::cout << "🧠 2. if constexpr - Compile-time Branching:" << std::endl;
    auto result1 = features::process_value(42);
    auto result2 = features::process_value(3.14);
    auto result3 = features::process_value(std::string("C++17"));
    std::cout << std::endl;
    
    // 3. Fold expressions
    std::cout << "📁 3. Fold Expressions:" << std::endl;
    auto sum = features::sum_all(1, 2, 3, 4, 5);
    std::cout << "Sum of 1,2,3,4,5: " << sum << std::endl;
    
    std::cout << "Printing multiple values: ";
    features::print_all("Hello", 42, 3.14, "World");
    std::cout << std::endl;
    
    // 4. std::optional usage
    std::cout << "🛡️ 4. std::optional for Safe Operations:" << std::endl;
    auto safe_result1 = features::safe_divide(10, 2);
    auto safe_result2 = features::safe_divide(10, 0);
    
    if (safe_result1) {
        std::cout << "10 / 2 = " << *safe_result1 << std::endl;
    }
    
    if (!safe_result2) {
        std::cout << "10 / 0 = Division by zero!" << std::endl;
    }
    std::cout << std::endl;
    
    // 5. std::variant demonstrations
    std::cout << "🎭 5. std::variant for Type-safe Unions:" << std::endl;
    features::GameInventory inventory_system;
    
    inventory_system.add_item(100);           // int
    inventory_system.add_item(99.9);          // double
    inventory_system.add_item(std::string("Magic Sword")); // string
    inventory_system.add_item(42);
    
    inventory_system.display_inventory();
    
    // Using structured bindings with custom function
    auto [total_items, int_count, double_count, string_count] = inventory_system.get_stats();
    std::cout << "Stats - Total: " << total_items 
              << ", Ints: " << int_count 
              << ", Doubles: " << double_count 
              << ", Strings: " << string_count << std::endl;
    std::cout << std::endl;
    
    // 6. Class template argument deduction
    std::cout << "🎯 6. Class Template Argument Deduction:" << std::endl;
    
    // C++17: No need to specify template arguments
    features::GameResource health_potion{100, "Health Potion"};
    features::GameResource mana_crystal{50.5, "Mana Crystal"};
    features::GameResource spell_book{std::string("Fireball"), "Spell Book"};
    
    std::cout << "Resource: " << health_potion.get_name() << " = " << health_potion.get() << std::endl;
    std::cout << "Resource: " << mana_crystal.get_name() << " = " << mana_crystal.get() << std::endl;
    std::cout << "Resource: " << spell_book.get_name() << " = " << spell_book.get() << std::endl;
    std::cout << std::endl;
    
    // 7. Advanced example: Combining all features
    std::cout << "🏆 7. Advanced Challenge - Combining All Features:" << std::endl;
    
    // Create a complex data structure
    auto create_player_summary = [](const std::string& name, int level, double xp) {
        return std::make_tuple(
            name,
            level,
            xp,
            features::safe_divide(static_cast<int>(xp), level),
            std::vector<features::GameValue>{level, xp, name + "'s Data"}
        );
    };
    
    auto player_summary = create_player_summary("C++ Master", 17, 2017.0);
    auto [p_name, p_level, p_xp, avg_xp_per_level, p_data] = player_summary;
    
    std::cout << "Player Summary:" << std::endl;
    std::cout << "  Name: " << p_name << std::endl;
    std::cout << "  Level: " << p_level << std::endl;
    std::cout << "  Experience: " << p_xp << std::endl;
    
    if (avg_xp_per_level) {
        std::cout << "  Avg XP/Level: " << *avg_xp_per_level << std::endl;
    }
    
    std::cout << "  Data items: ";
    for (const auto& data : p_data) {
        std::visit([](const auto& value) {
            std::cout << value << " ";
        }, data);
    }
    std::cout << std::endl;
    
    std::cout << std::endl << "🎉 Congratulations! You've mastered advanced C++17 features!" << std::endl;
    std::cout << "👑 You are now a C++17 Grandmaster! 👑" << std::endl;
    
    return 0;
}