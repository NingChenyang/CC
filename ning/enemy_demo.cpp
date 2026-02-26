#include "enemy.h"

int main() {
    // 初始化：HP=15，位置=2，巡逻点 2..6，韧性 3
    Enemy e(15, 2, 2, 6, 3);

    std::cout << "Initial: HP=" << e.getHP() << " pos=" << e.getPos() << "\n";

    // 模拟若干帧，敌人在巡逻
    for (int t = 0; t < 4; ++t) e.update();

    // 玩家出现在远端（pos=8），触发追击
    std::cout << "-- Player appears at 8 --\n";
    e.setPlayerPos(8);
    for (int t = 0; t < 6; ++t) e.update();

    // 对敌人造成韧性伤害，使其瘫痪
    std::cout << "-- Player hits tenacity --\n";
    e.changeTenacity(-3); // tenacity -> 0 -> Stunned
    e.update(); // Stunned 恢复并回到巡逻

    // 再次让玩家出现并对敌人造成致命伤
    std::cout << "-- Player appears and finishes enemy --\n";
    e.setPlayerPos(4);
    for (int t = 0; t < 2; ++t) e.update();
    e.takeDamage(50); // 直接杀死

    // 死亡后尝试动作
    e.move(-1);
    e.attack();

    return 0;
}

