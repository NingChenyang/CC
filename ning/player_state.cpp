#include "player_state.h"
#include "player.h"
#include "enemy.h"
#include "map.hpp"
#include <iostream>



// 基类的move方法实现
void PlayerState::move(int distance) {
  if (!player_)
    return;

  if (distance > Player::MAX_MOVE_DISTANCE) {
    distance = Player::MAX_MOVE_DISTANCE;
  }
  if (distance < -Player::MAX_MOVE_DISTANCE) {
    distance = -Player::MAX_MOVE_DISTANCE;
  }
  int newPos = player_->getPos() + distance;
  // 处理边界情况，将超出边界的位置设置为边界值
  if (newPos < Player::MIN_POS) {
    newPos = Player::MIN_POS;
  }
  if (newPos > Player::MAX_POS) {
    newPos = Player::MAX_POS;
  }
  player_->setPos(newPos);
  std::cout << "Player: 移动了 " << distance
            << " 步，现在位置: " << player_->getPos() << std::endl;
}

// 基类的takeDamage方法实现
void PlayerState::takeDamage(int dmg) {
    if (!player_) return;
    
    player_->setHp(player_->getHp() - dmg);
    std::cout << "Player: 受到 " << dmg
              << " 点伤害，当前血量: " << player_->getHp() << std::endl;
    if (player_->getHp() <= 0) {
        player_->setState(player_->getDeadState());
    }
}

// 状态转换辅助方法
void PlayerState::checkStateTransition() {
    if (!player_) return;
    
    Map *map = player_->getMap();
    Enemy *enemy = player_->getEnemy();
    
    // 如果没有敌人，切换到空闲状态
    if (!enemy) {
        enemy = map->detectEnemy(player_->getPos(), player_->getAlertDistance());
        if (!enemy) {
            if (dynamic_cast<IdleState*>(this) == nullptr) {
                std::cout << "Player: 没有敌人，切换到空闲状态" << std::endl;
                player_->setState(player_->getIdleState());
            }
            return;
        }
        player_->setEnemy(enemy);
    }
    
    int dist = std::abs(enemy->getPos() - player_->getPos());
    
    // 根据距离切换状态
    if (dist > player_->getAlertDistance()) {
        if (dynamic_cast<IdleState*>(this) == nullptr) {
            std::cout << "Player: 敌人超出警戒范围，切换到空闲状态" << std::endl;
            player_->setState(player_->getIdleState());
            player_->clearEnemy();
        }
    } else if (dist <= player_->getEngageDistance()) {
        if (dynamic_cast<EngageState*>(this) == nullptr) {
            std::cout << "Player: 敌人进入交战范围，切换到交战状态" << std::endl;
            player_->setState(player_->getEngageState());
        }
    } else {
        if (dynamic_cast<AlertState*>(this) == nullptr) {
            std::cout << "Player: 敌人进入警戒范围，切换到警戒状态" << std::endl;
            player_->setState(player_->getAlertState());
        }
    }
}

// 空闲状态
void IdleState::enter() { std::cout << "Player: 进入空闲状态" << std::endl; }

void IdleState::exit() { std::cout << "Player: 退出空闲状态" << std::endl; }

void IdleState::attack() {
  std::cout << "Player: 没有敌人，无法攻击" << std::endl;
}

void IdleState::update() {
  checkStateTransition();
}

// 交战状态
void EngageState::enter() { std::cout << "Player: 进入交战状态" << std::endl; }

void EngageState::exit() { std::cout << "Player: 退出交战状态" << std::endl; }

void EngageState::attack() {
  Enemy* enemy = player_->getEnemy();
  if (!enemy) {
    std::cout << "Player: 没有敌人，无法攻击" << std::endl;
    return;
  }
  
  int distance = std::abs(enemy->getPos() - player_->getPos());
  if (distance <= player_->getEngageDistance()) {
    std::cout << "Player: 攻击敌人！" << std::endl;
    enemy->takeDamage(player_->getAttackDamage());
  } else {
    std::cout << "Player: 敌人太远，无法攻击" << std::endl;
  }
}

void EngageState::update() {
  checkStateTransition();
}

// 警戒状态
void AlertState::enter() { std::cout << "Player: 进入警戒状态" << std::endl; }

void AlertState::exit() { std::cout << "Player: 退出警戒状态" << std::endl; }

void AlertState::attack() {
  Enemy* enemy = player_->getEnemy();
  if (!enemy) {
    std::cout << "Player: 没有敌人，无法攻击" << std::endl;
    return;
  }
  
  int distance = std::abs(enemy->getPos() - player_->getPos());
  if (distance <= player_->getEngageDistance()) {
    std::cout << "Player: 攻击敌人！" << std::endl;
    enemy->takeDamage(player_->getAttackDamage());
  } else {
    std::cout << "Player: 敌人太远，无法攻击" << std::endl;
  }
}

void AlertState::update() {
  checkStateTransition();
}

// 死亡状态
void PlayerDeadState::enter() {
  std::cout << "Player: 进入死亡状态" << std::endl;
}

void PlayerDeadState::exit() {
  std::cout << "Player: 退出死亡状态" << std::endl;
}

void PlayerDeadState::move(int distance) {
  std::cout << "Player: 已死亡，无法移动" << std::endl;
}

void PlayerDeadState::attack() {
  std::cout << "Player: 已死亡，无法攻击" << std::endl;
}

void PlayerDeadState::takeDamage(int dmg) {
  std::cout << "Player: 已死亡，无法再受到伤害" << std::endl;
}

void PlayerDeadState::update() {
  // 死亡状态不需要更新
}