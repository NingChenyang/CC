#pragma once
#include <iostream>

// 敌人状态与敌人类（1x10 线路）
// 地图为 1 x 10 的线路，位置范围 [0,9]

class Enemy; // 提前声明

class State {
public:
    virtual ~State() {}
    virtual void enter(Enemy* /*e*/) {}
    virtual void exit(Enemy* /*e*/) {}
    // dir 为 -1 表示左，+1 表示右
    virtual void move(Enemy* e, int dir) = 0;
    virtual void attack(Enemy* e) = 0;
    virtual void takeDamage(Enemy* e, int dmg) = 0;
    virtual void update(Enemy* e) = 0;
};

// 前置声明具体状态类
class PatrolState;
class ChaseState;
class AttackState;
class StunnedState;
class DeadState;

class Enemy {
public:
    static const int MIN_POS = 0;
    static const int MAX_POS = 9;

private:
    int hp_;
    int tenacity_; // 韧性（stun 机制）
    int pos_;
    int player_pos_; // 玩家当前位置（线路上的索引）
    int patrol_a_; // 巡逻端点A
    int patrol_b_; // 巡逻端点B
    int patrol_dir_; // 当前巡逻方向，-1 或 +1
    State* current_;
    PatrolState* patrolState_;
    ChaseState* chaseState_;
    AttackState* attackState_;
    StunnedState* stunnedState_;
    DeadState* deadState_;

public:
    // patrol_a/patrol_b 为巡逻端点（0..9），tenacity 为初始韧性
    Enemy(int hp = 10, int pos = 0, int patrol_a = 0, int patrol_b = 3, int tenacity = 5);
    ~Enemy();

    // 状态管理
    void setState(State* s);
    State* getPatrolState() const;
    State* getChaseState() const;
    State* getAttackState() const;
    State* getStunnedState() const;
    State* getDeadState() const;

    // 对外动作
    // dir: -1 left, +1 right
    void move(int dir);
    void attack();
    void takeDamage(int dmg);
    void update();

    // 设置玩家位置（用于 Chase/Attack 决策）
    void setPlayerPos(int p);

    // tenacity 操作
    int getTenacity() const;
    void changeTenacity(int delta);

    // 属性访问
    int getHP() const;
    int getPos() const;
    void setPos(int p);
    void setHP(int hp);
    void setPatrolPoints(int a, int b);
    // 供状态访问的安全访问器
    int getPlayerPos() const;
    int getPatrolA() const;
    int getPatrolB() const;
    int getPatrolDir() const;
    void setPatrolDir(int d);
};


    

