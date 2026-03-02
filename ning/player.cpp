#include "player.h"
#include "player_state.h"
#include "map.hpp"
#include <iostream>

// 构造函数
Player::Player(int pos, int hp, int engageDistance, int alertDistance, int attackDamage) : pos_(pos), hp_(hp), engageDistance_(engageDistance), alertDistance_(alertDistance), attackDamage_(attackDamage), enemy_(nullptr) {
    // 初始化状态
    idleState_ = new IdleState(this);
    engageState_ = new EngageState(this);
    alertState_ = new AlertState(this);
    deadState_ = new PlayerDeadState(this);
    
    // 默认状态为空闲
    current_ = idleState_;
    current_->setPlayer(this);
    current_->enter();
}

// 析构函数
Player::~Player() {
    delete idleState_;
    delete engageState_;
    delete alertState_;
    delete deadState_;
}

// 移动方法
void Player::move(int distance) {
    current_->move(distance);
}

// 攻击方法
void Player::attack() {
    current_->attack();
}

// 受伤方法
void Player::takeDamage(int dmg) {
    current_->takeDamage(dmg);
    if (hp_ <= 0) {
        setState(deadState_);
    }
}

// 更新方法
void Player::update(char moveCommand) {
    // 根据移动指令调用move方法
    switch (moveCommand) {
        case 'a': // 左移动一格
            move(-1);
            break;
        case 'd': // 右移动一格
            move(1);
            break;
        case 's': // 不动
            // 不移动
            break;
        case 'q': // 左移动两格
            move(-2);
            break;
        case 'e': // 右移动两格
            move(2);
            break;
        case 'f': // 攻击敌人
            attack();
            break;
        default:
            // 无效指令，不移动
            break;
    }
    
    current_->update();
}

// 设置位置
void Player::setPos(int pos) {
    if (pos >= MIN_POS && pos <= MAX_POS) {
        int oldPos = pos_; // 保存旧位置
        pos_ = pos;
        std::cout << "Player position set to: " << pos_ << std::endl;
        // 通知地图更新位置
        if (map_) {
            map_->updatePlayerPosition(oldPos);
        }
    }
}

// 设置血量
void Player::setHp(int hp) {
    hp_ = hp;
    if (hp_ < 0) hp_ = 0;
    std::cout << "Player HP set to: " << hp_ << std::endl;
}

// 设置敌人
void Player::setEnemy(Enemy* enemy) {
    enemy_ = enemy;
}

// 获取敌人
Enemy* Player::getEnemy() const {
    return enemy_;
}

// 状态管理
void Player::setState(PlayerState* s) {
    if (current_) {
        current_->exit();
    }
    current_ = s;
    current_->enter();
}

PlayerState* Player::getIdleState() const { return idleState_; }
PlayerState* Player::getEngageState() const { return engageState_; }
PlayerState* Player::getAlertState() const { return alertState_; }
PlayerState* Player::getDeadState() const { return deadState_; }

// 检查是否死亡
bool Player::isDead() const {
    return hp_ <= 0;
}