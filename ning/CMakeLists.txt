cmake_minimum_required(VERSION 3.10)
project(EnemyStateMachine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directories
include_directories(${PROJECT_SOURCE_DIR})

# Add executable for enemy demo
add_executable(enemy_demo
    enemy.cpp
    enemy_demo.cpp
    enemy_state.cpp
)

# Optionally, set compiler flags for better diagnostics
if(MSVC)
    target_compile_options(enemy_demo PRIVATE /W4)
else()
    target_compile_options(enemy_demo PRIVATE -Wall -Wextra -Wpedantic)
endif()
