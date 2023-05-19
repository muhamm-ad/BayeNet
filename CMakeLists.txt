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

# Add the library
add_library(${PROJECT_NAME} ${SOURCE_FILES})

# Add include directories
target_include_directories(${PROJECT_NAME} 
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/lib
)

# Compiler warnings
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(${PROJECT_NAME} PRIVATE /W4)
endif()
