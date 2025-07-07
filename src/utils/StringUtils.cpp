#include "StringUtils.hpp"
#include <algorithm>
#include <regex>
#include <set>

// String manipulation implementations
std::string StringUtils::toLowerCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string StringUtils::toUpperCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::string StringUtils::trim(const std::string& str) {
    auto start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) {
        return "";
    }
    auto end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}

std::string StringUtils::removeSpaces(const std::string& str) {
    std::string result = str;
    result.erase(std::remove_if(result.begin(), result.end(), ::isspace), result.end());
    return result;
}

// String searching implementations
bool StringUtils::contains(const std::string& str, const std::string& substr) {
    return str.find(substr) != std::string::npos;
}

bool StringUtils::containsIgnoreCase(const std::string& str, const std::string& substr) {
    return toLowerCase(str).find(toLowerCase(substr)) != std::string::npos;
}

bool StringUtils::containsAll(const std::string& str, const std::vector<std::string>& substrings) {
    for (const auto& substr : substrings) {
        if (!contains(str, substr)) {
            return false;
        }
    }
    return true;
}

bool StringUtils::containsAny(const std::string& str, const std::vector<std::string>& substrings) {
    for (const auto& substr : substrings) {
        if (contains(str, substr)) {
            return true;
        }
    }
    return false;
}

// String splitting and joining implementations
std::vector<std::string> StringUtils::split(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::istringstream iss(str);
    std::string token;
    
    while (std::getline(iss, token, delimiter)) {
        result.push_back(token);
    }
    
    return result;
}

std::vector<std::string> StringUtils::split(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = str.find(delimiter);
    
    while (end != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }
    
    result.push_back(str.substr(start));
    return result;
}

std::string StringUtils::join(const std::vector<std::string>& strings, const std::string& delimiter) {
    if (strings.empty()) {
        return "";
    }
    
    std::string result = strings[0];
    for (size_t i = 1; i < strings.size(); ++i) {
        result += delimiter + strings[i];
    }
    
    return result;
}

// String replacement implementations
std::string StringUtils::replace(const std::string& str, const std::string& from, const std::string& to) {
    if (from.empty()) {
        return str;
    }
    
    std::string result = str;
    size_t pos = result.find(from);
    if (pos != std::string::npos) {
        result.replace(pos, from.length(), to);
    }
    
    return result;
}

std::string StringUtils::replaceAll(const std::string& str, const std::string& from, const std::string& to) {
    if (from.empty()) {
        return str;
    }
    
    std::string result = str;
    size_t pos = 0;
    
    while ((pos = result.find(from, pos)) != std::string::npos) {
        result.replace(pos, from.length(), to);
        pos += to.length();
    }
    
    return result;
}

// String validation implementations
bool StringUtils::isNumeric(const std::string& str) {
    if (str.empty()) {
        return false;
    }
    
    return std::all_of(str.begin(), str.end(), [](char c) {
        return std::isdigit(c) || c == '.' || c == '-' || c == '+';
    });
}

bool StringUtils::isAlpha(const std::string& str) {
    if (str.empty()) {
        return false;
    }
    
    return std::all_of(str.begin(), str.end(), [](char c) {
        return std::isalpha(c);
    });
}

bool StringUtils::isAlphaNumeric(const std::string& str) {
    if (str.empty()) {
        return false;
    }
    
    return std::all_of(str.begin(), str.end(), [](char c) {
        return std::isalnum(c);
    });
}

bool StringUtils::isEmpty(const std::string& str) {
    return str.empty();
}

bool StringUtils::isWhitespace(const std::string& str) {
    if (str.empty()) {
        return true;
    }
    
    return std::all_of(str.begin(), str.end(), [](char c) {
        return std::isspace(c);
    });
}

// C++ code specific utility implementations
bool StringUtils::isValidCppIdentifier(const std::string& str) {
    if (str.empty()) {
        return false;
    }
    
    // Must start with letter or underscore
    if (!std::isalpha(str[0]) && str[0] != '_') {
        return false;
    }
    
    // Rest must be alphanumeric or underscore
    for (size_t i = 1; i < str.length(); ++i) {
        if (!std::isalnum(str[i]) && str[i] != '_') {
            return false;
        }
    }
    
    // Check if it's a C++ keyword
    const auto keywords = getCppKeywords();
    return std::find(keywords.begin(), keywords.end(), str) == keywords.end();
}

bool StringUtils::containsCppKeyword(const std::string& str, const std::string& keyword) {
    // Use word boundaries to avoid partial matches
    std::regex wordRegex("\\b" + keyword + "\\b");
    return std::regex_search(str, wordRegex);
}

std::vector<std::string> StringUtils::extractCppKeywords(const std::string& code) {
    std::vector<std::string> foundKeywords;
    const auto keywords = getCppKeywords();
    
    for (const auto& keyword : keywords) {
        if (containsCppKeyword(code, keyword)) {
            foundKeywords.push_back(keyword);
        }
    }
    
    return foundKeywords;
}

bool StringUtils::hasBalancedBraces(const std::string& code) {
    int braceCount = 0;
    int parenCount = 0;
    int bracketCount = 0;
    
    for (char c : code) {
        switch (c) {
            case '{': braceCount++; break;
            case '}': braceCount--; break;
            case '(': parenCount++; break;
            case ')': parenCount--; break;
            case '[': bracketCount++; break;
            case ']': bracketCount--; break;
        }
        
        // Early exit if any count goes negative
        if (braceCount < 0 || parenCount < 0 || bracketCount < 0) {
            return false;
        }
    }
    
    return braceCount == 0 && parenCount == 0 && bracketCount == 0;
}

int StringUtils::countOccurrences(const std::string& str, const std::string& substr) {
    if (substr.empty()) {
        return 0;
    }
    
    int count = 0;
    size_t pos = 0;
    
    while ((pos = str.find(substr, pos)) != std::string::npos) {
        count++;
        pos += substr.length();
    }
    
    return count;
}

// Formatting utility implementations
std::string StringUtils::padLeft(const std::string& str, size_t width, char fillChar) {
    if (str.length() >= width) {
        return str;
    }
    
    return std::string(width - str.length(), fillChar) + str;
}

std::string StringUtils::padRight(const std::string& str, size_t width, char fillChar) {
    if (str.length() >= width) {
        return str;
    }
    
    return str + std::string(width - str.length(), fillChar);
}

std::string StringUtils::center(const std::string& str, size_t width, char fillChar) {
    if (str.length() >= width) {
        return str;
    }
    
    size_t padding = width - str.length();
    size_t leftPadding = padding / 2;
    size_t rightPadding = padding - leftPadding;
    
    return std::string(leftPadding, fillChar) + str + std::string(rightPadding, fillChar);
}

// Helper function implementations
bool StringUtils::isSpace(char c) {
    return std::isspace(static_cast<unsigned char>(c));
}

std::vector<std::string> StringUtils::getCppKeywords() {
    return {
        // C++ keywords
        "alignas", "alignof", "and", "and_eq", "asm", "auto", "bitand", "bitor",
        "bool", "break", "case", "catch", "char", "char8_t", "char16_t", "char32_t",
        "class", "compl", "concept", "const", "consteval", "constexpr", "constinit",
        "const_cast", "continue", "co_await", "co_return", "co_yield", "decltype",
        "default", "delete", "do", "double", "dynamic_cast", "else", "enum",
        "explicit", "export", "extern", "false", "float", "for", "friend", "goto",
        "if", "inline", "int", "long", "mutable", "namespace", "new", "noexcept",
        "not", "not_eq", "nullptr", "operator", "or", "or_eq", "private", "protected",
        "public", "register", "reinterpret_cast", "requires", "return", "short",
        "signed", "sizeof", "static", "static_assert", "static_cast", "struct",
        "switch", "template", "this", "thread_local", "throw", "true", "try",
        "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual",
        "void", "volatile", "wchar_t", "while", "xor", "xor_eq",
        
        // Common C++ library identifiers
        "std", "string", "vector", "map", "set", "list", "queue", "stack",
        "unique_ptr", "shared_ptr", "weak_ptr", "make_unique", "make_shared",
        "move", "forward", "pair", "tuple", "optional", "variant", "any",
        "function", "lambda", "bind", "ref", "cref", "iterator", "const_iterator",
        "begin", "end", "size", "empty", "push_back", "pop_back", "insert",
        "erase", "find", "count", "sort", "reverse", "transform", "for_each",
        "algorithm", "numeric", "functional", "memory", "utility", "type_traits",
        "chrono", "thread", "mutex", "lock_guard", "unique_lock", "condition_variable",
        "future", "promise", "async", "packaged_task", "exception", "runtime_error",
        "logic_error", "invalid_argument", "out_of_range", "length_error",
        "domain_error", "range_error", "overflow_error", "underflow_error"
    };
}