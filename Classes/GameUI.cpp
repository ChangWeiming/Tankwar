#include "GameUI.h"
#include "HelloWorldScene.h"
#include "cocos2d.h"
#include "Tank.h"
#include "StateBoard.h"
#include "Enemy.h"
#include <iostream>
#include "TileMap.h"

using namespace cocos2d;

GameUI::GameUI() {}

GameUI::~GameUI() {}

GameUI* GameUI::scene() {
	return GameUI::create();
}

const int EnemyNum[] = { 5,10,15 };

bool GameUI::init() {

	if (!Scene::init()) {
		return false;
	}

	auto _sharedState = GameState::create();

	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("born.plist");
	cache->addSpriteFramesWithFile("blast.plist");
	cache->addSpriteFramesWithFile("missile.plist");
	cache->addSpriteFramesWithFile("PlayerTank1.plist");
	cache->addSpriteFramesWithFile("Enemies.plist");

	this->addChild(_sharedState->getBase());

	auto Size = Director::getInstance()->getVisibleSize();

	map = TileMap::create();
	playerTank = Tank::createWithStartPos(Vec2(60, 60));
	enemy = Enemy::createWithEnemyNums(EnemyNum[0]);

	enemy->setVisible(true);
	enemy->setPlayerTankPointer(playerTank);
	enemy->setMap(map);

	playerTank->setEnemyTank(enemy);
	playerTank->setTileMap(map);

	this->addChild(enemy);
	this->addChild(playerTank);
	this->addChild(map,20);

	scoreBoard = StateBoard::create();

	this->addChild(scoreBoard);

	_scheduler->scheduleUpdate(this,3,false);

	return true;
}


void GameUI::update(float dt) {
	Scene::update(dt);
	static int round = 1;
	static Sprite* gameOver = nullptr;

	if (round == 3) {
		if (gameOver == nullptr) {
			gameOver = Sprite::create();
			gameOver->setTexture("gameover.png");
			auto viewSize = Director::getInstance()->getVisibleSize();
			gameOver->setPosition(viewSize / 2);
			this->addChild(gameOver);
		}
		return;
	}

	if (GameState::getInstance()->isDestoryed()) {
		static float gap = 0;
		gap += dt;
		if (gap > 0.5) {
			this->removeAllChildren();
			round = 3;
			return;
		}
	}

	scoreBoard->addNumber(enemy->getDestoryedNum() * 100);

	if (enemy->isOver()) {
		if (round == 2) {
			round++;
			this->removeAllChildren();
			return;
		}
		this->removeChild(map);
		this->removeChild(playerTank);
		this->removeChild(enemy);
		round++;
		if (round == 3) {
			this->removeAllChildren();
			return;
		}
		map = TileMap::createWithRound(round);
		enemy = Enemy::createWithEnemyNums(EnemyNum[round - 1]);
		playerTank = Tank::createWithStartPos(Vec2(60, 60));

		enemy->setPlayerTankPointer(playerTank);
		enemy->setMap(map);
		playerTank->setEnemyTank(enemy);
		playerTank->setTileMap(map);
		this->addChild(map,11);
		this->addChild(enemy);
		this->addChild(playerTank);
	}


}

