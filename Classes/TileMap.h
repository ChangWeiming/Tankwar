#pragma once

#include "cocos2d.h"

class TileMap :
	public cocos2d::Layer
{
private:
	/*
	* 0 for brick
	* 1 for steel
	* 2 for grass
	*/
	cocos2d::TMXLayer* mapLayers[3];
	cocos2d::TMXTiledMap* map;
	int round;
public:
	static TileMap* createWithRound(int);
	static TileMap* create();
	bool init() override;
	void setRound(int);
	cocos2d::TMXLayer* getBrickLayer();
	cocos2d::TMXLayer* getSteelLayer();
	cocos2d::Size getTileSize();
};
