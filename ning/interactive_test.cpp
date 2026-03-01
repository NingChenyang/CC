#include "player.h"
#include "enemy.h"
#include "map.hpp"
#include <iostream>
#include <string>

void printGameState(Player& player, Enemy& enemy) {
    std::cout << "\n=== 游戏状态 ===" << std::endl;
    std::cout << "玩家位置: " << player.getPos() << ", 血量: " << player.getHp() << std::endl;
    std::cout << "敌人位置: " << enemy.getPos() << ", 血量: " << enemy.getHP() << ", 韧性: " << enemy.getTenacity() << std::endl;
    std::cout << "距离: " << std::abs(player.getPos() - enemy.getPos()) << std::endl;
    std::cout << "==============" << std::endl;
}

void printHelp() {
    std::cout << "\n操作说明：" << std::endl;
    std::cout << "a: 左移1格" << std::endl;
    std::cout << "d: 右移1格" << std::endl;
    std::cout << "s: 不动" << std::endl;
    std::cout << "q: 左移2格" << std::endl;
    std::cout << "e: 右移2格" << std::endl;
    std::cout << "f: 攻击敌人" << std::endl;
    std::cout << "h: 显示帮助" << std::endl;
    std::cout << "x: 退出游戏" << std::endl;
}

int main() {
    std::cout << "=== 交互式游戏测试 ===" << std::endl;
    
    // 创建地图
    Map map(10);
    
    // 创建玩家
    Player player(0, 100, 1, 3, 5);
    map.setPlayer(&player);
    
    // 创建敌人
    Enemy enemy(2, 2, 1, 20, 8,
         5, 9, 10);
    enemy.setMap(&map);
    map.addEnemy(&enemy);
    
    printHelp();
    printGameState(player, enemy);
    
    char input;
    bool gameOver = false;
    
    while (!gameOver) {
        std::cout << "\n请输入操作: ";
        std::cin >> input;
        
        switch (input) {
            case 'a':
            case 'd':
            case 's':
            case 'q':
            case 'e':
            case 'f':
                // 玩家执行操作
                player.update(input);
                // 敌人执行动作
                enemy.update();
                // 检查游戏结束条件
                if (player.isDead()) {
                    std::cout << "\n=== 游戏结束：玩家死亡 ===" << std::endl;
                    gameOver = true;
                }
                if (enemy.getHP() <= 0) {
                    std::cout << "\n=== 游戏结束：敌人死亡 ===" << std::endl;
                    gameOver = true;
                }
                printGameState(player, enemy);
                break;
            case 'h':
                printHelp();
                break;
            case 'x':
                std::cout << "\n=== 游戏退出 ===" << std::endl;
                gameOver = true;
                break;
            default:
                std::cout << "无效操作，请输入 h 查看帮助" << std::endl;
                break;
        }
    }
    
    std::cout << "\n=== 游戏测试完成 ===" << std::endl;
    return 0;
}