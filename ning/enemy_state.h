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
    void takeDamage(int dmg) override;
    void update() override;
    std::string getName() const override { return "Patrol"; }
};

class ChaseState : public State {
public:
    ChaseState(Enemy* enemy) : State(enemy) {}
    void enter() override;
    void exit() override;
    void takeDamage(int dmg) override;
    void update() override;
    std::string getName() const override { return "Chase"; }
};

class AttackState : public State {
public:
    AttackState(Enemy* enemy) : State(enemy) {}
    void enter() override;
    void exit() override;
    void takeDamage(int dmg) override;
    void update() override;
    std::string getName() const override { return "Attack"; }
};

class StunnedState : public State {
private:
    int turns_; // 瘫痪回合计数器
public:
    StunnedState(Enemy* enemy) : State(enemy), turns_(0) {}
    void enter() override;
    void exit() override;
    void takeDamage(int dmg) override;
    void update() override;
    int getTurns() const { return turns_; }
    void setTurns(int t) { turns_ = t; }
    std::string getName() const override { return "Stunned"; }
};

class DeadState : public State {
public:
    DeadState(Enemy* enemy) : State(enemy) {}
    void enter() override;
    void exit() override;
    void takeDamage(int dmg) override;
    void update() override;
    std::string getName() const override { return "Dead"; }
};