#include "enemy.h"
#include "Player.h"
#include "map.hpp"
#include <iostream>

void testEnemyStateExtended() {
    std::cout << "=== 敌人状态机扩展测试 ===" << std::endl;
    
    // 创建地图
    Map map(10);
    
    // 创建玩家
    Player player(5, 100, 1, 3, 5);
    map.setPlayer(&player);
    
    // 创建敌人
    Enemy enemy(2, 3, 1, 20, 6, 0, 9, 10);
    enemy.setMap(&map);
    map.addEnemy(&enemy);
    
    std::cout << "\n=== 测试1: 玩家超出追击范围 ===" << std::endl;
    // 玩家移动到超出敌人追击范围的位置
    player.move(5); // 从5移动到10，会被限制在9
    std::cout << "玩家移动到位置: " << player.getPos() << std::endl;
    std::cout << "敌人当前状态: 巡逻" << std::endl;
    enemy.update(); // 敌人更新，应该发现玩家
    enemy.update(); // 敌人追击
    enemy.update(); // 敌人继续追击
    enemy.update(); // 敌人应该发现玩家超出范围，返回巡逻
    
    std::cout << "\n=== 测试2: 敌人在追击过程中失去玩家 ===" << std::endl;
    // 重置敌人状态
    enemy.setState(enemy.getPatrolState());
    // 玩家移动到敌人附近
    player.move(-4); // 从9移动到5
    std::cout << "玩家移动到位置: " << player.getPos() << std::endl;
    enemy.update(); // 敌人发现玩家并开始追击
    // 玩家快速移动到远处
    player.move(5); // 从5移动到10，被限制在9
    std::cout << "玩家移动到位置: " << player.getPos() << std::endl;
    enemy.update(); // 敌人应该失去玩家，返回巡逻
    
    std::cout << "\n=== 测试3: 敌人巡逻路线改变 ===" << std::endl;
    // 重置敌人状态
    enemy.setState(enemy.getPatrolState());
    // 设置固定巡逻点
    enemy.setPatrolPoints(2, 7);
    std::cout << "敌人巡逻范围: " << enemy.getPatrolA() << "-" << enemy.getPatrolB() << std::endl;
    // 测试巡逻边界处理
    for (int i = 0; i < 10; i++) {
        enemy.update();
        std::cout << "巡逻位置: " << enemy.getPos() << ", 巡逻点: " << enemy.getPatrolA() << "-" << enemy.getPatrolB() << std::endl;
    }
    
    std::cout << "\n=== 测试4: 玩家在攻击范围内移动 ===" << std::endl;
    // 重置敌人状态
    enemy.setState(enemy.getPatrolState());
    // 玩家移动到敌人攻击范围内
    player.setPos(6); // 移动到敌人位置附近
    std::cout << "玩家移动到位置: " << player.getPos() << std::endl;
    enemy.update(); // 敌人发现玩家并开始追击
    enemy.update(); // 敌人进入攻击状态
    // 玩家在攻击范围内移动
    player.move(1); // 移动到7
    std::cout << "玩家移动到位置: " << player.getPos() << std::endl;
    enemy.update(); // 敌人应该保持攻击状态
    player.move(-2); // 移动到5
    std::cout << "玩家移动到位置: " << player.getPos() << std::endl;
    enemy.update(); // 敌人应该保持攻击状态
    
    std::cout << "\n=== 测试5: 敌人在不同状态下受到伤害 ===" << std::endl;
    // 重置敌人状态和血量
    enemy.setHP(20);
    enemy.setState(enemy.getPatrolState());
    std::cout << "敌人当前状态: 巡逻" << std::endl;
    enemy.takeDamage(5); // 巡逻状态下受伤
    
    // 敌人切换到追击状态
    player.setPos(7);
    enemy.update();
    std::cout << "敌人当前状态: 追击" << std::endl;
    enemy.takeDamage(5); // 追击状态下受伤
    
    // 敌人切换到攻击状态
    player.setPos(6);
    enemy.update();
    enemy.update();
    std::cout << "敌人当前状态: 攻击" << std::endl;
    enemy.takeDamage(5); // 攻击状态下受伤
    
    std::cout << "\n=== 测试6: 敌人连续受到伤害进入瘫痪 ===" << std::endl;
    // 重置敌人状态、血量和韧性
    enemy.setHP(20);
    enemy.changeTenacity(10); // 恢复韧性
    enemy.setState(enemy.getAttackState());
    std::cout << "敌人当前韧性: " << enemy.getTenacity() << std::endl;
    enemy.takeDamage(6); // 第一次受伤，减少韧性
    std::cout << "敌人当前韧性: " << enemy.getTenacity() << std::endl;
    enemy.takeDamage(6); // 第二次受伤，应该进入瘫痪状态
    
    std::cout << "\n=== 测试7: 瘫痪状态下敌人恢复 ===" << std::endl;
    // 测试瘫痪恢复过程
    for (int i = 0; i < 3; i++) {
        enemy.update();
    }
    
    std::cout << "\n=== 测试8: 敌人死亡后状态 ===" << std::endl;
    // 重置敌人状态和血量
    enemy.setHP(5);
    enemy.setState(enemy.getAttackState());
    std::cout << "敌人当前血量: " << enemy.getHP() << std::endl;
    enemy.takeDamage(10); // 敌人应该死亡
    // 测试死亡状态下的行为
    enemy.attack();
    enemy.move(1);
    enemy.takeDamage(5);
    
    std::cout << "\n=== 敌人状态机扩展测试完成 ===" << std::endl;
}

int main() {
    testEnemyStateExtended();
    return 0;
}