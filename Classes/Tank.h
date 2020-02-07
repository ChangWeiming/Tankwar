#pragma once
#include "cocos2d.h"
#include "Missile.h"
#include "Constant.h"
#include "TileMap.h"
#include <iostream>

class Enemy;

class Tank :
	public cocos2d::Layer
{
private:
	cocos2d::EventKeyboard::KeyCode KeyPressing;
	bool fireCmd;
	int KeyPressingNums;
	cocos2d::Sprite* playerTank;
	cocos2d::Vec2 getNextPosition(const float &, const float &);
	cocos2d::Vec2 startPos;
	Missile* pMissile;
	TankState::Direction face;
	double missileTime;
	Enemy* enemyTank;
	TileMap* map;
	bool isBorning;
	bool isBooming;
public:
	cocos2d::Rect rect;
	Tank();
	Tank(cocos2d::Vec2 StartPos);
	virtual ~Tank();
	virtual bool init();
	static Tank* scene();
	virtual void update(float dt) override;
	bool getTankVisible() const;
	bool getBorningStatus();
	bool getMissileBoomingState() const;
	cocos2d::Vec2 getShootingPosition();
	cocos2d::Rect getRect() const;
	cocos2d::Rect getMissileRect() const;
	TankState::Direction getFace();
	void setEnemyTank(Enemy*);
	void setPositionAndRect(const cocos2d::Vec2&);
	void setMissileInvisible();
	void setTankInvisible();
	void setBooming();
	void setTileMap(TileMap*);
	void createTank();
	void reborn();
	void goBoom();
	bool isCollision(cocos2d::Vec2&);
	static Tank* create() { 
	    Tank *pRet = new(std::nothrow) Tank();
	    if (pRet && pRet->init()) {
	        pRet->autorelease();
	        return pRet;
	    }
		else{
	        delete pRet;
	        pRet = nullptr;
	        return nullptr;
	    }
	}

	static Tank* createWithStartPos(cocos2d::Vec2 StartPos) {
	    Tank *pRet = new(std::nothrow) Tank(StartPos);
	    if (pRet && pRet->init()) {
	        pRet->autorelease();
	        return pRet;
	    }
		else{
	        delete pRet;
	        pRet = nullptr;
	        return nullptr;
	    }
	}

	cocos2d::Vec2 getPlayerTankPos() const {
		return playerTank->getPosition();
	}

};

