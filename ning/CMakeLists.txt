cmake_minimum_required(VERSION 3.10)
project(EnemyStateMachine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directories
include_directories(${PROJECT_SOURCE_DIR})


# Add executable for player move test
add_executable(test_player_move
    player.cpp
    test_player_move.cpp
    enemy.cpp
    map.hpp
    enemy_state.cpp
    player_state.cpp
)
add_executable(interactive_test
    player.cpp
    interactive_test.cpp
    enemy.cpp
    map.hpp
    enemy_state.cpp
    player_state.cpp
)
