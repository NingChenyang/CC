#include "enemy.h"
#include "Player.h"
#include "map.hpp"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    std::cout << "=== 敌人状态机测试 ===" << std::endl;
    
    // 创建地图
    Map map(10);
    
    
    
    // 创建敌人
    Enemy enemy(2, 2, 1, 20, 6, 5, 9, 10); // attackDmg, zhuijiDis, attackDis, hp, pos, patrol_a, patrol_b, tenacity
    map.addEnemy(&enemy);
    std::cout << "创建敌人，初始位置: " << enemy.getPos() << "，初始血量: " << enemy.getHP() << std::endl;
    
    // 测试巡逻状态 无问题
    // std::cout << "\n=== 测试巡逻状态 ===" << std::endl;
    // for (int i = 0; i < 10; i++) {
    //     enemy.update();
    //     // std::this_thread::sleep_for(std::chrono::milliseconds(500));
    // }
    
    // 测试发现玩家
    // 创建玩家
    Player player(5, 100);
    map.setPlayer(&player);
    std::cout << "创建玩家，初始位置: " << player.getPos() << "，初始血量: " << player.getHp() << std::endl;
    std::cout << "\n=== 测试发现玩家 ===" << std::endl;

    enemy.update();
    // std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // 测试追击状态
    std::cout << "\n=== 测试追击状态 ===" << std::endl;
    for (int i = 0; i < 2; i++) {
        enemy.update();
        // std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    // 测试攻击状态
    std::cout << "\n=== 测试攻击状态 ===" << std::endl;
    for (int i = 0; i < 2; i++) {
        enemy.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    // 测试敌人受伤
    std::cout << "\n=== 测试敌人受伤 ===" << std::endl;
    std::cout << "敌人当前血量: " << enemy.getHP() << "，当前韧性: " << enemy.getTenacity() << std::endl;
    enemy.takeDamage(8); // 造成伤害
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "敌人当前血量: " << enemy.getHP() << "，当前韧性: " << enemy.getTenacity() << std::endl;
    // 测试瘫痪状态
    std::cout << "\n=== 测试瘫痪状态 ===" << std::endl;
    enemy.takeDamage(3); // 再次造成伤害，触发瘫痪
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // 测试瘫痪恢复
    std::cout << "\n=== 测试瘫痪恢复 ===" << std::endl;
    for (int i = 0; i < 2; i++) {
        enemy.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    // 测试敌人死亡
    std::cout << "\n=== 测试敌人死亡 ===" << std::endl;
    std::cout << "敌人当前血量: " << enemy.getHP() << std::endl;
    enemy.takeDamage(10); // 造成致命伤害
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // 测试死亡状态
    std::cout << "\n=== 测试死亡状态 ===" << std::endl;
    enemy.update(); // 尝试更新死亡状态的敌人
    enemy.attack(); // 尝试攻击
    enemy.move(1); // 尝试移动
    enemy.takeDamage(5); // 尝试造成伤害
    
    std::cout << "\n=== 测试完成 ===" << std::endl;
    
    return 0;
}