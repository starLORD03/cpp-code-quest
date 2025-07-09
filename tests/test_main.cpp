/**
 * C++ Code Quest - Unit Tests
 * 
 * This file contains unit tests for the C++ Code Quest game engine
 * and utility functions using Google Test framework.
 */

#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <vector>
#include <sstream>

// Test basic C++14/17 concepts that are taught in the game
namespace CppCodeQuestTests {

// ==========================================
// Test C++14 Auto Type Deduction
// ==========================================

TEST(AutoTypeDeduction, BasicAutoUsage) {
    // Test basic auto type deduction
    auto number = 42;
    auto text = "Hello World";
    auto pi = 3.14159;
    
    EXPECT_EQ(typeid(number), typeid(int));
    EXPECT_EQ(typeid(text), typeid(const char*));
    EXPECT_EQ(typeid(pi), typeid(double));
    
    // Test with containers
    auto vec = std::vector<int>{1, 2, 3, 4, 5};
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[0], 1);
}

TEST(AutoTypeDeduction, AutoWithInitializerList) {
    // Test auto with initializer lists
    auto numbers = {1, 2, 3, 4, 5};
    EXPECT_EQ(numbers.size(), 5);
    
    auto begin_iter = numbers.begin();
    EXPECT_EQ(*begin_iter, 1);
}

// ==========================================
// Test C++14 Generic Lambdas
// ==========================================

TEST(GenericLambdas, BasicGenericLambda) {
    // Test generic lambda with auto parameter
    auto multiplier = [](auto x, auto y) {
        return x * y;
    };
    
    EXPECT_EQ(multiplier(5, 3), 15);
    EXPECT_DOUBLE_EQ(multiplier(2.5, 4.0), 10.0);
}

TEST(GenericLambdas, LambdaWithCapture) {
    int base = 10;
    
    // Test lambda with capture
    auto adder = [base](auto x) {
        return base + x;
    };
    
    EXPECT_EQ(adder(5), 15);
    EXPECT_EQ(adder(3.5), 13.5);
}

TEST(GenericLambdas, GeneralizedCapture) {
    // Test C++14 generalized capture (init capture)
    auto unique_ptr = std::make_unique<int>(42);
    
    auto lambda = [ptr = std::move(unique_ptr)](auto multiplier) {
        return *ptr * multiplier;
    };
    
    EXPECT_EQ(lambda(2), 84);
}

// ==========================================
// Test C++14 Smart Pointers
// ==========================================

TEST(SmartPointers, MakeUnique) {
    // Test std::make_unique
    auto ptr = std::make_unique<int>(42);
    EXPECT_NE(ptr, nullptr);
    EXPECT_EQ(*ptr, 42);
    
    // Test with custom type
    auto str_ptr = std::make_unique<std::string>("Hello");
    EXPECT_EQ(*str_ptr, "Hello");
}

TEST(SmartPointers, MakeShared) {
    // Test std::make_shared
    auto shared1 = std::make_shared<int>(100);
    auto shared2 = shared1;
    
    EXPECT_EQ(shared1.use_count(), 2);
    EXPECT_EQ(shared2.use_count(), 2);
    EXPECT_EQ(*shared1, 100);
    EXPECT_EQ(*shared2, 100);
}

TEST(SmartPointers, UniqueOwnership) {
    // Test unique_ptr ownership transfer
    auto ptr1 = std::make_unique<int>(50);
    auto ptr2 = std::move(ptr1);
    
    EXPECT_EQ(ptr1, nullptr);
    EXPECT_NE(ptr2, nullptr);
    EXPECT_EQ(*ptr2, 50);
}

// ==========================================
// Test C++14/17 Move Semantics
// ==========================================

class TestResource {
public:
    std::string data;
    bool moved_from = false;
    
    TestResource(const std::string& d) : data(d) {}
    
    // Copy constructor
    TestResource(const TestResource& other) : data(other.data) {}
    
    // Move constructor
    TestResource(TestResource&& other) noexcept 
        : data(std::move(other.data)) {
        other.moved_from = true;
    }
    
    // Move assignment
    TestResource& operator=(TestResource&& other) noexcept {
        if (this != &other) {
            data = std::move(other.data);
            other.moved_from = true;
        }
        return *this;
    }
};

TEST(MoveSemantics, BasicMove) {
    TestResource original("test_data");
    TestResource moved = std::move(original);
    
    EXPECT_EQ(moved.data, "test_data");
    EXPECT_TRUE(original.moved_from);
}

TEST(MoveSemantics, MoveAssignment) {
    TestResource resource1("data1");
    TestResource resource2("data2");
    
    resource2 = std::move(resource1);
    
    EXPECT_EQ(resource2.data, "data1");
    EXPECT_TRUE(resource1.moved_from);
}

// ==========================================
// Test C++17 Structured Bindings
// ==========================================

#if __cplusplus >= 201703L
TEST(StructuredBindings, PairDecomposition) {
    auto pair = std::make_pair(42, "hello");
    auto [number, text] = pair;
    
    EXPECT_EQ(number, 42);
    EXPECT_STREQ(text, "hello");
}

TEST(StructuredBindings, TupleDecomposition) {
    auto tuple = std::make_tuple(1, 2.5, "world");
    auto [a, b, c] = tuple;
    
    EXPECT_EQ(a, 1);
    EXPECT_DOUBLE_EQ(b, 2.5);
    EXPECT_STREQ(c, "world");
}
#endif

// ==========================================
// Test C++17 if constexpr
// ==========================================

#if __cplusplus >= 201703L
template<typename T>
std::string type_name() {
    if constexpr (std::is_integral_v<T>) {
        return "integer";
    } else if constexpr (std::is_floating_point_v<T>) {
        return "floating_point";
    } else {
        return "other";
    }
}

TEST(IfConstexpr, TypeTraits) {
    EXPECT_EQ(type_name<int>(), "integer");
    EXPECT_EQ(type_name<double>(), "floating_point");
    EXPECT_EQ(type_name<std::string>(), "other");
}
#endif

// ==========================================
// Test Perfect Forwarding
// ==========================================

template<typename T>
auto perfect_forward_wrapper(T&& arg) {
    return std::forward<T>(arg);
}

TEST(PerfectForwarding, LvalueReference) {
    std::string lvalue = "lvalue_test";
    auto result = perfect_forward_wrapper(lvalue);
    
    EXPECT_EQ(result, "lvalue_test");
    EXPECT_EQ(lvalue, "lvalue_test"); // Original should be unchanged
}

TEST(PerfectForwarding, RvalueReference) {
    auto result = perfect_forward_wrapper(std::string("rvalue_test"));
    EXPECT_EQ(result, "rvalue_test");
}

// ==========================================
// Test Variadic Templates
// ==========================================

template<typename... Args>
size_t count_args(Args&&... args) {
    return sizeof...(args);
}

TEST(VariadicTemplates, ArgumentCounting) {
    EXPECT_EQ(count_args(), 0);
    EXPECT_EQ(count_args(1), 1);
    EXPECT_EQ(count_args(1, 2.0, "three"), 3);
}

// ==========================================
// Test C++14 Variable Templates
// ==========================================

template<typename T>
constexpr T pi = T(3.14159265358979323846);

TEST(VariableTemplates, PiTemplate) {
    EXPECT_FLOAT_EQ(pi<float>, 3.14159f);
    EXPECT_DOUBLE_EQ(pi<double>, 3.14159265358979323846);
}

// ==========================================
// Integration Tests
// ==========================================

TEST(Integration, ModernCppFeatures) {
    // Test combination of modern C++ features
    
    // Auto with smart pointers
    auto resources = std::vector<std::unique_ptr<TestResource>>{};
    resources.push_back(std::make_unique<TestResource>("resource1"));
    resources.push_back(std::make_unique<TestResource>("resource2"));
    
    EXPECT_EQ(resources.size(), 2);
    EXPECT_EQ(resources[0]->data, "resource1");
    EXPECT_EQ(resources[1]->data, "resource2");
    
    // Generic lambda with capture
    auto processor = [&resources](auto index) {
        if (index < resources.size()) {
            return resources[index]->data;
        }
        return std::string("invalid");
    };
    
    EXPECT_EQ(processor(0), "resource1");
    EXPECT_EQ(processor(1), "resource2");
    EXPECT_EQ(processor(2), "invalid");
}

TEST(Integration, PerformanceOptimization) {
    // Test that move semantics provide performance benefits
    std::vector<TestResource> resources;
    resources.reserve(100);
    
    // Use emplace_back for in-place construction
    for (int i = 0; i < 100; ++i) {
        resources.emplace_back("resource_" + std::to_string(i));
    }
    
    EXPECT_EQ(resources.size(), 100);
    EXPECT_EQ(resources[0].data, "resource_0");
    EXPECT_EQ(resources[99].data, "resource_99");
    
    // Test move from vector
    auto moved_resource = std::move(resources[0]);
    EXPECT_TRUE(resources[0].moved_from);
    EXPECT_EQ(moved_resource.data, "resource_0");
}

} // namespace CppCodeQuestTests

// ==========================================
// Main Test Function
// ==========================================

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    
    std::cout << "🧪 Running C++ Code Quest Tests\n";
    std::cout << "================================\n";
    std::cout << "Testing modern C++14/17 concepts learned in the game...\n\n";
    
    int result = RUN_ALL_TESTS();
    
    if (result == 0) {
        std::cout << "\n🎉 All tests passed! You've mastered C++ Code Quest concepts!\n";
    } else {
        std::cout << "\n❌ Some tests failed. Review the concepts and try again.\n";
    }
    
    return result;
}