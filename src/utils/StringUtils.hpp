#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

class StringUtils {
public:
    // String manipulation
    static std::string toLowerCase(const std::string& str);
    static std::string toUpperCase(const std::string& str);
    static std::string trim(const std::string& str);
    static std::string removeSpaces(const std::string& str);
    
    // String searching
    static bool contains(const std::string& str, const std::string& substr);
    static bool containsIgnoreCase(const std::string& str, const std::string& substr);
    static bool containsAll(const std::string& str, const std::vector<std::string>& substrings);
    static bool containsAny(const std::string& str, const std::vector<std::string>& substrings);
    
    // String splitting and joining
    static std::vector<std::string> split(const std::string& str, char delimiter);
    static std::vector<std::string> split(const std::string& str, const std::string& delimiter);
    static std::string join(const std::vector<std::string>& strings, const std::string& delimiter);
    
    // String replacement
    static std::string replace(const std::string& str, const std::string& from, const std::string& to);
    static std::string replaceAll(const std::string& str, const std::string& from, const std::string& to);
    
    // String validation
    static bool isNumeric(const std::string& str);
    static bool isAlpha(const std::string& str);
    static bool isAlphaNumeric(const std::string& str);
    static bool isEmpty(const std::string& str);
    static bool isWhitespace(const std::string& str);
    
    // C++ code specific utilities
    static bool isValidCppIdentifier(const std::string& str);
    static bool containsCppKeyword(const std::string& str, const std::string& keyword);
    static std::vector<std::string> extractCppKeywords(const std::string& code);
    static bool hasBalancedBraces(const std::string& code);
    static int countOccurrences(const std::string& str, const std::string& substr);
    
    // Formatting utilities
    static std::string padLeft(const std::string& str, size_t width, char fillChar = ' ');
    static std::string padRight(const std::string& str, size_t width, char fillChar = ' ');
    static std::string center(const std::string& str, size_t width, char fillChar = ' ');
    
    // Utility templates
    template<typename T>
    static std::string toString(const T& value) {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }
    
    template<typename T>
    static T fromString(const std::string& str) {
        std::istringstream iss(str);
        T value;
        iss >> value;
        return value;
    }
    
private:
    // Helper functions
    static bool isSpace(char c);
    static std::vector<std::string> getCppKeywords();
};