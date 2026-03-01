#include "enemy.h"
#include "enemy_state.h"

// 构造函数
Enemy::Enemy(int attackDmg, int zhuijiDis, int attackDis, int hp, int pos, int patrol_a, int patrol_b, int tenacity)
    : attackDmg_(attackDmg), zhuijiDis_(zhuijiDis), originalZhuijiDis_(zhuijiDis), attackDis_(attackDis), 
      hp_(hp), maxHp_(hp), tenacity_(tenacity), pos_(pos), 
      patrol_a_(patrol_a), patrol_b_(patrol_b), patrol_dir_(1), player_(nullptr),
      isEnraged_(false), shield_(0), hasBlinkStrike_(false), attackCount_(0) {
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
    int remainingDamage = dmg;
    
    // 如果有护盾，先消耗护盾值
    if (shield_ > 0) {
        if (dmg <= shield_) {
            // 护盾完全吸收伤害
            shield_ -= dmg;
            remainingDamage = 0;
            std::cout << "敌人的护盾吸收了全部伤害！剩余护盾值: " << shield_ << std::endl;
        } else {
            // 护盾部分吸收伤害
            remainingDamage = dmg - shield_;
            std::cout << "敌人的护盾吸收了 " << shield_ << " 点伤害，剩余伤害: " << remainingDamage << std::endl;
            shield_ = 0;
        }
    }
    
    // 剩余伤害伤害血量
    if (remainingDamage > 0) {
        current_->takeDamage(remainingDamage);
    }
    
    if (hp_ <= 0) {
        setState(deadState_);
    }
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
void Enemy::setHP(int hp) {
    hp_ = hp;
    if (hp_ < 0) hp_ = 0;
    
    // 检查是否触发狂暴状态（血量低于10%）
    if (!isEnraged_ && hp_ <= maxHp_ * 0.1) {
        isEnraged_ = true;
        shield_ = static_cast<int>(maxHp_ * 0.3); // 护盾值为最大血量的30%
        hasBlinkStrike_ = true;
        // 提升追击距离至3格
        zhuijiDis_ = 3;
        std::cout << "\n=== 敌人进入狂暴状态！ ===" << std::endl;
        std::cout << "敌人获得护盾值: " << shield_ << "，追击距离提升至3格，获得瞬砍技能！" << std::endl;
    }
}
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