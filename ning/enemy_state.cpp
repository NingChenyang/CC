#include "enemy_state.h"
#include "enemy.h"

class Enemy;
class Player;
void PatrolState::enter() { std::cout << "进入巡逻" << std::endl; }
void PatrolState::exit() { std::cout << "退出巡逻" << std::endl; }
void PatrolState::update() {

  if (!enemy_)
    return;
  Map *map = enemy_->getMap();
  if (!map)
    return;

  // 巡逻移动一步
  // 如果到了边界，改变方向
  // if (enemy_->getPos() <= enemy_->getPatrolA() || enemy_->getPos() >=
  // enemy_->getPatrolB()) {
  //     int dir = -enemy_->getPatrolDir();
  //     enemy_->setPatrolDir(dir);
  // }

  //   int dir = enemy_->getPatrolDir();
  //   int next = enemy_->getPos() + dir;
  // 检查是否到达巡逻边界
  int minPos = map->getRight() / 2;
  int maxPos = map->getRight();
  // 如果到了边界，改变方向
  if (enemy_->getPos() <= enemy_->getPatrolA() ||
      enemy_->getPos() >= enemy_->getPatrolB()) {
    int dir = -enemy_->getPatrolDir();
    enemy_->setPatrolDir(dir);
    if (dir > 0) {
      // 向右移动，设置巡逻点B
      // 生成 [patrolA, maxPos] 范围内的随机数
      int randomPatrolB =
          enemy_->getPatrolA() + 1 + rand() % (maxPos - enemy_->getPatrolA());
      enemy_->setPatrolB(randomPatrolB);
    } else {
      // 向左移动，设置巡逻点A
      // 生成 [minPos, patrolB] 范围内的随机数
      int randomPatrolA = minPos + rand() % (enemy_->getPatrolB() - minPos);
      enemy_->setPatrolA(randomPatrolA);
    }
  }
  int next = enemy_->getPos() + enemy_->getPatrolDir();
  // 更新位置
  if (map->isValidPos(next)) {

    enemy_->setPos(next);
  }

  std::cout << "Patrol: pos -> " << enemy_->getPos() << std::endl;

  Player *player = map->detectPlayer(enemy_->getPos(), enemy_->getZhuijiDis());
  if (player == nullptr) {
    enemy_->clearPlayer();
    return;
  } // 没找到玩家，继续巡逻
  enemy_->setPlayer(player); // 设置目标
  int dist = std::abs(player->getPos() - enemy_->getPos());
  if (dist <= enemy_->getAttackDis()) { // 如果玩家在攻击范围内直接进入攻击状态
    enemy_->setState(enemy_->getAttackState()); // 进入攻击
    return;                                     // 进入攻击状态后，不继续巡逻
  }
  enemy_->setState(enemy_->getChaseState()); // 进入追击状态
}
void PatrolState::move(int dir) {}
void PatrolState::attack() { std::cout << "Patrol: no attack\n"; }
void PatrolState::takeDamage(int dmg) {
  // 玩家只会近战，不会受到攻击
}

// Chase: 追击玩家
void ChaseState::enter() { std::cout << "进入追击" << std::endl; }
void ChaseState::exit() { std::cout << "退出追击" << std::endl; }
void ChaseState::update() {
  if (!enemy_)
    return;
  Map *map = enemy_->getMap();
  Player *player = enemy_->getPlayer();

  // 检查地图和玩家是否存在
  if (!map || !player) {
    std::cout << "目标丢失，继续巡逻" << std::endl;
    enemy_->clearPlayer();
    enemy_->setState(enemy_->getPatrolState());
    return;
  }

  // 向玩家移动一格
  int pos = enemy_->getPos();
  int target = player->getPos();
  int dir = (target > pos) ? 1 : -1;

  // 检查移动是否有效
  int nextPos = pos + dir;
  if (map->isValidPos(nextPos)) {
    enemy_->setPos(nextPos);
  }

  std::cout << "Chase: move to " << enemy_->getPos() << " (target=" << target
            << ")\n";

  int dist = std::abs(player->getPos() - enemy_->getPos());
  if (dist > enemy_->getZhuijiDis()) {
    std::cout << "目标丢失，继续巡逻" << std::endl;
    enemy_->clearPlayer();
    enemy_->setState(enemy_->getPatrolState());
    return;
  } else if (dist <= enemy_->getAttackDis()) {
    std::cout << "目标在攻击范围内，切换至攻击状态" << std::endl;
    enemy_->setState(enemy_->getAttackState());
    return;
  }
}
void ChaseState::move(int dir) {}
void ChaseState::attack() { std::cout << "Chase: not in range\n"; }
void ChaseState::takeDamage(int dmg) {
  std::cout << "Chase: takeDamage " << dmg << "\n";
  enemy_->setHP(enemy_->getHP() - dmg);
}

// Attack: 接近玩家后攻击
void AttackState::enter() { std::cout << "Enter Attack\n"; }
void AttackState::exit() { std::cout << "Exit Attack\n"; }
void AttackState::update() {
  if (!enemy_)
    return;
  Player *player = enemy_->getPlayer();
  if (!player) {
    std::cout << "目标丢失，继续巡逻" << std::endl;
    enemy_->clearPlayer();
    enemy_->setState(enemy_->getPatrolState());
    return;
  }
  std::cout << "攻击目标" << std::endl;
  player->takeDamage(enemy_->getAttackDmg());

  int dist = std::abs(player->getPos() - enemy_->getPos());
  if (dist > enemy_->getZhuijiDis()) {
    std::cout << "目标丢失，继续巡逻" << std::endl;
    enemy_->clearPlayer();
    enemy_->setState(enemy_->getPatrolState());
    return;
  } else if (dist <= enemy_->getAttackDis()) {
    std::cout << "目标在攻击范围内，切换至攻击状态" << std::endl;
    return;
  } else {
    std::cout << "目标在追击范围，切换至追击状态" << std::endl;
    enemy_->setState(enemy_->getChaseState());
    return;
  }
}
void AttackState::move(int dir) { (void)dir; }
void AttackState::attack() { std::cout << "Attack: direct attack call\n"; }
void AttackState::takeDamage(int dmg) {
  std::cout << "Attack: takeDamage " << dmg << "\n";
  enemy_->setHP(enemy_->getHP() - dmg);
  enemy_->changeTenacity(-1 * dmg); // 攻击后减少韧性暂时设置与伤害相同
  if (enemy_->getHP() <= 0) {
    enemy_->setState(enemy_->getDeadState());
    return;
  }
  if (enemy_->getTenacity() <= 0) {
    enemy_->setState(enemy_->getStunnedState());
  }
}

// Stunned: 韧性为 0 时进入，无法行动，短时间后恢复韧性并回到巡逻
void StunnedState::enter() {
  std::cout << "Enter Stunned\n";
  turns_ = 0; // 重置回合计数器
}
void StunnedState::exit() { std::cout << "Exit Stunned\n"; }
void StunnedState::update() {
  if (!enemy_)
    return;
  turns_++;
  std::cout << "Stunned: recovering... (turn " << turns_ << "/2)\n";

  // 经过两个回合后恢复
  if (turns_ >= 2) {
    enemy_->recoverTenacity(); // 恢复韧性

    Player *player = enemy_->getPlayer();
    if (!player) {
      std::cout << "目标丢失，继续巡逻" << std::endl;
      enemy_->clearPlayer();
      enemy_->setState(enemy_->getPatrolState());
      return;
    }

    int dist = std::abs(player->getPos() - enemy_->getPos());
    if (dist > enemy_->getZhuijiDis()) {
      std::cout << "目标丢失，继续巡逻" << std::endl;
      enemy_->clearPlayer();
      enemy_->setState(enemy_->getPatrolState());
      return;
    } else if (dist <= enemy_->getAttackDis()) {
      std::cout << "目标在攻击范围内，切换至攻击状态" << std::endl;
      enemy_->setState(enemy_->getAttackState());
      return;
    } else {
      std::cout << "目标在追击范围，继续追击" << std::endl;
      enemy_->setState(enemy_->getChaseState());
    }
  }
}
void StunnedState::move(int dir) {
  (void)dir;
  std::cout << "Stunned: cannot move\n";
}
void StunnedState::attack() { std::cout << "Stunned: cannot attack\n"; }
void StunnedState::takeDamage(int dmg) {
  std::cout << "Stunned: takeDamage " << dmg << "\n";
  enemy_->setHP(enemy_->getHP() - dmg);
}

// Dead
void DeadState::enter() { std::cout << "Enter Dead\n"; }
void DeadState::exit() {}
void DeadState::update() {}
void DeadState::move(int dir) {
  (void)dir;
  std::cout << "Dead: cannot move\n";
}
void DeadState::attack() { std::cout << "Dead: cannot attack\n"; }
void DeadState::takeDamage(int dmg) {
  (void)dmg;
  std::cout << "Dead: already dead\n";
}