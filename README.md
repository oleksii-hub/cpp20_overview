# C++20 Features Overview

A minimal project demonstrating C++20 features with tests.

## Quick Start

### Build
```bash
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```

### Run Tests
```bash
cd build
ctest --output-on-failure
```

## Project Structure

```
cpp20_overview/
├── .gitignore
├── CMakeLists.txt
├── src/
│   └── spaceship_operator.hpp
└── tests/
    ├── CMakeLists.txt
    └── test_spaceship_operator.cpp
```

## What's Implemented

- **Spaceship Operators**: Three-way comparison operator (`<=>`) and synthesized operators

## Test Results

All tests passing ✓

```
SpaceshipTest.* (6 tests)
```

## Requirements

- **CMake** 3.25+
- **C++20 compiler**: GCC 11+, Clang 12+, or MSVC 17+
- **GoogleTest** (auto-downloaded via CMake)

## Adding More Features

To add another C++20 feature:

1. Create `src/new_feature.hpp` with your implementation
2. Create `tests/test_new_feature.cpp` with tests
3. Add to `tests/CMakeLists.txt`:
   ```cmake
   add_feature_test(test_new_feature)
   ```
4. Rebuild: `cmake --build . && ctest --output-on-failure`

## Compiler Support

| Compiler | Min Version | Status |
|----------|-------------|--------|
| GCC | 11 | ✅ Supported |
| Clang | 12 | ✅ Supported |
| MSVC | 17 | ✅ Tested |

**C++ Standard:** C++20 (can be changed to C++20 in CMakeLists.txt)

## Building Details

### Release Build
```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

### Debug Build (with sanitizers on GCC/Clang)
```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --config Debug
```

### With Specific Compiler
```bash
CC=gcc-11 CXX=g++-11 cmake ..
```

## Troubleshooting

### CMake not found
→ Install from https://cmake.org

### Compiler not found
→ Install GCC/Clang/MSVC

### Tests don't compile
→ Check C++20 support in compiler
→ Update CMakeLists.txt C++ standard

### Build fails
```bash
rm -rf build
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```

## Resources

- [cppreference - C++20](https://en.cppreference.com/w/cpp/20)
- [Google Test Docs](https://github.com/google/googletest)
