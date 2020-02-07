#pragma once
#include "cocos2d.h"
#include "TileMap.h"
#include "Tank.h"
#include "Enemy.h"
#include "StateBoard.h"

class GameUI :
	public  cocos2d::Scene
{
private:
	TileMap* map;
	Tank* playerTank;
	Enemy* enemy;
	StateBoard* scoreBoard;
public:
	virtual bool init() override;
	static GameUI* scene();
	virtual void update(float dt);
	GameUI();
	~GameUI();
	CREATE_FUNC(GameUI);
};
