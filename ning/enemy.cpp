#include "enemy.h"

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

// Patrol: 在 patrol_a_ 和 patrol_b_ 之间来回
void PatrolState::enter(Enemy* e) { std::cout<<"Enter Patrol\n"; }
void PatrolState::exit(Enemy* e) { std::cout<<"Exit Patrol\n"; }
void PatrolState::update(Enemy* e) {
    // 发现玩家（在检测范围内） -> 切换 Chase
    if (e->getPlayerPos() >= 0) {
        int dist = std::abs(e->getPlayerPos() - e->getPos());
        if (dist <= 3) { // 视野范围
            std::cout << "Detect player at "<< e->getPlayerPos() <<" (dist="<<dist<<") -> Chase\n";
            e->setState(e->getChaseState());
            return;
        }
    }

    // 巡逻移动一步
    int dir = e->getPatrolDir();
    int next = e->getPos() + dir;
    if (next < e->getPatrolA() || next > e->getPatrolB()) {
        e->setPatrolDir(-dir);
        dir = e->getPatrolDir();
        next = e->getPos() + dir;
    }
    e->setPos(next);
    std::cout << "Patrol: pos -> " << e->getPos() << "\n";
}
void PatrolState::move(Enemy* e, int dir) {
    // 外部强制移动也可视为切换至 Patrol 后移动
    e->setState(e->getPatrolState());
    int next = e->getPos() + dir;
    e->setPos(next);
    std::cout << "Patrol move forced to "<< e->getPos() <<"\n";
}
void PatrolState::attack(Enemy* e) { std::cout<<"Patrol: no attack\n"; }
void PatrolState::takeDamage(Enemy* e, int dmg) {
    std::cout<<"Patrol: takeDamage "<<dmg<<"\n";
    e->setHP(e->getHP() - dmg);
    if (e->getHP() <= 0) return;
    // 受伤可能发现玩家，切换为 Chase
    if (e->getPlayerPos() >= 0) e->setState(e->getChaseState());
}

// Chase: 追击玩家
void ChaseState::enter(Enemy* e) { std::cout<<"Enter Chase\n"; }
void ChaseState::exit(Enemy* e) { std::cout<<"Exit Chase\n"; }
void ChaseState::update(Enemy* e) {
    if (e->getPlayerPos() < 0) { e->setState(e->getPatrolState()); return; }
    int pos = e->getPos();
    int target = e->getPlayerPos();
    int dist = std::abs(target - pos);
    if (dist <= 1) {
        std::cout<<"Chase: in range -> Attack\n";
        e->setState(e->getAttackState());
        return;
    }
    int dir = (target > pos) ? 1 : -1;
    e->setPos(pos + dir);
    std::cout<<"Chase: move to "<< e->getPos() <<" (target="<<target<<")\n";
}
void ChaseState::move(Enemy* e, int dir) { /* not used */ }
void ChaseState::attack(Enemy* e) { std::cout<<"Chase: not in range\n"; }
void ChaseState::takeDamage(Enemy* e, int dmg) {
    std::cout<<"Chase: takeDamage "<<dmg<<"\n";
    e->setHP(e->getHP() - dmg);
}

// Attack: 接近玩家后攻击
void AttackState::enter(Enemy* e) { std::cout<<"Enter Attack\n"; }
void AttackState::exit(Enemy* e) { std::cout<<"Exit Attack\n"; }
void AttackState::update(Enemy* e) {
    std::cout<<"Attack: strike player!\n";
    // 攻击后回到 Chase（继续追击/接触）
    e->setState(e->getChaseState());
}
void AttackState::move(Enemy* e, int dir) { (void)e; (void)dir; }
void AttackState::attack(Enemy* e) { std::cout<<"Attack: direct attack call\n"; }
void AttackState::takeDamage(Enemy* e, int dmg) {
    std::cout<<"Attack: takeDamage "<<dmg<<"\n";
    e->setHP(e->getHP() - dmg);
}

// Stunned: 韧性为 0 时进入，无法行动，短时间后恢复韧性并回到巡逻
void StunnedState::enter(Enemy* e) { std::cout<<"Enter Stunned\n"; }
void StunnedState::exit(Enemy* e) { std::cout<<"Exit Stunned\n"; }
void StunnedState::update(Enemy* e) {
    std::cout<<"Stunned: recovering...\n";
    // 简单恢复逻辑：恢复一定韧性，然后回到 Patrol
    e->changeTenacity(3); // 恢复 3 点
    e->setState(e->getPatrolState());
}
void StunnedState::move(Enemy* e, int dir) { (void)e; (void)dir; std::cout<<"Stunned: cannot move\n"; }
void StunnedState::attack(Enemy* e) { (void)e; std::cout<<"Stunned: cannot attack\n"; }
void StunnedState::takeDamage(Enemy* e, int dmg) { std::cout<<"Stunned: takeDamage "<<dmg<<"\n"; e->setHP(e->getHP()-dmg); }

// Dead
void DeadState::enter(Enemy* e) { std::cout<<"Enter Dead\n"; }
void DeadState::exit(Enemy* e) { (void)e; }
void DeadState::update(Enemy* e) { (void)e; }
void DeadState::move(Enemy* e, int dir) { (void)e; (void)dir; std::cout<<"Dead: cannot move\n"; }
void DeadState::attack(Enemy* e) { (void)e; std::cout<<"Dead: cannot attack\n"; }
void DeadState::takeDamage(Enemy* e, int dmg) { (void)e; (void)dmg; std::cout<<"Dead: already dead\n"; }


