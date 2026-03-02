#include "enemy.h"
#include <iostream>
#include "map.hpp"
int main() {
    // 创建地图
    Map map(10);
    
    // 创建敌人
    Enemy enemy1(2, 1, 10, 2, 0, 4, 5);
    // Enemy enemy2(15, 7, 5, 9, 8);
    
    // 将敌人添加到地图
    map.addEnemy(&enemy1);
    // map.addEnemy(&enemy2);
    
    // // 设置玩家位置
    // map.setPlayerPos(5);
    // std::cout << "Player position: " << map.getPlayerPos() << std::endl;
    
    // 更新敌人状态
    std::cout << "\n=== Updating enemies ===\n";
    // enemy1.update();

    while (1) {
        enemy1.update();
    }

    // enemy2.update();
    
    // // 玩家移动
    // map.setPlayerPos(3);
    // std::cout << "\nPlayer moved to: " << map.getPlayerPos() << std::endl;
    
    // // 再次更新敌人状态
    // std::cout << "\n=== Updating enemies again ===\n";
    // enemy1.update();
    // enemy2.update();
    
    // // 玩家离开
    // map.setPlayerPos(-1);
    // std::cout << "\nPlayer left the map" << std::endl;
    
    // // 再次更新敌人状态
    // std::cout << "\n=== Updating enemies after player left ===\n";
    // enemy1.update();
    // enemy2.update();
    
    return 0;
}