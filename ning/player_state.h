#pragma once
// #include "Player.h"
#include "enemy.h"

class Player;

// 玩家状态基类
class PlayerState {
protected:
    Player* player_;
public:
    PlayerState(Player* player = nullptr) : player_(player) {}
    virtual ~PlayerState() {}
    virtual void setPlayer(Player* p) { player_ = p; }
    virtual void enter()=0;
    virtual void exit()=0;
    virtual void move(int distance);
    virtual void attack() = 0;
    virtual void takeDamage(int dmg);
    virtual void update() = 0;
    // 状态转换辅助方法
    void checkStateTransition();
};

// 前置声明具体状态类
class IdleState;
class EngageState;
class AlertState;
class PlayerDeadState;

// 空闲状态：未遇敌
class IdleState : public PlayerState {
public:
    IdleState(Player* player) : PlayerState(player) {}
    void enter() override;
    void exit() override;
    void attack() override;
    void update() override;
};

// 交战状态：遇敌距离为1
class EngageState : public PlayerState {
public:
    EngageState(Player* player) : PlayerState(player) {}
    void enter() override;
    void exit() override;
    void attack() override;
    void update() override;
};

// 警戒状态：遇敌距离为3
class AlertState : public PlayerState {
public:
    AlertState(Player* player) : PlayerState(player) {}
    void enter() override;
    void exit() override;
    void attack() override;
    void update() override;
};

// 死亡状态
class PlayerDeadState : public PlayerState {
public:
    PlayerDeadState(Player* player) : PlayerState(player) {}
    void enter() override;
    void exit() override;
    void move(int distance) override;
    void attack() override;
    void takeDamage(int dmg) override;
    void update() override;
};