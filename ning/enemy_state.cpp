#include "enemy_state.h"
class Enemy; // 提前声明
// Patrol: 在 patrol_a_ 和 patrol_b_ 之间来回
void PatrolState::enter(Enemy* e) { std::cout<<"Enter Patrol\n"; }
void PatrolState::exit(Enemy* e) { std::cout<<"Exit Patrol\n"; }
void PatrolState::update(Enemy* e) {
    // 发现玩家（在检测范围内） -> 切换 Chase
    if (e->getPlayerPos() >= 0) {
        int dist = std::abs(e->getPlayerPos() - e->getPos());
        if (dist <= 3) { // 视野范围
            std::cout << "Detect player at "<< e->getPlayerPos() <<" (dist="<<dist<<") -> Chase\n";
            e->setState(e->getChaseState());
            return;
        }
    }

    // 巡逻移动一步
    int dir = e->getPatrolDir();
    int next = e->getPos() + dir;
    if (next < e->getPatrolA() || next > e->getPatrolB()) {
        e->setPatrolDir(-dir);
        dir = e->getPatrolDir();
        next = e->getPos() + dir;
    }
    e->setPos(next);
    std::cout << "Patrol: pos -> " << e->getPos() << "\n";
}
void PatrolState::move(Enemy* e, int dir) {
    // 外部强制移动也可视为切换至 Patrol 后移动
    e->setState(e->getPatrolState());
    int next = e->getPos() + dir;
    e->setPos(next);
    std::cout << "Patrol move forced to "<< e->getPos() <<"\n";
}
void PatrolState::attack(Enemy* e) { std::cout<<"Patrol: no attack\n"; }
void PatrolState::takeDamage(Enemy* e, int dmg) {
    std::cout<<"Patrol: takeDamage "<<dmg<<"\n";
    e->setHP(e->getHP() - dmg);
    if (e->getHP() <= 0) return;
    // 受伤可能发现玩家，切换为 Chase
    if (e->getPlayerPos() >= 0) e->setState(e->getChaseState());
}

// Chase: 追击玩家
void ChaseState::enter(Enemy* e) { std::cout<<"Enter Chase\n"; }
void ChaseState::exit(Enemy* e) { std::cout<<"Exit Chase\n"; }
void ChaseState::update(Enemy* e) {
    if (e->getPlayerPos() < 0) { e->setState(e->getPatrolState()); return; }
    int pos = e->getPos();
    int target = e->getPlayerPos();
    int dist = std::abs(target - pos);
    if (dist <= 1) {
        std::cout<<"Chase: in range -> Attack\n";
        e->setState(e->getAttackState());
        return;
    }
    int dir = (target > pos) ? 1 : -1;
    e->setPos(pos + dir);
    std::cout<<"Chase: move to "<< e->getPos() <<" (target="<<target<<")\n";
}
void ChaseState::move(Enemy* e, int dir) { /* not used */ }
void ChaseState::attack(Enemy* e) { std::cout<<"Chase: not in range\n"; }
void ChaseState::takeDamage(Enemy* e, int dmg) {
    std::cout<<"Chase: takeDamage "<<dmg<<"\n";
    e->setHP(e->getHP() - dmg);
}

// Attack: 接近玩家后攻击
void AttackState::enter(Enemy* e) { std::cout<<"Enter Attack\n"; }
void AttackState::exit(Enemy* e) { std::cout<<"Exit Attack\n"; }
void AttackState::update(Enemy* e) {
    std::cout<<"Attack: strike player!\n";
    // 攻击后回到 Chase（继续追击/接触）
    e->setState(e->getChaseState());
}
void AttackState::move(Enemy* e, int dir) { (void)e; (void)dir; }
void AttackState::attack(Enemy* e) { std::cout<<"Attack: direct attack call\n"; }
void AttackState::takeDamage(Enemy* e, int dmg) {
    std::cout<<"Attack: takeDamage "<<dmg<<"\n";
    e->setHP(e->getHP() - dmg);
}

// Stunned: 韧性为 0 时进入，无法行动，短时间后恢复韧性并回到巡逻
void StunnedState::enter(Enemy* e) { std::cout<<"Enter Stunned\n"; }
void StunnedState::exit(Enemy* e) { std::cout<<"Exit Stunned\n"; }
void StunnedState::update(Enemy* e) {
    std::cout<<"Stunned: recovering...\n";
    // 简单恢复逻辑：恢复一定韧性，然后回到 Patrol
    e->changeTenacity(3); // 恢复 3 点
    e->setState(e->getPatrolState());
}
void StunnedState::move(Enemy* e, int dir) { (void)e; (void)dir; std::cout<<"Stunned: cannot move\n"; }
void StunnedState::attack(Enemy* e) { (void)e; std::cout<<"Stunned: cannot attack\n"; }
void StunnedState::takeDamage(Enemy* e, int dmg) { std::cout<<"Stunned: takeDamage "<<dmg<<"\n"; e->setHP(e->getHP()-dmg); }

// Dead
void DeadState::enter(Enemy* e) { std::cout<<"Enter Dead\n"; }
void DeadState::exit(Enemy* e) { (void)e; }
void DeadState::update(Enemy* e) { (void)e; }
void DeadState::move(Enemy* e, int dir) { (void)e; (void)dir; std::cout<<"Dead: cannot move\n"; }
void DeadState::attack(Enemy* e) { (void)e; std::cout<<"Dead: cannot attack\n"; }
void DeadState::takeDamage(Enemy* e, int dmg) { (void)e; (void)dmg; std::cout<<"Dead: already dead\n"; }