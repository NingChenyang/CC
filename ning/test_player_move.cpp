#include "player.h"
#include "enemy.h"
#include "map.hpp"
#include <iostream>

int main() {
    std::cout << "=== 玩家移动指令测试 ===" << std::endl;
    
    // 创建地图
    Map map(10);
    
    // 创建玩家
    Player player(5, 100);
    map.setPlayer(&player);
    std::cout << "创建玩家，初始位置: " << player.getPos() << "，初始血量: " << player.getHp() << std::endl;
    
    // 创建敌人
    Enemy enemy(2, 2, 1, 20, 8, 5, 9, 10);
    map.addEnemy(&enemy);
    std::cout << "创建敌人，初始位置: " << enemy.getPos() << "，初始血量: " << enemy.getHP() << std::endl;
    
    // // 设置敌人和玩家的相互引用
    // player.setEnemy(&enemy);
    // enemy.setPlayer(&player);
    
    // 测试移动指令
    std::cout << "\n=== 测试移动指令 ===" << std::endl;
    
    // 测试右移动一格
    std::cout << "\n测试右移动一格 (d):" << std::endl;
    player.update('d');
    
    // 测试右移动两格
    std::cout << "\n测试右移动两格 (e):" << std::endl;
    player.update('e');
    
    // 测试左移动一格
    std::cout << "\n测试左移动一格 (a):" << std::endl;
    player.update('a');
    
    // 测试左移动两格
    std::cout << "\n测试左移动两格 (q):" << std::endl;
    player.update('q');
    
    // 测试不动
    std::cout << "\n测试不动 (s):" << std::endl;
    player.update('s');
    
    // 测试无效指令
    std::cout << "\n测试无效指令 (x):" << std::endl;
    player.update('x');
    
    // 测试边界情况
    std::cout << "\n=== 测试边界情况 ===" << std::endl;
    
    // 移动到左边界
    std::cout << "\n测试移动到左边界:" << std::endl;
    player.setPos(1);
    player.update('q'); // 左移动两格，应该停在0
    std::cout << "当前位置: " << player.getPos() << std::endl;
    
    // 移动到右边界
    std::cout << "\n测试移动到右边界:" << std::endl;
    player.setPos(8);
    player.update('e'); // 右移动两格，应该停在9
    std::cout << "当前位置: " << player.getPos() << std::endl;
    std::cout << "\n=== 测试完成 ===" << std::endl;
    
    return 0;
}