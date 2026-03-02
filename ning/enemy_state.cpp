#include "enemy_state.h"
#include "enemy.h"

class Enemy;
class Player;

// 状态转换辅助方法
void State::checkStateTransition() {
  
  if (!enemy_)
    return;
  if (dynamic_cast<DeadState *>(this) == enemy_->getDeadState())
  {return;}
  if (enemy_->getHP() <= 0) {
    enemy_->setState(enemy_->getDeadState());
    return;
  }

  if (enemy_->isEnraged()) {
    enemy_->recoverTenacity(); // 恢复韧性
    Map *map = enemy_->getMap();
    Player *player = enemy_->getPlayer();

    // 如果没有玩家，切换到巡逻状态
    if (!player) {
      player = map->detectPlayer(enemy_->getPos(), enemy_->getZhuijiDis());
      if (!player) {
        // 已经在巡逻状态，不需要切换
        return;
      }
      enemy_->setPlayer(player);
    }

    int dist = std::abs(player->getPos() - enemy_->getPos());

    // 根据距离切换状态
    if (dist > enemy_->getZhuijiDis()) {
      std::cout << "Enemy: 目标丢失，切换到巡逻状态" << std::endl;
      enemy_->clearPlayer();
      enemy_->setState(enemy_->getPatrolState());
    } else if (dist <= enemy_->getAttackDis()) {
      // 如果不在攻击状态，切换到攻击状态
      if (dynamic_cast<AttackState *>(this) == nullptr) {
        std::cout << "Enemy: 目标在攻击范围内，切换到攻击状态" << std::endl;
        enemy_->setState(enemy_->getAttackState());
      }
    } else {
      // 如果不在追击状态，切换到追击状态
      if (dynamic_cast<ChaseState *>(this) == nullptr) {
        std::cout << "Enemy: 目标在追击范围，切换到追击状态" << std::endl;
        enemy_->setState(enemy_->getChaseState());
      }
    }
    return;
  }
  if (dynamic_cast<StunnedState *>(this) == enemy_->getStunnedState() &&
      enemy_->getTenacity() <= 0) {
    // 经过两个回合后恢复
    if (dynamic_cast<StunnedState *>(this)->getTurns() >= 2) {
      enemy_->recoverTenacity(); // 恢复韧性

      Player *player = enemy_->getPlayer();
      if (!player) {
        std::cout << "目标丢失，继续巡逻" << std::endl;
        enemy_->clearPlayer();
        enemy_->setState(enemy_->getPatrolState());
        return;
      }
      checkStateTransition();
    }
    return;
  }

  // if (enemy_->getHP() <= enemy_->getMaxHp() * 0.1) {

  //   enemy_->recoverTenacity(); // 恢复韧性

  //   // // 触发狂暴状态
  //   // enemy_->setHP(enemy_->getMaxHp()); // 重新设置血量，触发狂暴状态

  //   Player *player = enemy_->getPlayer();
  //   if (!player) {
  //     std::cout << "目标丢失，继续巡逻" << std::endl;
  //     enemy_->clearPlayer();
  //     enemy_->setState(enemy_->getPatrolState());

  //     return;
  //   }
  //   // if (dynamic_cast<StunnedState *>(this) == enemy_->getStunnedState())
  //   // {
  //   //   dynamic_cast<StunnedState *>(this)->setTurns(2);
  //   //   checkStateTransition();
  //   // }
  //   // 检查状态转换
  //   // checkStateTransition();
  //   return;
  // }

  if (enemy_->getTenacity() <= 0 && !enemy_->isEnraged()) {
    enemy_->setState(enemy_->getStunnedState());
    return;
  }

  Map *map = enemy_->getMap();
  Player *player = enemy_->getPlayer();

  // 如果没有玩家，切换到巡逻状态
  if (!player) {
    player = map->detectPlayer(enemy_->getPos(), enemy_->getZhuijiDis());
    if (!player) {
      // 已经在巡逻状态，不需要切换
      return;
    }
    enemy_->setPlayer(player);
  }

  int dist = std::abs(player->getPos() - enemy_->getPos());

  // 根据距离切换状态
  if (dist > enemy_->getZhuijiDis()) {
    std::cout << "Enemy: 目标丢失，切换到巡逻状态" << std::endl;
    enemy_->clearPlayer();
    enemy_->setState(enemy_->getPatrolState());
  } else if (dist <= enemy_->getAttackDis()) {
    // 如果不在攻击状态，切换到攻击状态
    if (dynamic_cast<AttackState *>(this) == nullptr) {
      std::cout << "Enemy: 目标在攻击范围内，切换到攻击状态" << std::endl;
      enemy_->setState(enemy_->getAttackState());
    }
  } else {
    // 如果不在追击状态，切换到追击状态
    if (dynamic_cast<ChaseState *>(this) == nullptr) {
      std::cout << "Enemy: 目标在追击范围，切换到追击状态" << std::endl;
      enemy_->setState(enemy_->getChaseState());
    }
  }
}
void PatrolState::enter() { std::cout << "进入巡逻" << std::endl; }
void PatrolState::exit() { std::cout << "退出巡逻" << std::endl; }
void PatrolState::update() {
  if (!enemy_)
    return;
  Map *map = enemy_->getMap();
  if (!map)
    return;

  // 巡逻移动一步
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

  // 检查状态转换
  // checkStateTransition();
}

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
  // 检查是否可以使用瞬砍技能
  int dist = std::abs(player->getPos() - enemy_->getPos());
  if (enemy_->isEnraged() && enemy_->hasBlinkStrike() && dist == 3) {
    std::cout << "\n=== 敌人发动瞬砍技能！ ===" << std::endl;
    std::cout << "敌人瞬间移动到玩家位置并发动攻击！" << std::endl;

    // 移动到玩家位置
    enemy_->setPos(player->getPos());
    // 计算攻击力
    int attackDmg = enemy_->getAttackDmg();
    // 计算狂暴状态下的近战攻击力（近战攻击*狂暴buff2倍增伤）
    attackDmg = static_cast<int>(attackDmg * 2 * 2); // 近战2倍 * 狂暴2倍
    std::cout << "瞬砍攻击力: " << attackDmg << std::endl;

    // 消耗瞬砍技能
    enemy_->useBlinkStrike();
    // 检查是否是第四下攻击（每攻击三下，第四下攻击*2）
    enemy_->incrementAttackCount();
    if (enemy_->getAttackCount() % 4 == 0) {
      int originalDmg = attackDmg;
      attackDmg = static_cast<int>(attackDmg * 2);
      std::cout << "敌人发动强化攻击！攻击力提升至: " << attackDmg
                << " (原本: " << originalDmg << ")" << std::endl;
    }

    std::cout << "攻击目标" << std::endl;
    player->takeDamage(attackDmg);
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

  // // 检查状态转换
  // checkStateTransition();
}

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

  // 检查是否可以使用瞬砍技能
  int dist = std::abs(player->getPos() - enemy_->getPos());

  // 计算攻击力
  int attackDmg = enemy_->getAttackDmg();

  // // 检查是否触发瞬砍技能
  // if (enemy_->isEnraged() && enemy_->hasBlinkStrike() && dist == 3) {
  //   std::cout << "\n=== 敌人发动瞬砍技能！ ===" << std::endl;
  //   std::cout << "敌人瞬间移动到玩家位置并发动攻击！" << std::endl;

  //   // 移动到玩家位置
  //   enemy_->setPos(player->getPos());

  //   // 计算狂暴状态下的近战攻击力（近战攻击*狂暴buff2倍增伤）
  //   attackDmg = static_cast<int>(attackDmg * 2 * 2); // 近战2倍 * 狂暴2倍
  //   std::cout << "瞬砍攻击力: " << attackDmg << std::endl;

  //   // 消耗瞬砍技能
  //   enemy_->useBlinkStrike();
  // } else 
  if (dist == 0) {
    // 距离为0时的近战攻击
    if (enemy_->isEnraged()) {
      // 狂暴状态下的近战攻击
      attackDmg = static_cast<int>(attackDmg * 2 * 2); // 近战2倍 * 狂暴2倍
      std::cout << "敌人发动狂暴近战攻击！攻击力提升至: " << attackDmg
                << std::endl;
    } else {
      // 普通近战攻击
      attackDmg = static_cast<int>(attackDmg * 2);
      std::cout << "敌人发动近战攻击！攻击力提升至: " << attackDmg << std::endl;
    }
  } else if (enemy_->isEnraged()) {
    // 狂暴状态下的普通攻击
    attackDmg = static_cast<int>(attackDmg * 2);
    std::cout << "敌人发动狂暴攻击！攻击力提升至: " << attackDmg << std::endl;
  }

  // 检查是否是第四下攻击（每攻击三下，第四下攻击*2）
  enemy_->incrementAttackCount();
  if (enemy_->getAttackCount() % 4 == 0) {
    int originalDmg = attackDmg;
    attackDmg = static_cast<int>(attackDmg * 2);
    std::cout << "敌人发动强化攻击！攻击力提升至: " << attackDmg
              << " (原本: " << originalDmg << ")" << std::endl;
  }

  std::cout << "攻击目标" << std::endl;
  player->takeDamage(attackDmg);


}

void AttackState::takeDamage(int dmg) {
  std::cout << "Attack: takeDamage " << dmg << "\n";
  enemy_->setHP(enemy_->getHP() - dmg);
  if (!enemy_->isEnraged())
  enemy_->changeTenacity(-1 * dmg); // 攻击后减少韧性暂时设置与伤害相同
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

  // 检查是否血量小于等于10%，如果是，直接苏醒进入狂暴状态
  int maxHp = enemy_->getMaxHp();
  int currentHp = enemy_->getHP();
  // if (currentHp <= maxHp * 0.1) {
  //   std::cout << "\n=== 敌人血量过低，强制苏醒并进入狂暴状态！ ==="
  //             << std::endl;
  //   enemy_->recoverTenacity(); // 恢复韧性

  //   // 触发狂暴状态
  //   enemy_->setHP(currentHp); // 重新设置血量，触发狂暴状态

  //   Player *player = enemy_->getPlayer();
  //   if (!player) {
  //     std::cout << "目标丢失，继续巡逻" << std::endl;
  //     enemy_->clearPlayer();
  //     enemy_->setState(enemy_->getPatrolState());
  //     return;
  //   }

  //   // 检查状态转换
  //   // checkStateTransition();
  //   return;
  // }

  turns_++;
  std::cout << "Stunned: recovering... (turn " << turns_ << "/2)\n";

  // // 经过两个回合后恢复
  // if (turns_ >= 2) {
  //   enemy_->recoverTenacity(); // 恢复韧性

  //   Player *player = enemy_->getPlayer();
  //   if (!player) {
  //     std::cout << "目标丢失，继续巡逻" << std::endl;
  //     enemy_->clearPlayer();
  //     enemy_->setState(enemy_->getPatrolState());
  //     return;
  //   }

  //   // 检查状态转换
  //   // checkStateTransition();
  // }
}

void StunnedState::takeDamage(int dmg) {
  std::cout << "Stunned: takeDamage " << dmg << "\n";
  enemy_->setHP(enemy_->getHP() - dmg);
}

// Dead
void DeadState::enter() { std::cout << "Enter Dead\n"; }
void DeadState::exit() {}
void DeadState::update() {}

void DeadState::takeDamage(int dmg) {
  (void)dmg;
  std::cout << "Dead: already dead\n";
}