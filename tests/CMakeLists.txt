# C++ Code Quest - Tests CMakeLists.txt
# This file is included by the main CMakeLists.txt

# Test configuration is handled in the main CMakeLists.txt
# This file exists for potential future test-specific configuration

# Additional test-specific settings can be added here
# For example:
# - Test data files
# - Test-specific compile definitions
# - Test utilities

# Test data directory (if needed in the future)
set(TEST_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)

# Test utilities (if needed in the future)
# add_library(test_utils STATIC
#     utils/test_helpers.cpp
# )

# Individual test executables (if you want to split tests)
# add_executable(concept_tests
#     concept_tests.cpp
# )

# target_link_libraries(concept_tests
#     gtest_main
#     gtest
# )

# add_test(NAME ConceptTests COMMAND concept_tests)

message(STATUS "Test configuration loaded")