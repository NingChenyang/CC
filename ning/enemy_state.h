#pragma once
#include "enemy.h"
#include "map.hpp"
class Enemy; // 提前声明
// 具体状态声明（在实现文件中定义行为）
class PatrolState : public State {
public:
    PatrolState(Enemy* enemy) : State(enemy) {}
    void enter() override;
    void exit() override;
    void move(int dir) override;
    void attack() override;
    void takeDamage(int dmg) override;
    void update() override;
};

class ChaseState : public State {
public:
    ChaseState(Enemy* enemy) : State(enemy) {}
    void enter() override;
    void exit() override;
    void move(int dir) override;
    void attack() override;
    void takeDamage(int dmg) override;
    void update() override;
};

class AttackState : public State {
public:
    AttackState(Enemy* enemy) : State(enemy) {}
    void enter() override;
    void exit() override;
    void move(int dir) override;
    void attack() override;
    void takeDamage(int dmg) override;
    void update() override;
};

class StunnedState : public State {
private:
    int turns_; // 瘫痪回合计数器
public:
    StunnedState(Enemy* enemy) : State(enemy), turns_(0) {}
    void enter() override;
    void exit() override;
    void move(int dir) override;
    void attack() override;
    void takeDamage(int dmg) override;
    void update() override;
};

class DeadState : public State {
public:
    DeadState(Enemy* enemy) : State(enemy) {}
    void enter() override;
    void exit() override;
    void move(int dir) override;
    void attack() override;
    void takeDamage(int dmg) override;
    void update() override;
};