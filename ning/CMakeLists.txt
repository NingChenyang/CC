cmake_minimum_required(VERSION 3.10)
project(EnemyStateMachine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directories
include_directories(${PROJECT_SOURCE_DIR})



add_executable(interactive_test
    player.cpp
    interactive_test.cpp
    enemy.cpp
    map.hpp
    enemy_state.cpp
    player_state.cpp
)
