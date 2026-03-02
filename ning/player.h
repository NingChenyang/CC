#pragma once
#include "player_state.h"

class Enemy; // 提前声明
class Map; // 提前声明

class Player
{
public:
    static const int MIN_POS = 0;
    static const int MAX_POS = 9;
    static const int MAX_MOVE_DISTANCE = 2;
    static const int ENGAGE_DISTANCE = 1;// 交战距离
    static const int ALERT_DISTANCE = 3;// 警戒距离

public:
    Player(int pos = -1, int hp = 100, int engageDistance = ENGAGE_DISTANCE, int alertDistance = ALERT_DISTANCE, int attackDamage = 10);
    ~Player();
    int pos_;
    int hp_;
    // 对外动作
    void move(int distance);
    void attack();
    void takeDamage(int dmg);
    void update(char moveCommand);
    // 对外查询
    int getPos() const;
    int getHp() const;
    // 设置方法
    void setPos(int pos);
    void setHp(int hp);
    void setEnemy(Enemy* enemy);
    Enemy* getEnemy() const;
    void clearEnemy();
    // 距离阈值
    int getEngageDistance() const;
    int getAlertDistance() const;
    int setEngageDistance(int dist);
    int setAlertDistance(int dist);

    // 攻击伤害
    int getAttackDamage() const;
    int setAttackDamage(int dmg);
    // 地图
    void setMap(Map *map);
    Map *getMap() const;



    // 状态管理
    void setState(PlayerState* s);
    PlayerState* getCurrentState() const;
    PlayerState* getIdleState() const;
    PlayerState* getEngageState() const;
    PlayerState* getAlertState() const;
    PlayerState* getDeadState() const;
    // 状态检查
    bool isDead() const;
private:
    PlayerState* current_;
    IdleState* idleState_;
    EngageState* engageState_;
    AlertState* alertState_;
    PlayerDeadState* deadState_;
    
    int engageDistance_;
    int alertDistance_;
    int attackDamage_;
    Enemy* enemy_;
    Map *map_;
};