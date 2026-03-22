cmake_minimum_required(VERSION 3.25)
project(cpp-modern LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Sanitizers on debug builds (GCC/Clang only)
if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT MSVC)
  add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address,undefined)
endif()

# Header-only library
add_library(cpp-modern-lib INTERFACE
        tests/test_spaceship_operator.cpp)
target_include_directories(cpp-modern-lib INTERFACE ${CMAKE_SOURCE_DIR}/src)

# Compiler-specific warning flags
if(MSVC)
  target_compile_options(cpp-modern-lib INTERFACE /W4)
else()
  target_compile_options(cpp-modern-lib INTERFACE -Wall -Wextra -Wpedantic)
endif()

# Pull in GoogleTest via FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_subdirectory(tests)