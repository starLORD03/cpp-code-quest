cmake_minimum_required(VERSION 3.10)

project(CppCodeQuest
    VERSION 1.0.0
    DESCRIPTION "A storytelling-based game to learn modern C++14 and C++17 concepts"
    LANGUAGES CXX
)

# Set C++ standard globally
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable compile commands export (good for IDEs like VSCode)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default build type to Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler-specific options and tweaks
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

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0 -fsanitize=address -fsanitize=undefined)
        add_link_options(-fsanitize=address -fsanitize=undefined)
    endif()

    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        # Disable LTO on MinGW because it often causes linker errors
        if(NOT MINGW)
            add_compile_options(-O3 -DNDEBUG -flto)
            add_link_options(-flto)
        else()
            add_compile_options(-O3 -DNDEBUG)
        endif()
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

# Detect MinGW for conditional logic
if(MINGW)
    message(STATUS "Configuring for MinGW toolchain")
endif()

# Include dirs
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/game
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
)

# Core source files
set(GAME_SOURCES
    src/game/GameEngine.cpp
    src/game/Level.cpp
)

set(UTILS_SOURCES
    src/utils/StringUtils.cpp
    src/utils/FileUtils.cpp
)

# Main executable
add_executable(cpp-code-quest
    src/main.cpp
    ${GAME_SOURCES}
    ${UTILS_SOURCES}
)

set_target_properties(cpp-code-quest PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    OUTPUT_NAME "cpp-code-quest"
)

# Examples
foreach(target level1_auto level2_lambdas level3_smart_pointers level4_move_semantics level5_advanced)
    add_executable(${target} examples/${target}.cpp)
    set_target_properties(${target} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples
    )
endforeach()

# Testing setup using FetchContent
include(FetchContent)

if(NOT TARGET gtest)
    message(STATUS "Google Test not found. Fetching...")
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )
    FetchContent_MakeAvailable(googletest)
endif()

enable_testing()

add_executable(cpp-code-quest-tests
    tests/test_main.cpp
    ${GAME_SOURCES}
    ${UTILS_SOURCES}
)

target_link_libraries(cpp-code-quest-tests
    gtest_main
    gtest
    # Use proper pthread on Windows MinGW
    $<$<AND:$<PLATFORM_ID:Windows>,$<CXX_COMPILER_ID:GNU>>:pthread>
)

set_target_properties(cpp-code-quest-tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)

add_test(NAME CppCodeQuestTests COMMAND cpp-code-quest-tests)

# Custom targets
add_custom_target(run-examples
    DEPENDS level1_auto level2_lambdas level3_smart_pointers level4_move_semantics level5_advanced
    COMMENT "Building all examples"
)

add_custom_target(run-tests
    DEPENDS cpp-code-quest-tests
    COMMAND cpp-code-quest-tests --gtest_output=xml:${CMAKE_BINARY_DIR}/test_results.xml
    COMMENT "Running tests"
)

# Optionally disable coverage and static analysis on Windows
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_custom_target(coverage
        COMMAND ${CMAKE_COMMAND} -E echo "Generating coverage report"
        DEPENDS cpp-code-quest-tests
    )
endif()

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
        COMMENT "Running static analysis"
    )
endif()

# clang-format target
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
        COMMENT "Formatting source files"
    )
endif()

# Install targets
install(TARGETS cpp-code-quest
    RUNTIME DESTINATION bin
)

install(TARGETS level1_auto level2_lambdas level3_smart_pointers level4_move_semantics level5_advanced
    RUNTIME DESTINATION bin/examples
)

install(FILES README.md LICENSE
    DESTINATION share/doc/cpp-code-quest
)

install(DIRECTORY docs/
    DESTINATION share/doc/cpp-code-quest/docs
)

# Packaging
set(CPACK_PACKAGE_NAME "CppCodeQuest")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_VENDOR "C++ Code Quest Team")
set(CPACK_PACKAGE_CONTACT "developer@cppquest.com")

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

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
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.17), libgcc1 (>= 1:3.0), libstdc++6 (>= 5.2)")
    set(CPACK_DEBIAN_PACKAGE_SECTION "Education")
    set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
    set(CPACK_RPM_PACKAGE_LICENSE "MIT")
    set(CPACK_RPM_PACKAGE_GROUP "Applications/Education")
    set(CPACK_RPM_PACKAGE_REQUIRES "glibc >= 2.17, libgcc >= 3.0, libstdc++ >= 5.2")
endif()

include(CPack)

# Print build info on configure
message(STATUS "")
message(STATUS "🎮 C++ Code Quest - Build Configuration")
message(STATUS "=====================================")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Install Prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Testing Enabled: ${BUILD_TESTING}")

if(TARGET gtest)
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
message(STATUS "  run-examples          - Build all examples")
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