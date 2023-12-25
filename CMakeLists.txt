# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(Polygon)

# Add executable and specify source files
add_executable(${PROJECT_NAME} main.cpp polygon.cpp)

# Set include directories for header files
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
