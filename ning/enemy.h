#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
// #include "enemy_state.h"
// 敌人状态与敌人类（1x10 线路）
// 地图为 1 x 10 的线路，位置范围 [0,9]

class Enemy; // 提前声明
class Map;
class Player;
class State {
protected:
    Enemy* enemy_;
public:
    State(Enemy* enemy = nullptr) : enemy_(enemy) {}
    virtual ~State() {}
    virtual void setEnemy(Enemy* e) { enemy_ = e; }
    virtual void enter() {}
    virtual void exit() {}
    virtual void takeDamage(int dmg) = 0;
    virtual void update() = 0;
    virtual std::string getName() const = 0;
    void checkStateTransition();
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
    int attackDmg_; // 攻击伤害
    int zhuijiDis_; // 追击距离
    int originalZhuijiDis_; // 原始追击距离
    int attackDis_; // 攻击距离
    int hp_;        //血量
    int maxHp_;     // 最大血量
    int tenacity_; // 韧性（stun 机制）
    int pos_;
    int patrol_a_; // 巡逻端点A
    int patrol_b_; // 巡逻端点B
    int patrol_dir_; // 当前巡逻方向，-1 或 +1
    State* current_;
    PatrolState* patrolState_;//巡逻
    ChaseState* chaseState_;//追击
    AttackState* attackState_;//攻击
    StunnedState* stunnedState_;//瘫痪
    DeadState* deadState_;//死亡
    Map* map_; // 地图指针
    Player* player_; // 玩家指针
    bool isEnraged_; // 是否处于狂暴状态
    int shield_; // 护盾值
    bool hasBlinkStrike_; // 是否有瞬砍技能
    int attackCount_; // 攻击计数器
public:
    // patrol_a/patrol_b 为巡逻端点（0..9），tenacity 为初始韧性
    Enemy(int attackDmg = 1, int zhuijiDis = 2, int attackDis = 1, int hp = 10, int pos = 0, int patrol_a = 0, int patrol_b = 3, int tenacity = 5);
    ~Enemy();
    
    // 设置地图
    void setMap(Map* map);
    // 设置玩家目标
    void setPlayer(Player* player);
    Player* getPlayer() const;
    void clearPlayer();
    // 状态管理
    void setState(State* s);
    State* getCurrentState() const;
    State* getPatrolState() const;
    State* getChaseState() const;
    State* getAttackState() const;
    State* getStunnedState() const;
    State* getDeadState() const;
    // 追击距离
    int getZhuijiDis() const;
    // 攻击距离
    int getAttackDis() const;
    int setZhuijiDis(int zhuijiDis);
    int setAttackDis(int attackDis);
    // 攻击伤害
    int getAttackDmg() const;
    void setAttackDmg(int attackDmg);
    // 对外动作

    void takeDamage(int dmg);
    void update();



    // tenacity 操作
    int getTenacity() const;
    void changeTenacity(int delta);

    // 属性访问
    int getHP() const;
    int getPos() const;
    void setPos(int p);
    void setHP(int hp);
    
    void setPatrolPoints(int a, int b);
    void setPatrolDir(int d);
    // 供状态访问的安全访问器
    int getPatrolA() const;
    int setPatrolA(int a);
    int getPatrolB() const;
    int setPatrolB(int b);
    int getPatrolDir() const;
    Map* getMap() const;
    void recoverTenacity();
    
    // 狂暴状态相关方法
    bool isEnraged() const { return isEnraged_; }
    bool hasBlinkStrike() const { return hasBlinkStrike_; }
    void useBlinkStrike() { hasBlinkStrike_ = false; }
    int getShield() const { return shield_; }
    int getMaxHp() const { return maxHp_; }
    
    // 攻击计数器相关方法
    void incrementAttackCount() { attackCount_++; }
    int getAttackCount() const { return attackCount_; }
};