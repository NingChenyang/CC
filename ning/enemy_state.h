#pragma once
#include "enemy.h"
class Enemy; // 提前声明
// 具体状态声明（在实现文件中定义行为）
class PatrolState : public State {
public:
    PatrolState() {}
    void enter(Enemy* e) override;
    void exit(Enemy* e) override;
    void move(Enemy* e, int dir) override;
    void attack(Enemy* e) override;
    void takeDamage(Enemy* e, int dmg) override;
    void update(Enemy* e) override;
};

class ChaseState : public State {
public:
    ChaseState() {}
    void enter(Enemy* e) override;
    void exit(Enemy* e) override;
    void move(Enemy* e, int dir) override;
    void attack(Enemy* e) override;
    void takeDamage(Enemy* e, int dmg) override;
    void update(Enemy* e) override;
};

class AttackState : public State {
public:
    AttackState() {}
    void enter(Enemy* e) override;
    void exit(Enemy* e) override;
    void move(Enemy* e, int dir) override;
    void attack(Enemy* e) override;
    void takeDamage(Enemy* e, int dmg) override;
    void update(Enemy* e) override;
};

class StunnedState : public State {
public:
    StunnedState() {}
    void enter(Enemy* e) override;
    void exit(Enemy* e) override;
    void move(Enemy* e, int dir) override;
    void attack(Enemy* e) override;
    void takeDamage(Enemy* e, int dmg) override;
    void update(Enemy* e) override;
};

class DeadState : public State {
public:
    DeadState() {}
    void enter(Enemy* e) override;
    void exit(Enemy* e) override;
    void move(Enemy* e, int dir) override;
    void attack(Enemy* e) override;
    void takeDamage(Enemy* e, int dmg) override;
    void update(Enemy* e) override;
};