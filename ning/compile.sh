#!/bin/bash

# 编译所有源文件
g++ -std=c++11 -o test_enemy test_enemy.cpp enemy.cpp enemy_state.cpp Player.cpp -I.

# 运行测试
if [ $? -eq 0 ]; then
    echo "编译成功，运行测试..."
    ./test_enemy
else
    echo "编译失败"
    exit 1
fi