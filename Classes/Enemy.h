#pragma once
#include "cocos2d.h"
#include "EnemyTank.h"
#include "Tank.h"
#include <vector>
#include <iostream>

class Missile;

struct MissileState {
	Missile* pMissile;
	bool used;
	MissileState(Missile* _pMissile = nullptr, bool _used = false) {
		pMissile = _pMissile;
		used = _used;
	}
};

class Enemy :
	public cocos2d::Layer
{
private:
	int enemyNums;
	int enemyDestoryed;
	Tank* playerTank;
	cocos2d::Vec2 startPoint[3];
	cocos2d::Array* tank;
	std::vector<MissileState> enemyMissile;
	Missile* getUnusedMissile();
	void setMissileUsed(Missile*);
	TileMap* map;
public:
	Enemy();
	void setPlayerTankPointer(Tank*);
	int getDestoryedNum();
	virtual ~Enemy();
	void setEnemyNums(int);
	bool init() override;
	CREATE_FUNC(Enemy);
	bool isOver();
	static Enemy* createWithEnemyNums(int );
	virtual void update(float) override;
	void setMap(TileMap* map);
	bool collisionTest(const cocos2d::Rect&);
	bool born(double);
	std::vector<cocos2d::Rect> getTankAlivePos();
};

