#include "enemy.h"
#include "enemy_state.h"

// 构造函数
Enemy::Enemy(int attackDmg, int zhuijiDis, int attackDis, int hp, int pos, int patrol_a, int patrol_b, int tenacity)
    : attackDmg_(attackDmg), zhuijiDis_(zhuijiDis), attackDis_(attackDis), hp_(hp), tenacity_(tenacity), pos_(pos), 
      patrol_a_(patrol_a), patrol_b_(patrol_b), patrol_dir_(1), player_(nullptr) {
    // 初始化状态
    patrolState_ = new PatrolState(this);
    chaseState_ = new ChaseState(this);
    attackState_ = new AttackState(this);
    stunnedState_ = new StunnedState(this);
    deadState_ = new DeadState(this);
    map_ = nullptr;
    // 默认状态为巡逻
    current_ = patrolState_;
    current_->enter();
}

// 析构函数
Enemy::~Enemy() {
    delete patrolState_;
    delete chaseState_;
    delete attackState_;
    delete stunnedState_;
    delete deadState_;
}

// 设置地图
void Enemy::setMap(Map* map) {
    map_ = map;
}

// 获取地图
Map* Enemy::getMap() const {
    return map_;
}
// 设置玩家目标
void Enemy::setPlayer(Player* player) {
    player_ = player;
}

// 获取玩家目标
Player* Enemy::getPlayer() const {
    return player_;
}

// 清除玩家目标
void Enemy::clearPlayer() {
    player_ = nullptr;
}



// 状态管理
void Enemy::setState(State* s) {
    if (current_) {
        current_->exit();
    }
    current_ = s;
    // current_->setEnemy(this);
    current_->enter();
}

State* Enemy::getPatrolState() const { return patrolState_; }
State* Enemy::getChaseState() const { return chaseState_; }
State* Enemy::getAttackState() const { return attackState_; }
State* Enemy::getStunnedState() const { return stunnedState_; }
State* Enemy::getDeadState() const { return deadState_; }

// 对外动作
void Enemy::move(int dir) {
    current_->move(dir);
}

void Enemy::attack() {
    current_->attack();
}

void Enemy::takeDamage(int dmg) {
    current_->takeDamage(dmg);
    // if (hp_ <= 0) {
    //     setState(deadState_);
    // }
}

void Enemy::update() {
    current_->update();
}

// tenacity 操作
int Enemy::getTenacity() const { return tenacity_; }
void Enemy::changeTenacity(int delta) {
    tenacity_ += delta;
    // if (tenacity_ <= 0) {
    //     setState(stunnedState_);
    // }
}

// 属性访问
int Enemy::getHP() const { return hp_; }
int Enemy::getPos() const { return pos_; }
void Enemy::setPos(int p) {
    if (p >= MIN_POS && p <= MAX_POS) {
        int oldPos = pos_; // 保存旧位置
        pos_ = p;
        // 通知地图更新位置
        if (map_) {
            map_->updateEnemyPosition(this, oldPos);
        }
    }
    
}
void Enemy::setHP(int hp) { hp_ = hp; }
void Enemy::setPatrolPoints(int a, int b) {
    patrol_a_ = a;
    patrol_b_ = b;
}
void Enemy::setPatrolDir(int d) {
    patrol_dir_ = (d > 0) ? 1 : -1;
}

// 供状态访问的安全访问器
int Enemy::getPatrolA() const { return patrol_a_; }
int Enemy::setPatrolA(int a) { patrol_a_ = a; return patrol_a_; }
int Enemy::getPatrolB() const { return patrol_b_; }
int Enemy::setPatrolB(int b) { patrol_b_ = b; return patrol_b_; }
int Enemy::getPatrolDir() const { return patrol_dir_; }


int Enemy::getZhuijiDis() const { return zhuijiDis_; }
int Enemy::getAttackDis() const { return attackDis_; }
int Enemy::setZhuijiDis(int zhuijiDis) { zhuijiDis_ = zhuijiDis; return zhuijiDis_; }
int Enemy::setAttackDis(int attackDis) { attackDis_ = attackDis; return attackDis_; }
// 攻击伤害
int Enemy::getAttackDmg() const { return attackDmg_; }
void Enemy::setAttackDmg(int attackDmg) { attackDmg_ = attackDmg; }
void Enemy::recoverTenacity() {
    changeTenacity(5); // 恢复 5 点韧性
}