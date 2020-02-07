#include "EnemyTank.h"
#include "Tank.h"
#include "cocos2d.h"
#include "Constant.h"

using namespace cocos2d;

EnemyTank::EnemyTank() {
	face = TankState::DOWN;
	shootGap = 0;
}

Missile* EnemyTank::getMissile() const {
	return pMissile;
}
void EnemyTank::setType(TankState::EnemyTankType temp) {
	type = temp;
}

void EnemyTank::setMap(TileMap* mp) {
	map = mp;
}

void EnemyTank::setBorningState() {
	isBorning = false;

	auto cache = SpriteFrameCache::getInstance();
	if (type == TankState::Normal) {
		auto frame = cache->getSpriteFrameByName("NormalD.gif");
		typeString = "Normal";
		this->setSpriteFrame(frame);
	}
	else if (type == TankState::Fast) {
		auto frame = cache->getSpriteFrameByName("FastD.gif");
		typeString = "Fast";
		this->setSpriteFrame(frame);
	}
	else if (type == TankState::HighHP) {
		auto frame = cache->getSpriteFrameByName("HighHPD.gif");
		typeString = "HighHP";
		this->setSpriteFrame(frame);
	}
	else {
		log("Debug: init the enemy tank without type");
	}
	pMissile->setMap(map);
}

void EnemyTank::setBoomingState() {
	isBooming = false;
	this->setVisible(false);
}

void EnemyTank::born(const Vec2&startPos) {

	if (isBorning) return;
	isBorning = true;

	auto animation = Animation::create();
	this->setTankPosition(startPos);

	char _born[20] = "born";
	for (int i = 1; i <= 4; i++) {
		char tmp[20];
		sprintf(tmp, "%s%d.png", _born, i);

		animation->addSpriteFrameWithFile(tmp);
	
	}

	animation->setDelayPerUnit(BorningFrameGap);
	animation->setLoops(1);

	auto animate = Animate::create(animation);
	auto callFunc = CallFunc::create(this, callfunc_selector(EnemyTank::setBorningState));
	auto seq = Sequence::createWithTwoActions(animate, callFunc);

	seq->setDuration(animate->getDuration());

	this->runAction(seq);
}

void EnemyTank::goBoom() {
	if (isBorning) return;
	if (isBooming) return;
	isBooming = true;

	//log("Boooooooooooooom:%lf %lf", this->getPositionX(), this->getPositionY());

	auto animation = Animation::create();
	
	char _blast[20] = "blast";
	
	for (int i = 1; i <= 6; i++) {
		
		char tmp[20];
		sprintf(tmp, "%s%d.png", _blast, i);
		animation->addSpriteFrameWithFile(tmp);
	}

	animation->setDelayPerUnit(0.075);
	animation->setLoops(1);

	auto animate = Animate::create(animation);
	
	auto callFunc = CallFunc::create(this, callfunc_selector(EnemyTank::setBoomingState));

	auto seq = Sequence::createWithTwoActions(animate, callFunc);

	seq->setDuration(animate->getDuration());

	this->runAction(seq);
}

EnemyTank* EnemyTank::createWithType(TankState::EnemyTankType temp) {

	EnemyTank *pRet = new(std::nothrow) EnemyTank();
	pRet->setType(temp);

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

Rect EnemyTank::getRect() {
	return rect;
}

Vec2 EnemyTank::getShootingPosition() {
	auto tankSize = this->getContentSize();
	auto pos = this->getPosition();
	using namespace TankState;
	if (face == UP) {
		return Vec2(pos.x, pos.y + tankSize.height / 2);
	}
	else if (face == DOWN) {
		return Vec2(pos.x, pos.y - tankSize.width / 2);
	}
	else if (face == LEFT) {
		return Vec2(pos.x - tankSize.width / 2, pos.y);

	}
	else {
		return Vec2(pos.x + tankSize.width / 2, pos.y);
	}
}

bool EnemyTank::init() {
	if (!Sprite::init()) {
		log("Debug: EnemyTankClass init Error!");
		return false;
	}

	isBorning = isBooming = false;

	pMissile = Missile::createWithType(TankState::EnemyTank); 
	pMissile->setVisible(false);
	this->addChild(pMissile,10);
	 
	auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("born1.gif");
	this->setSpriteFrame(frame);

	this->setVisible(true);
	
	return true;
}

Rect EnemyTank::getMissileRect() const {
	return pMissile->getRect();
}

void EnemyTank::getNextState(float dt) {
	if (this->isVisible() == false) return;
	if (isBorning || isBooming) {
		return;
	}
	auto pos = this->getPosition();

//	log("Rect:%lf %lf", rect.getMinX(), rect.getMinY());
//	log("Pos:%lf %lf", this->getPositionX(), this->getPositionX());

	SpriteFrame* frame;
	auto TankSpeed = TankState::TankSpeed*dt;
	switch (face) {
	case TankState::Direction::UP:
		pos.y += TankSpeed;
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(typeString + "U.gif");
		this->setSpriteFrame(frame);
		break;
	case TankState::Direction::DOWN:
		pos.y -= TankSpeed;
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(typeString + "D.gif");
		this->setSpriteFrame(frame);
		break;
	case TankState::Direction::LEFT:
		pos.x -= TankSpeed;
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(typeString + "L.gif");
		this->setSpriteFrame(frame);
		break;
	case TankState::Direction::RIGHT:
		pos.x += TankSpeed;
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(typeString + "R.gif");
		this->setSpriteFrame(frame);
		break;
	}

	auto resultBoarder = isBoarder(pos);
	auto resultCollision = isCollision(pos);

	if (resultBoarder ||
		resultCollision ) {
		auto number = rand() % 4;
		while (number == face) number = rand() % 4;
		face = (TankState::Direction)number;
	}

	this->setTankPosition(pos);

	shootGap += dt;
	if (shootGap > 1.00 && !pMissile->isVisible()) {
		shootGap = 0;
		auto missilePos = getShootingPosition();
		auto testResult = pMissile->collisionTest(missilePos);
		pMissile->setVisible(!testResult);
		pMissile->setFace(face);
		pMissile->setPositionAndRect(missilePos);
	}
	//log("isVisible:%d", pMissile->isVisible());
	//log("pMissile->getPosition:%lf %lf", pMissile->getPositionX(), pMissile->getPositionY());
	//log("this->getPosition:%lf %lf", this->getPositionX(), this->getPositionY());
}

void EnemyTank::setTankPosition(const Vec2& temp) {

	float _w = 60.0f;
	float _h = 60.0f;

	float _x = temp.x - _w / 2;
	float _y = temp.y - _h / 2;
	rect.setRect(_x, _y, _w, _h);

	this->setPosition(temp);
}

bool EnemyTank::isBoarder(Vec2& pos) {
	bool ret = false;

	if (gameSize.x < pos.x+this->getContentSize().width/2) {
		pos.x = gameSize.x-this->getContentSize().width/2;
		ret = true;
	}
	else if (pos.x-this->getContentSize().width/2 < 0) {
		pos.x = this->getContentSize().width/2;
		ret = true;
	}
	
	if (gameSize.y < pos.y+this->getContentSize().height/2) {
		pos.y = gameSize.y-this->getContentSize().height/2;
		ret = true;
	}
	else if (pos.y - this->getContentSize().height/2 < 0) {
		pos.y = this->getContentSize().height/2;
		ret = true;
	}
	return ret;
}

bool EnemyTank::isCollision(Vec2& pos) {

	float _w = this->getContentSize().width;
	float _h = this->getContentSize().height;
	float _x = pos.x - _w / 2;
	float _y = pos.y - _h / 2;
	rect.setRect(_x, _y, _w, _h);

	bool ret = false;
	Rect playerRect = playerTank->getRect();
	//log("playerPosition:%lf %lf", playerTank->getPlayerTankPos().x, playerTank->getPlayerTankPos().y);
	if (playerRect.intersectsRect(rect)) {
		ret = true;
		float offsetX = 0, offsetY = 0;

		using namespace TankState;
		if (face == RIGHT && rect.getMaxX() >= playerRect.getMinX()) {
			offsetX = playerRect.getMinX()- rect.getMaxX() - 1;
		}
		else if (face == LEFT && rect.getMinX() <= playerRect.getMaxX()) {
			offsetX = playerRect.getMaxX() - rect.getMinX() + 1;

//			log("\n%lf %lf %lf\n", playerRect.getMaxX(), rect.getMinX(), offsetX);
		}
		else if (face == UP && rect.getMaxY() >= playerRect.getMinY()) {
			offsetY = playerRect.getMinY() - rect.getMaxY() - 1;
		}
		else if (face == DOWN && rect.getMinY() <= playerRect.getMaxY()) {
			offsetY = playerRect.getMaxY() - rect.getMinY() + 1;
		}

		pos.x += offsetX;
		pos.y += offsetY;
/*
		if (Vec2(offsetX,offsetY) != Vec2::ZERO) {
			log("offset:%lf %lf", offsetX, offsetY);
			log("rectMin:%lf %lf", rect.getMinX(), rect.getMinY());
			log("rectMax:%lf %lf", rect.getMaxX(), rect.getMaxY());
			log("playerTankMax:%lf %lf", playerRect.getMaxX(), playerRect.getMaxY());
			log("playerRectMin:%lf %lf", playerRect.getMinX(), playerRect.getMinY());

		}
*/
	}

	
	_x = pos.x - _w / 2;
	_y = pos.y - _h / 2;
	rect.setRect(_x, _y, _w, _h);


	Ref* pRef;
	CCARRAY_FOREACH(enemyTanks, pRef) {
		auto pEnemy = (EnemyTank*)pRef;
		if (pEnemy == this) continue;
		auto enemyRect = pEnemy->getRect();

		if (enemyRect.intersectsRect(rect)) {

			ret = true;
			float offsetX = 0, offsetY = 0;

			using namespace TankState;
			if (face == RIGHT && rect.getMaxX() >= enemyRect.getMinX()) {
				offsetX = enemyRect.getMinX()- rect.getMaxX() - 1;
			}
			else if (face == LEFT && rect.getMinX() <= enemyRect.getMaxX()) {
				offsetX = enemyRect.getMaxX() - rect.getMinX() + 1;
	
	//			log("\n%lf %lf %lf\n", playerRect.getMaxX(), rect.getMinX(), offsetX);
			}
			else if (face == UP && rect.getMaxY() >= enemyRect.getMinY()) {
				offsetY = enemyRect.getMinY() - rect.getMaxY() - 1;
			}
			else if (face == DOWN && rect.getMinY() <= enemyRect.getMaxY()) {
				offsetY = enemyRect.getMaxY() - rect.getMinY() + 1;
			}
	
			pos.x += offsetX;
			pos.y += offsetY;		
		}
	}

	//map with tank
	auto brickLayer = map->getBrickLayer();
	auto steelLayer = map->getSteelLayer();

	//following two positions are two vertices
	Vec2 pos1 = pos, pos2 = pos, pos3 = pos;

	auto _width = this->getContentSize().width / 2;
	auto _height = this->getContentSize().height / 2;
	auto tileSize = map->getTileSize();
	auto tileY = brickLayer->getLayerSize().height;
	_width = _width - 3;
	_height = _height - 3;


	using namespace TankState;

	if (face == UP) {
		pos1.x -= _width;
		pos1.y += _height;

		pos2.x += _width;
		pos2.y += _height;

		pos3.y += _height;
	}
	else if(face == DOWN) {
		pos1.x -= _width;
		pos1.y -= _height;

		pos2.x += _width;
		pos2.y -= _height;
		
		pos3.y -= _height;
	}
	else if (face == LEFT) {
		pos1.x -= _width;
		pos1.y += _height;

		pos2.x -= _width;
		pos2.y -= _height;

		pos3.x -= _width;
	}
	else if (face == RIGHT) {
		pos1.x += _width;
		pos1.y += _height;

		pos2.x += _width;
		pos2.y -= _height;

		pos3.x += _width;
	}
	else {
		CCAssert(false, "Debug:face not initialized");
	}

	auto _adjust = [](Vec2& _pos,float _tileY) {
		if (_pos.y < 0) _pos.y = 0;
		else if ((int)_pos.y == _tileY) _pos.y = _tileY - 1;
		if ((int)_pos.x == _tileY) _pos.x = _tileY - 1;
		else if (_pos.x < 0) _pos.x = 0;
	};

	pos1.x = (int)(pos1.x / tileSize.width);
	pos1.y = (int)(pos1.y / tileSize.height);
	pos1.y = tileY - pos1.y - 1;
	_adjust(pos1, tileY);

	pos2.x = (int)(pos2.x / tileSize.width);
	pos2.y = (int)(pos2.y / tileSize.height);
	pos2.y = tileY - pos2.y - 1;
	_adjust(pos2, tileY);


	pos3.x = (int)(pos3.x / tileSize.width);
	pos3.y = (int)(pos3.y / tileSize.height);
	pos3.y = tileY - pos3.y - 1;
	_adjust(pos3, tileY);

//brickLayer

	Sprite* sprite[6];
	sprite[0] = brickLayer->getTileAt(pos1);
	sprite[1] = brickLayer->getTileAt(pos2);
	sprite[2] = brickLayer->getTileAt(pos3);
	sprite[3] = steelLayer->getTileAt(pos1);
	sprite[4] = steelLayer->getTileAt(pos2);
	sprite[5] = steelLayer->getTileAt(pos3);


	rect.setRect(pos.x - _w / 2, pos.y - _h / 2, _w, _h);

	Rect spriteRect;
	for (int i = 0; i < 6; i++) {
		if (sprite[i] != nullptr && sprite[i]->isVisible()) {

			auto _pos = sprite[i]->getPosition();
			spriteRect.setRect(_pos.x, _pos.y, tileSize.width, tileSize.height);
			ret = true;
			if (spriteRect.intersectsRect(rect)) {
				if (face == UP) {
					pos.y += spriteRect.getMinY() - rect.getMaxY() - 1;
					break;
				}
				else if (face == DOWN) {
					pos.y += spriteRect.getMaxY() - rect.getMinY() ;
					break;
				}
				else if (face == RIGHT) {
					pos.x += spriteRect.getMinX() - rect.getMaxX() - 1;
					break;
				}
				else if (face == LEFT) {
					pos.x += spriteRect.getMaxX() - rect.getMinX() ;
					break;
				}
				else {
					CCAssert(false, "Debug:face not initialized");
				}
			}
		}
	}
	return ret;
}

void EnemyTank::setEnemyTanksArray(Array* temp) {
	enemyTanks = temp;
	enemyTanks->retain();
}

void EnemyTank::setPlayerTank(Tank* tank) {
	playerTank = tank;
	playerTank->retain();
}

void EnemyTank::setMissile(Missile* temp) {
	pMissile = temp;
	pMissile->retain();
}
