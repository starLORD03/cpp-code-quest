# 🧠 C++ Concepts Reference

Comprehensive reference for all C++ concepts covered in the game.

## C++14 Features

### Auto Type Deduction
```cpp
// Auto with variables
auto number = 42;           // int
auto text = "Hello";        // const char*
auto value = 3.14;          // double

// Auto with function return types
auto multiply(int a, int b) -> int {
    return a * b;
}

// Generic lambdas
auto lambda = [](auto x) {
    return x * 2;
};
```
### Generalized Lambda Capture
```cpp // Move capture
auto ptr = std::make_unique<int>(42);
auto lambda = [p = std::move(ptr)](int multiplier) {
    return *p * multiplier;
};

// Init capture
auto counter = [count = 0]() mutable { return ++count; };
make_unique
cpp// C++14 introduced make_unique
auto ptr = std::make_unique<int>(42);
auto arr = std::make_unique<int[]>(10);
```

### C++17 Features
### Structured Bindings
```cpp // Pair decomposition
auto pair = std::make_pair(42, "Hello");
auto [number, text] = pair;

// Tuple decomposition
auto tuple = std::make_tuple(1, 2.5, "test");
auto [a, b, c] = tuple;
if constexpr
cpptemplate<typename T>
auto process(T value) {
    if constexpr (std::is_integral_v<T>) {
        return value * 2;
    } else {
        return value;
    }
}
```

### Fold Expressions
```cpp
cpptemplate<typename... Args>
auto sum(Args... args) {
    return (args + ...);  // Fold expression
}
Move Semantics
Perfect Forwarding
cpptemplate<typename T>
auto wrapper(T&& arg) {
    return std::forward<T>(arg);
}
Move Constructor
cppclass MyClass {
public:
    MyClass(MyClass&& other) noexcept 
        : data(std::move(other.data)) {}
private:
    std::unique_ptr<int> data;
};
```
### Best Practices

Use auto judiciously: When the type is obvious or complex
Prefer make_unique/make_shared: Over raw new
Use structured bindings: For multiple return values
Leverage move semantics: For expensive-to-copy objects
Apply if constexpr: For compile-time conditionals

