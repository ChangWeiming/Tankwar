#pragma once

#include "cocos2d.h"
#include "Missile.h"
#include "Constant.h"
#include <string>
#include "TileMap.h"
#include "tank.h"

class EnemyTank :
	public cocos2d::Sprite
{
private:
	Missile* pMissile;
	int HP;
	TankState::EnemyTankType type;
	TankState::Direction face;
	std::string typeString;
	Tank* playerTank;
	cocos2d::Array* enemyTanks;
	float shootGap;
	bool isBorning;
	bool isBooming;
	cocos2d::Rect rect;
	TileMap* map;
public:
	EnemyTank();
	void setType(TankState::EnemyTankType);
	virtual bool init() override;
	static EnemyTank* createWithType(TankState::EnemyTankType temp);
	void setPlayerTank(Tank*);
	void setMissile(Missile*);
	void getNextState(float);
	Missile* getMissile() const;
	void setBorningState();
	void setBoomingState();
	void setMap(TileMap*);
	void born(const cocos2d::Vec2&);
	void goBoom();
	cocos2d::Rect getMissileRect() const;
	cocos2d::Rect getRect();
	void setTankPosition(const cocos2d::Vec2&);
	void setEnemyTanksArray(cocos2d::Array*);
	bool isBoarder(cocos2d::Vec2&);
	bool isCollision(cocos2d::Vec2&);
	cocos2d::Vec2 getShootingPosition();
	
};
