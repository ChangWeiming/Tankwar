#include "cocos2d.h"
#include "TileMap.h"
#include <vector>
#include <iostream>

using namespace cocos2d;

TileMap* TileMap::createWithRound(int Round) {
	auto pRet = new(std::nothrow) TileMap;
	if (pRet != nullptr) {
		pRet->setRound(Round);
		if (pRet->init() == false) return nullptr;
		return pRet;
	}
	else {
		CCAssert(0, "Debug: unexpected error");
		return nullptr;
	}
}

TileMap* TileMap::create() {
	return TileMap::createWithRound(1);
}

void TileMap::setRound(int r) {
	round = r;
}

bool TileMap::init() {
	if (Layer::init() == false) {
		CCASSERT(false, "Debug:Unexpected error");
		return false;
	}
	char _round[20];
	sprintf(_round, "round%d.tmx", round);
	map = TMXTiledMap::create(_round);
	this->addChild(map,11);
	mapLayers[0] = map->getLayer("BrickWall");
	mapLayers[1] = map->layerNamed("SteelWall");
	mapLayers[2] = map->layerNamed("grass");

	return true;
}

TMXLayer* TileMap::getBrickLayer() {
	return mapLayers[0];
}

TMXLayer* TileMap::getSteelLayer() {
	return mapLayers[1];
}

Size TileMap::getTileSize() {
	return map->getTileSize();
}
