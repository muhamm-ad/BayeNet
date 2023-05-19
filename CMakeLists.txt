cmake_minimum_required(VERSION 3.5)
project(BayeNet)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# List of source files
set(SOURCE_FILES
    src/BayesianNetwork.cpp
    src/Variable.cpp
)

# List of BayesianNetwork test source files
set(TEST_BAYENET_FILES
    test/test_BayesianNetwork.cpp
)

# List of Variable test source files
set(TEST_VARIABLE_FILES
    test/test_Variable.cpp
)

# Add the library
add_library(${PROJECT_NAME} ${SOURCE_FILES})

# Add include directories
target_include_directories(${PROJECT_NAME} 
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/lib
)

# Locate GTest package
find_package(GTest REQUIRED)

# Add the BayesianNetwork test executable
add_executable(tests_BayeNet ${TEST_BAYENET_FILES} test/test_main.cpp)

# Add the Variable test executable
add_executable(tests_Variable ${TEST_VARIABLE_FILES} test/test_main.cpp)

# Link Google Test to the BayesianNetwork test executable
target_link_libraries(tests_BayeNet GTest::GTest GTest::Main ${PROJECT_NAME})

# Link Google Test to the Variable test executable
target_link_libraries(tests_Variable GTest::GTest GTest::Main ${PROJECT_NAME})

# Compiler warnings
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(${PROJECT_NAME} PRIVATE /W4)
endif()
