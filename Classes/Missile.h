#pragma once

#include "cocos2d.h"
#include "Constant.h"
#include "TileMap.h"

class Missile :
	public cocos2d::Sprite
{
private:
	TankState::TankType type;
	TankState::Direction face;
	cocos2d::Rect rect;
	bool isBooming;
	TileMap *map;
public:

	Missile();
	Missile(TankState::TankType);
	virtual ~Missile();
	virtual bool init();
	virtual void getNextState(float);
	void setType(TankState::TankType);
	void setFace(TankState::Direction);
	void setPositionAndRect(const cocos2d::Vec2&);
	void setInvisible();
	void setMap(TileMap*);
	bool collisionTest(cocos2d::Vec2&);
	void goBoom();
	bool getBoomingState() const;
	cocos2d::Rect getRect() const;
	static Missile* create(){
		Missile *pRet = new(std::nothrow) Missile();
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
	static Missile* createWithType(TankState::TankType temp) {
		Missile *pRet = new(std::nothrow) Missile(temp);
		if (pRet && pRet->init()) {
			pRet->autorelease();
			return pRet;
		}
		else {
			delete pRet;
			pRet = nullptr;
			return nullptr;
		}
	}
};
