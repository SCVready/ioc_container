cmake_minimum_required(VERSION 3.10)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

project(ioc_container)

# Library
add_library(ioc_container SHARED
                    src/IocContainer.cpp)

target_include_directories(ioc_container PUBLIC inc)

# Example
add_executable(example
                    example/main.cpp)

target_include_directories(example PUBLIC inc example)
target_link_libraries(example PUBLIC ioc_container)
