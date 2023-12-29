# CMakeLists.txt

# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(Polygon)

# Add an executable and specify source files
add_executable(${PROJECT_NAME} main.cpp polygon.cpp)

# Set include directories for header files
# PRIVATE indicates that these include directories are only for the target ${PROJECT_NAME}
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Set the C++ standard to 11
set(CMAKE_CXX_STANDARD 11)
# Require the specified C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)
