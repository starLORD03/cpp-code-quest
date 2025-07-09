# C++ Code Quest - CMake Configuration
# Modern C++14/17 Learning Adventure Game

cmake_minimum_required(VERSION 3.10)

# Project configuration
project(CppCodeQuest
    VERSION 1.0.0
    DESCRIPTION "A storytelling-based game to learn modern C++14 and C++17 concepts"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable testing
enable_testing()

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wcast-align
        -Wformat=2
        -Wuninitialized
        -Wnull-dereference
        -Wdouble-promotion
        -Wimplicit-fallthrough
    )
    
    # Debug-specific flags
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0 -fsanitize=address -fsanitize=undefined)
        add_link_options(-fsanitize=address -fsanitize=undefined)
    endif()
    
    # Release-specific flags
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-O3 -DNDEBUG -flto)
        add_link_options(-flto)
    endif()
    
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4 /permissive-)
    
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(/Od /Zi)
    endif()
    
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(/O2 /DNDEBUG)
    endif()
endif()

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/game
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
)

# ==========================================
# Source Files Organization
# ==========================================

# Core game engine sources
set(GAME_SOURCES
    src/game/GameEngine.cpp
    src/game/Level.cpp
)

# Utility sources
set(UTILS_SOURCES
    src/utils/StringUtils.cpp
    src/utils/FileUtils.cpp
)

# Header files for IDEs
set(GAME_HEADERS
    src/game/GameEngine.hpp
    src/game/Level.hpp
)

set(UTILS_HEADERS
    src/utils/StringUtils.hpp
    src/utils/FileUtils.hpp
)

# ==========================================
# Main Game Executable
# ==========================================

add_executable(cpp-code-quest
    src/main.cpp
    ${GAME_SOURCES}
    ${UTILS_SOURCES}
)

# Set target properties
set_target_properties(cpp-code-quest PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    OUTPUT_NAME "cpp-code-quest"
)

# ==========================================
# Example Level Executables
# ==========================================

# Level 1: Auto Type Deduction
add_executable(level1_auto
    examples/level1_auto.cpp
)

# Level 2: Advanced Lambdas
add_executable(level2_lambdas
    examples/level2_lambdas.cpp
)

# Level 3: Smart Pointers
add_executable(level3_smart_pointers
    examples/level3_smart_pointers.cpp
)

# Level 4: Move Semantics
add_executable(level4_move_semantics
    examples/level4_move_semantics.cpp
)

# Level 5: Advanced C++17 Features
add_executable(level5_advanced
    examples/level5_advanced.cpp
)

# Set properties for all level executables
foreach(target level1_auto level2_lambdas level3_smart_pointers level4_move_semantics level5_advanced)
    set_target_properties(${target} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples
    )
endforeach()

# ==========================================
# Testing Configuration
# ==========================================

# Find or download GoogleTest
find_package(GTest QUIET)

# Test executable
add_executable(cpp-code-quest-tests
    tests/test_main.cpp
    ${GAME_SOURCES}
    ${UTILS_SOURCES}
)

target_link_libraries(cpp-code-quest-tests
    gtest_main
    gtest
    pthread
)

set_target_properties(cpp-code-quest-tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)

# Add test to CTest
add_test(NAME CppCodeQuestTests COMMAND cpp-code-quest-tests)

# ==========================================
# Custom Targets for Development
# ==========================================

# Run all examples
add_custom_target(run-examples
    COMMAND ${CMAKE_COMMAND} -E echo "🏛️ Running Level 1: Auto Type Deduction"
    COMMAND $<TARGET_FILE:level1_auto>
    COMMAND ${CMAKE_COMMAND} -E echo "🌟 Running Level 2: Advanced Lambdas"
    COMMAND $<TARGET_FILE:level2_lambdas>
    COMMAND ${CMAKE_COMMAND} -E echo "🔨 Running Level 3: Smart Pointers"
    COMMAND $<TARGET_FILE:level3_smart_pointers>
    COMMAND ${CMAKE_COMMAND} -E echo "🏔️ Running Level 4: Move Semantics"
    COMMAND $<TARGET_FILE:level4_move_semantics>
    COMMAND ${CMAKE_COMMAND} -E echo "🏰 Running Level 5: Advanced C++17"
    COMMAND $<TARGET_FILE:level5_advanced>
    DEPENDS level1_auto level2_lambdas level3_smart_pointers level4_move_semantics level5_advanced
    COMMENT "Running all C++ Code Quest examples"
)

# Run tests with detailed output
add_custom_target(run-tests
    COMMAND ${CMAKE_COMMAND} -E echo "🧪 Running C++ Code Quest Tests"
    COMMAND $<TARGET_FILE:cpp-code-quest-tests> --gtest_output=xml:${CMAKE_BINARY_DIR}/test_results.xml
    DEPENDS cpp-code-quest-tests
    COMMENT "Running all tests with XML output"
)

# Code coverage (GCC/Clang only)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_custom_target(coverage
        COMMAND ${CMAKE_COMMAND} -E echo "📊 Generating code coverage report"
        COMMAND gcov -r ${CMAKE_BINARY_DIR}/CMakeFiles/cpp-code-quest-tests.dir/src/game/*.gcno
        COMMAND gcov -r ${CMAKE_BINARY_DIR}/CMakeFiles/cpp-code-quest-tests.dir/src/utils/*.gcno
        DEPENDS cpp-code-quest-tests
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating code coverage report"
    )
endif()

# Static analysis with clang-tidy (if available)
find_program(CLANG_TIDY_EXECUTABLE NAMES clang-tidy)
if(CLANG_TIDY_EXECUTABLE)
    add_custom_target(static-analysis
        COMMAND ${CLANG_TIDY_EXECUTABLE} 
            ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/game/*.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/*.cpp
            --
            -std=c++17
            -I${CMAKE_CURRENT_SOURCE_DIR}/src
        COMMENT "Running static analysis with clang-tidy"
    )
endif()

# Format code with clang-format (if available)
find_program(CLANG_FORMAT_EXECUTABLE NAMES clang-format)
if(CLANG_FORMAT_EXECUTABLE)
    file(GLOB_RECURSE ALL_CXX_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/examples/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp
    )
    
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXECUTABLE} -i ${ALL_CXX_SOURCE_FILES}
        COMMENT "Formatting all C++ source files"
    )
endif()

# ==========================================
# Installation Configuration
# ==========================================

# Install main executable
install(TARGETS cpp-code-quest
    RUNTIME DESTINATION bin
)

# Install example executables
install(TARGETS level1_auto level2_lambdas level3_smart_pointers level4_move_semantics level5_advanced
    RUNTIME DESTINATION bin/examples
)

# Install documentation
install(FILES README.md LICENSE
    DESTINATION share/doc/cpp-code-quest
)

install(DIRECTORY docs/
    DESTINATION share/doc/cpp-code-quest/docs
)

# ==========================================
# Package Configuration
# ==========================================

# CPack configuration
set(CPACK_PACKAGE_NAME "CppCodeQuest")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_VENDOR "C++ Code Quest Team")
set(CPACK_PACKAGE_CONTACT "developer@cppquest.com")

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

# Platform-specific packaging
if(WIN32)
    set(CPACK_GENERATOR "ZIP;NSIS")
    set(CPACK_NSIS_DISPLAY_NAME "C++ Code Quest")
    set(CPACK_NSIS_HELP_LINK "https://github.com/starlod03/cpp-code-quest")
    set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/starlord03/cpp-code-quest")
    set(CPACK_NSIS_CONTACT "developer@cppquest.com")
    
elseif(APPLE)
    set(CPACK_GENERATOR "TGZ;DragNDrop")
    
else()
    set(CPACK_GENERATOR "TGZ;DEB;RPM")
    
    # Debian package configuration
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.17), libgcc1 (>= 1:3.0), libstdc++6 (>= 5.2)")
    set(CPACK_DEBIAN_PACKAGE_SECTION "Education")
    set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
    
    # RPM package configuration
    set(CPACK_RPM_PACKAGE_LICENSE "MIT")
    set(CPACK_RPM_PACKAGE_GROUP "Applications/Education")
    set(CPACK_RPM_PACKAGE_REQUIRES "glibc >= 2.17, libgcc >= 3.0, libstdc++ >= 5.2")
endif()

include(CPack)

# ==========================================
# Build Information
# ==========================================

# Print build configuration
message(STATUS "")
message(STATUS "🎮 C++ Code Quest - Build Configuration")
message(STATUS "=====================================")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Install Prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Testing Enabled: ${BUILD_TESTING}")

if(GTest_FOUND OR TARGET gtest)
    message(STATUS "Google Test: Found")
else()
    message(STATUS "Google Test: Not Found")
endif()

if(CLANG_TIDY_EXECUTABLE)
    message(STATUS "Static Analysis: clang-tidy available")
endif()

if(CLANG_FORMAT_EXECUTABLE)
    message(STATUS "Code Formatting: clang-format available")
endif()

message(STATUS "")
message(STATUS "Available targets:")
message(STATUS "  cpp-code-quest        - Main game executable")
message(STATUS "  level1_auto           - Level 1 example")
message(STATUS "  level2_lambdas        - Level 2 example")
message(STATUS "  level3_smart_pointers - Level 3 example")
message(STATUS "  level4_move_semantics - Level 4 example")
message(STATUS "  level5_advanced       - Level 5 example")
message(STATUS "  cpp-code-quest-tests  - Run all tests")
message(STATUS "  run-examples          - Run all examples")
message(STATUS "  run-tests             - Run tests with XML output")
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    message(STATUS "  coverage              - Generate coverage report")
endif()
if(CLANG_TIDY_EXECUTABLE)
    message(STATUS "  static-analysis       - Run clang-tidy")
endif()
if(CLANG_FORMAT_EXECUTABLE)
    message(STATUS "  format                - Format code with clang-format")
endif()
message(STATUS "")
message(STATUS "Build with: cmake --build . --target <target_name>")
message(STATUS "")