#pragma once
#include "Player.h"
#include "enemy.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>
class Enemy; // 提前声明

// 对象类型枚举
enum class ObjectType { NONE, PLAYER, ENEMY };

// 位置对象结构体
struct PositionObject {
  ObjectType type;
  void *object; // 指向玩家或敌人的指针

  PositionObject() : type(ObjectType::NONE), object(nullptr) {}
  PositionObject(ObjectType t, void *obj) : type(t), object(obj) {}
};

// 地图类：管理玩家和敌人的位置
class Map {
private:
 
  std::vector<Enemy *> enemies_; // 敌人列表
  const int left_;               // 地图左边界
  const int right_;              // 地图右边界
  std::unordered_map<int, PositionObject>
      positionMap_; // 位置映射表，存储有对象的位置
 Player *player_;               // 玩家对象
public:
  Map(int width = 10) : left_(0), right_(width - 1), player_(nullptr) {}

  // 设置玩家位置
  void setPlayer(Player *player) {
    if (player != nullptr) {
      // 清除旧位置
      if (player_) {
        positionMap_.erase(player_->getPos());
      }

      player_ = player;
      // 更新新位置
      if (isValidPos(player->getPos())) {
        positionMap_[player->getPos()] = 
            PositionObject(ObjectType::PLAYER, player);
      }
      player_->setMap(this);
    }
  }

  // 更新玩家位置
  void updatePlayerPosition(int oldPos) {
    if (player_) {
      // 清除旧位置
      positionMap_.erase(oldPos);
      // 设置新位置
      if (isValidPos(player_->getPos())) {
        positionMap_[player_->getPos()] =
            PositionObject(ObjectType::PLAYER, player_);
      }
    }
  }
  // 获取玩家
  Player *getPlayer() const { return player_; }
  // 获取玩家位置
  int getPlayerPos() const { 
    if (player_) {
      return player_->getPos(); 
    }
    return -1; // 返回无效位置
  }

  // 添加敌人
  void addEnemy(Enemy *enemy) {
    enemies_.push_back(enemy);
    enemy->setMap(this);
    // 更新位置映射
    if (isValidPos(enemy->getPos())) {
      positionMap_[enemy->getPos()] = PositionObject(ObjectType::ENEMY, enemy);
    }
  }

  // 移除敌人
  void removeEnemy(Enemy *enemy) {
    auto it = std::find(enemies_.begin(), enemies_.end(), enemy);
    if (it != enemies_.end()) {
      // 清除位置映射
      positionMap_.erase(enemy->getPos());
      enemies_.erase(it);
    }
  }

  // 更新敌人位置
  void updateEnemyPosition(Enemy *enemy, int oldPos) {
    // 清除旧位置
    positionMap_.erase(oldPos);
    // 设置新位置
    if (isValidPos(enemy->getPos())) {
      positionMap_[enemy->getPos()] = PositionObject(ObjectType::ENEMY, enemy);
    }
  }

  // 获取敌人列表
  const std::vector<Enemy *> &getEnemies() const { return enemies_; }
  int getLeft() const { return left_; }
  int getRight() const { return right_; }
  int getWidth() const { return right_ - left_ + 1; }

  // 检查位置是否有效
  bool isValidPos(int pos) const { return pos >= left_ && pos <= right_; }
  // 通过位置获取对象
  PositionObject getObjectAt(int pos) const {
    auto it = positionMap_.find(pos);
    if (it != positionMap_.end()) {
      return it->second;
    }
    return PositionObject();
  }

  // 检测特定距离内的玩家，没找到返回nullptr
  Player *detectPlayer(int centerPos, int distance) const {
    // 计算检测范围
    int startPos = std::max(left_, centerPos - distance);
    int endPos = std::min(right_, centerPos + distance);

    // 只检查范围内的位置
    for (int pos = startPos; pos <= endPos; pos++) {
      auto it = positionMap_.find(pos);
      if (it != positionMap_.end() && it->second.type == ObjectType::PLAYER) {
        return static_cast<Player *>(it->second.object);
      }
    }
    return nullptr; // 没找到玩家
  }

  // 检测特定距离内的敌人，没找到返回nullptr
  Enemy *detectEnemy(int centerPos, int distance,
                     Enemy *excludeEnemy = nullptr) const {
    // 计算检测范围
    int startPos = std::max(left_, centerPos - distance);
    int endPos = std::min(right_, centerPos + distance);

    // 只检查范围内的位置
    for (int pos = startPos; pos <= endPos; pos++) {
      auto it = positionMap_.find(pos);
      if (it != positionMap_.end() && it->second.type == ObjectType::ENEMY) {
        Enemy *enemy = static_cast<Enemy *>(it->second.object);
        if (enemy != excludeEnemy) {
          return enemy;
        }
      }
    }
    return nullptr; // 没找到敌人
  }

  // 检测特定距离内的所有敌人
  std::vector<Enemy *> detectAllEnemies(int centerPos, int distance,
                                        Enemy *excludeEnemy = nullptr) const {
    std::vector<Enemy *> result;
    // 计算检测范围
    int startPos = std::max(left_, centerPos - distance);
    int endPos = std::min(right_, centerPos + distance);

    // 只检查范围内的位置
    for (int pos = startPos; pos <= endPos; pos++) {
      auto it = positionMap_.find(pos);
      if (it != positionMap_.end() && it->second.type == ObjectType::ENEMY) {
        Enemy *enemy = static_cast<Enemy *>(it->second.object);
        if (enemy != excludeEnemy) {
          result.push_back(enemy);
        }
      }
    }
    return result;
  }
};