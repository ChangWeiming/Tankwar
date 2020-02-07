#pragma once

#include "cocos2d.h"

namespace TankState{
	const enum TankType { EnemyTank, PlayerTank };
	const enum Direction {UP = 0,DOWN,LEFT,RIGHT,NONE};
	const enum EnemyTankType { Normal = 0, Fast, HighHP };
	const float TankSpeed = 180;
	const float MissileSpeed = 360;
};

class GameState;

static GameState* sharedState;

class GameState {
private:
	cocos2d::Rect _baseRect;
	cocos2d::Sprite* _base;
	bool _isBaseDestoryed;
public:


	static GameState* getInstance() {
		return sharedState;
	}

	void setDestoryed(bool _state) {
		_isBaseDestoryed = _state;
		if (_state) {
			_base->setTexture("destory.png");
		}
	}
	cocos2d::Rect getBaseRect() {
		return _baseRect;
	}
	cocos2d::Sprite* getBase() {
		return _base;
	}
	bool isDestoryed() {
		return _isBaseDestoryed;
	}
	void init() {
		_baseRect = cocos2d::Rect(360, 0, 60, 60);
		_isBaseDestoryed = false;
		_base = cocos2d::Sprite::create();
		_base->setTexture("Base.png");
		_base->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
		_base->setPosition(cocos2d::Vec2(390, 30));
		sharedState = this;
	}
	static GameState* create() {
		auto pRet = new(std::nothrow) GameState;
		if (pRet == nullptr) return nullptr;
		pRet->init();
		return pRet;
	}
};

const cocos2d::Vec2 gameSize = cocos2d::Vec2(779, 779);

const float BorningFrameGap = 0.125;
