#pragma once

// #include "map.hpp"
#include <string>
class Enemy; // 提前声明
class State {
protected:
    Enemy* enemy_;
public:
    State(Enemy* enemy = nullptr);
    virtual ~State();
    virtual void setEnemy(Enemy* e);
    virtual void enter();
    virtual void exit();
    virtual void takeDamage(int dmg) = 0;
    virtual void update() = 0;
    virtual std::string getName() const = 0;
    void checkStateTransition();
};
// 具体状态声明（在实现文件中定义行为）
class PatrolState : public State {
public:
    PatrolState(Enemy* enemy);
    void enter() override;
    void exit() override;
    void takeDamage(int dmg) override;
    void update() override;
    std::string getName() const override;
};

class ChaseState : public State {
public:
    ChaseState(Enemy* enemy);
    void enter() override;
    void exit() override;
    void takeDamage(int dmg) override;
    void update() override;
    std::string getName() const override;
};

class AttackState : public State {
public:
    AttackState(Enemy* enemy);
    void enter() override;
    void exit() override;
    void takeDamage(int dmg) override;
    void update() override;
    std::string getName() const override;
};

class StunnedState : public State {
private:
    int turns_; // 瘫痪回合计数器
public:
    StunnedState(Enemy* enemy);
    void enter() override;
    void exit() override;
    void takeDamage(int dmg) override;
    void update() override;
    int getTurns() const;
    void setTurns(int t);
    std::string getName() const override;
};

class DeadState : public State {
public:
    DeadState(Enemy* enemy);
    void enter() override;
    void exit() override;
    void takeDamage(int dmg) override;
    void update() override;
    std::string getName() const override;
};