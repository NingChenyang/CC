#include "enemy.h"
#include "enemy_state.h"
#include <algorithm>
#include <cmath>

// define static consts (ODR-use)
const int Enemy::MIN_POS;
const int Enemy::MAX_POS;

// helper
static void clampPos(int &p) {
    if (p < Enemy::MIN_POS) p = Enemy::MIN_POS;
    if (p > Enemy::MAX_POS) p = Enemy::MAX_POS;
}

// ===== Enemy 实现 =====
Enemy::Enemy(int hp, int pos, int patrol_a, int patrol_b, int tenacity)
    : hp_(hp), tenacity_(tenacity), pos_(pos), player_pos_(-1), patrol_a_(patrol_a), patrol_b_(patrol_b), patrol_dir_(1), current_(nullptr)
{
    clampPos(pos_);
    clampPos(patrol_a_);
    clampPos(patrol_b_);
    if (patrol_a_ > patrol_b_) std::swap(patrol_a_, patrol_b_);

    patrolState_ = new PatrolState();
    chaseState_ = new ChaseState();
    attackState_ = new AttackState();
    stunnedState_ = new StunnedState();
    deadState_ = new DeadState();

    current_ = patrolState_;
    current_->enter(this);
}

Enemy::~Enemy() {
    delete patrolState_;
    delete chaseState_;
    delete attackState_;
    delete stunnedState_;
    delete deadState_;
}

void Enemy::setState(State* s) {
    if (current_ == s) return;
    if (current_) current_->exit(this);
    current_ = s;
    if (current_) current_->enter(this);
}

State* Enemy::getPatrolState() const { return patrolState_; }
State* Enemy::getChaseState() const { return chaseState_; }
State* Enemy::getAttackState() const { return attackState_; }
State* Enemy::getStunnedState() const { return stunnedState_; }
State* Enemy::getDeadState() const { return deadState_; }

void Enemy::move(int dir) { if(current_) current_->move(this, dir); }
void Enemy::attack() { if(current_) current_->attack(this); }
void Enemy::takeDamage(int dmg) { if(current_) current_->takeDamage(this, dmg); }
void Enemy::update() { if(current_) current_->update(this); }

void Enemy::setPlayerPos(int p) { player_pos_ = p; clampPos(player_pos_); }

int Enemy::getPlayerPos() const { return player_pos_; }
int Enemy::getPatrolA() const { return patrol_a_; }
int Enemy::getPatrolB() const { return patrol_b_; }
int Enemy::getPatrolDir() const { return patrol_dir_; }
void Enemy::setPatrolDir(int d) { patrol_dir_ = d; }

int Enemy::getTenacity() const { return tenacity_; }
void Enemy::changeTenacity(int delta) {
    tenacity_ += delta;
    std::cout << "tenacity=" << tenacity_ << "\n";
    if (tenacity_ <= 0) {
        std::cout << "Becomes stunned due to tenacity depletion\n";
        setState(getStunnedState());
    }
}

int Enemy::getHP() const { return hp_; }
int Enemy::getPos() const { return pos_; }
void Enemy::setPos(int p) { pos_ = p; clampPos(pos_); }
void Enemy::setHP(int hp) {
    hp_ = hp;
    if (hp_ <= 0) setState(getDeadState());
}

void Enemy::setPatrolPoints(int a, int b) {
    patrol_a_ = std::max(MIN_POS, std::min(MAX_POS, a));
    patrol_b_ = std::max(MIN_POS, std::min(MAX_POS, b));
    if (patrol_a_ > patrol_b_) std::swap(patrol_a_, patrol_b_);
}

// ======== 状态实现 ========



