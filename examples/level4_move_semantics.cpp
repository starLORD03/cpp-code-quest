/**
 * C++ Code Quest - Level 4: The Valley of Move Semantics
 * 
 * This example demonstrates:
 * - Move semantics with std::move
 * - Perfect forwarding with std::forward
 * - Universal references (T&&)
 * - Rvalue references and move constructors
 * - When the compiler moves automatically
 * 
 * Learning Objectives:
 * - Understand the difference between lvalue and rvalue references
 * - Master perfect forwarding for performance optimization
 * - Implement move constructors and move assignment operators
 * - Use std::move and std::forward correctly
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <type_traits>

// ==========================================
// 1. Basic Move Semantics Example
// ==========================================

class Resource {
private:
    std::string name;
    std::vector<int> data;
    
public:
    // Constructor
    Resource(const std::string& n, size_t size) 
        : name(n), data(size, 42) {
        std::cout << "📦 Resource '" << name << "' created with " << size << " elements\n";
    }
    
    // Copy constructor
    Resource(const Resource& other) 
        : name(other.name + "_copy"), data(other.data) {
        std::cout << "📋 Resource '" << name << "' copied (expensive!)\n";
    }
    
    // Move constructor
    Resource(Resource&& other) noexcept 
        : name(std::move(other.name)), data(std::move(other.data)) {
        std::cout << "🚀 Resource '" << name << "' moved (efficient!)\n";
        other.name = "moved_from";
    }
    
    // Copy assignment
    Resource& operator=(const Resource& other) {
        if (this != &other) {
            name = other.name + "_assigned";
            data = other.data;
            std::cout << "📋 Resource copy assigned\n";
        }
        return *this;
    }
    
    // Move assignment
    Resource& operator=(Resource&& other) noexcept {
        if (this != &other) {
            name = std::move(other.name);
            data = std::move(other.data);
            std::cout << "🚀 Resource move assigned\n";
            other.name = "moved_from";
        }
        return *this;
    }
    
    // Destructor
    ~Resource() {
        std::cout << "💀 Resource '" << name << "' destroyed\n";
    }
    
    const std::string& getName() const { return name; }
    size_t getSize() const { return data.size(); }
};

// ==========================================
// 2. Perfect Forwarding Example
// ==========================================

class DataProcessor
{
public:
    // Perfect forwarding factory function
    template <typename T>
    static auto createResource(T &&name, size_t size)
    {
        std::cout << "🏭 Factory creating resource with forwarded name\n";
        return std::make_unique<Resource>(std::forward<T>(name), size);
    }

    // Perfect forwarding wrapper function
    template <typename T>
    static auto processAndReturn(T &&value)
    {
        std::cout << "⚙️ Processing value...\n";
        return std::forward<T>(value);
    }

    // Variadic template with perfect forwarding
    template <typename... Args>
    static auto createMultipleResources(Args &&...args)
    {
        std::vector<std::unique_ptr<Resource>> resources;

        // Using static_assert to ensure we have even number of arguments
        static_assert(sizeof...(Args) % 2 == 0, "Arguments must be in name-size pairs");

        // Helper function
        auto helper = [&resources](auto &&name, auto &&size)
        {
            resources.push_back(std::make_unique<Resource>(
                std::forward<decltype(name)>(name),
                std::forward<decltype(size)>(size)));
        };

        // Use tuple unpacking and index trick to call helper for each pair
        auto callHelpers = [&](auto &&...packedArgs)
        {
            auto tup = std::make_tuple(std::forward<decltype(packedArgs)>(packedArgs)...);

            constexpr size_t N = sizeof...(packedArgs) / 2;
            [&]<std::size_t... I>(std::index_sequence<I...>)
            {
                (helper(
                     std::get<2 * I>(tup),
                     std::get<2 * I + 1>(tup)),
                 ...);
            }(std::make_index_sequence<N>{});
        };

        callHelpers(std::forward<Args>(args)...);

        return resources;
    } 
};

// ==========================================
// 3. Universal Reference Examples
// ==========================================

// Function template with universal reference
template<typename T>
auto universalWrapper(T&& arg) {
    std::cout << "🔄 Universal wrapper called with: ";
    
    if constexpr (std::is_lvalue_reference_v<T>) {
        std::cout << "lvalue reference\n";
    } else {
        std::cout << "rvalue reference\n";
    }
    
    return std::forward<T>(arg);
}

// Overload resolution demonstration
void processValue(const std::string& value) {
    std::cout << "📝 Processing lvalue: " << value << "\n";
}

void processValue(std::string&& value) {
    std::cout << "🚀 Processing rvalue: " << value << "\n";
}

// ==========================================
// 4. Move Semantics Best Practices
// ==========================================

class OptimizedContainer {
private:
    std::vector<Resource> resources;
    
public:
    // Efficient insertion with perfect forwarding
    template<typename T>
    void emplaceResource(T&& name, size_t size) {
        resources.emplace_back(std::forward<T>(name), size);
        std::cout << "✨ Resource emplaced efficiently\n";
    }
    
    // Move-aware member function
    Resource extractResource() {
        if (!resources.empty()) {
            Resource result = std::move(resources.back());
            resources.pop_back();
            return result; // Automatic move on return
        }
        return Resource("empty", 0);
    }
    
    // Perfect forwarding with constraints
    template<typename T>
    void addResource(T&& resource) {
        static_assert(std::is_same_v<std::decay_t<T>, Resource>, 
                     "Only Resource objects can be added");
        
        resources.push_back(std::forward<T>(resource));
        std::cout << "📦 Resource added to container\n";
    }
    
    size_t size() const { return resources.size(); }
};

// ==========================================
// 5. Demonstration Functions
// ==========================================

void demonstrateBasicMoveSemantics() {
    std::cout << "\n=== 🏔️ Basic Move Semantics Demo ===\n";
    
    // Create original resource
    Resource original("Mystic_Sword", 1000);
    
    // Copy (expensive)
    Resource copied = original;
    
    // Move (efficient)
    Resource moved = std::move(original);
    
    std::cout << "Original name after move: " << original.getName() << "\n";
    std::cout << "Moved resource name: " << moved.getName() << "\n";
}

void demonstratePerfectForwarding() {
    std::cout << "\n=== ⚡ Perfect Forwarding Demo ===\n";
    
    // Test with lvalue
    std::string lvalue = "Lightning_Staff";
    auto resource1 = DataProcessor::createResource(lvalue, 500);
    
    // Test with rvalue
    auto resource2 = DataProcessor::createResource("Fire_Orb", 750);
    
    // Test universal wrapper
    std::string test = "Test";
    auto result1 = universalWrapper(test);          // lvalue
    auto result2 = universalWrapper(std::string("Temp")); // rvalue
    
    // Demonstrate overload resolution
    processValue(test);                    // lvalue version
    processValue(std::move(test));         // rvalue version
}

void demonstrateOptimizedContainer() {
    std::cout << "\n=== ✨ Optimized Container Demo ===\n";
    
    OptimizedContainer container;
    
    // Emplace with different argument types
    std::string name1 = "Ancient_Rune";
    container.emplaceResource(name1, 300);           // lvalue
    container.emplaceResource("Dragon_Scale", 400);  // rvalue
    
    // Add existing resource
    Resource existing("Magic_Crystal", 250);
    container.addResource(std::move(existing));
    
    std::cout << "Container size: " << container.size() << "\n";
    
    // Extract resource (demonstrates return value optimization)
    Resource extracted = container.extractResource();
    std::cout << "Extracted: " << extracted.getName() << "\n";
}

void demonstrateAdvancedForwarding() {
    std::cout << "\n=== 🚀 Advanced Forwarding Demo ===\n";
    
    // Variadic template with perfect forwarding
    auto resources = DataProcessor::createMultipleResources(
        "Sword", 100,
        "Shield", 150,
        "Armor", 200
    );
    
    std::cout << "Created " << resources.size() << " resources\n";
    
    // Lambda with move capture (C++14)
    auto processor = [resource = std::make_unique<Resource>("Lambda_Resource", 50)](auto multiplier) mutable {
        std::cout << "Lambda processing: " << resource->getName() << "\n";
        return std::move(resource); // Move out of lambda
    };
    
    auto processed = processor(2);
    std::cout << "Processed resource: " << processed->getName() << "\n";
}

// ==========================================
// 6. Performance Comparison
// ==========================================

void performanceComparison() {
    std::cout << "\n=== 📊 Performance Comparison ===\n";
    
    // Simulate expensive operations
    std::cout << "Creating large resources...\n";
    
    // Without move semantics (copy)
    {
        std::cout << "\n--- Without Move Semantics ---\n";
        Resource large("Large_Dataset", 10000);
        Resource copy1 = large;                    // Copy
        Resource copy2 = copy1;                    // Another copy
    }
    
    // With move semantics
    {
        std::cout << "\n--- With Move Semantics ---\n";
        Resource large("Large_Dataset", 10000);
        Resource moved1 = std::move(large);        // Move
        Resource moved2 = std::move(moved1);       // Another move
    }
}

// ==========================================
// 7. Main Function - The Valley Adventure
// ==========================================

int main() {
    std::cout << "🏔️ Welcome to the Valley of Move Semantics! ⚡\n";
    std::cout << "==============================================\n";
    std::cout << "Here you'll learn to harness the power of efficient resource management!\n";
    
    try {
        demonstrateBasicMoveSemantics();
        demonstratePerfectForwarding();
        demonstrateOptimizedContainer();
        demonstrateAdvancedForwarding();
        performanceComparison();
        
        std::cout << "\n🎉 Congratulations! You've mastered Move Semantics!\n";
        std::cout << "You've earned: 🚀 Move Semantics Mastery\n";
        std::cout << "\nKey Concepts Mastered:\n";
        std::cout << "✅ std::move for transferring resources\n";
        std::cout << "✅ std::forward for perfect forwarding\n";
        std::cout << "✅ Universal references (T&&)\n";
        std::cout << "✅ Move constructors and assignment operators\n";
        std::cout << "✅ Performance optimization techniques\n";
        
    } catch (const std::exception& e) {
        std::cerr << "❌ Error in the Valley: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

/* 
 * 🎓 Learning Notes:
 * 
 * 1. Move Semantics:
 *    - std::move casts to rvalue reference
 *    - Move constructors transfer resources instead of copying
 *    - Use noexcept for move operations when possible
 * 
 * 2. Perfect Forwarding:
 *    - std::forward preserves value category
 *    - Universal references (T&&) bind to both lvalue and rvalue
 *    - Template argument deduction rules determine reference type
 * 
 * 3. Performance Benefits:
 *    - Eliminates unnecessary copies
 *    - Especially important for expensive-to-copy objects
 *    - Automatic move in return statements (RVO/NRVO)
 * 
 * 4. Best Practices:
 *    - Use std::move for transferring ownership
 *    - Use std::forward in template functions
 *    - Prefer emplace operations over insert
 *    - Make move operations noexcept when possible
 * 
 * 5. Common Pitfalls:
 *    - Don't use std::move on return values (prevents RVO)
 *    - Don't move from const objects
 *    - Be careful with moved-from objects
 */