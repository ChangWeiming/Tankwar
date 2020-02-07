#include "Tank.h"
#include "cocos2d.h"
#include "Constant.h"
#include "Missile.h"
#include "Enemy.h"
#include "EnemyTank.h"
#include<ctime>

using namespace cocos2d;

Tank::Tank() {
	//frameTime = 0;
	KeyPressingNums = 0;
	KeyPressing = EventKeyboard::KeyCode::KEY_NONE;
	startPos = ccp(0,0);
	isBorning = false;
	isBooming = false;
}

Tank::Tank(cocos2d::Vec2 StartPos) {
	//frameTime = 0;
	KeyPressingNums = 0;
	KeyPressing = EventKeyboard::KeyCode::KEY_NONE;
	startPos = StartPos;
}

Tank::~Tank() {

}

Tank* Tank::scene() {
	return Tank::create();
}

Rect Tank::getRect() const {
	return rect;
}

Rect Tank::getMissileRect() const {
	return pMissile->getRect();
}

bool Tank::getTankVisible() const {
	return playerTank->isVisible();
}

bool Tank::getMissileBoomingState() const{
	return pMissile->getBoomingState();
}


void Tank::setTankInvisible() {
	playerTank->setVisible(false);
}

void Tank::goBoom() {

	if (isBooming) return;

	isBooming = true;
	auto animation = Animation::create();

	auto cache=SpriteFrameCache::getInstance();
	char _blast[20] = "blast";
	for (int i = 1; i <= 8; i++) {
		char tmp[20];
		sprintf(tmp, "%s%d.gif", _blast, i);
		auto frame = cache->getSpriteFrameByName(tmp);
		animation->addSpriteFrame(frame);
	}

	animation->setLoops(1);
	animation->setDelayPerUnit(0.075f);

	auto animate = Animate::create(animation);
	auto callFunc = CallFunc::create(this, callfunc_selector(Tank::setBooming));
	auto seq = Sequence::createWithTwoActions(animate,callFunc);

	seq->setDuration(animation->getDuration());

	playerTank->runAction(seq);
}

void Tank::setTileMap(TileMap* _map) {
	map = _map;
	map->retain();
}

void Tank::setBooming() {
	isBooming = false;
	playerTank->setVisible(false);
}

void Tank::setPositionAndRect(const Vec2& temp) {

	playerTank->setPosition(temp);
	float _w = playerTank->getContentSize().width;
	float _h = playerTank->getContentSize().height;
	float _x = playerTank->getPosition().x - _w / 2;
	float _y = playerTank->getPosition().y - _h / 2;
	rect.setRect(_x, _y, _w, _h);	

}

void Tank::reborn() {
	isBorning = true;
	
	auto borningFrame = Animation::create();

	char picName[20] = "born";
	auto cache = SpriteFrameCache::getInstance();
	for (int i = 1; i <= 4; i++) {
		char tmp[20];
		sprintf(tmp, "%s%d.gif", picName, i);
		auto frame = cache->getSpriteFrameByName(tmp);
		borningFrame->addSpriteFrame(frame);
	}

	borningFrame->setDelayPerUnit(BorningFrameGap);

	auto borningAction = Animate::create(borningFrame);
	auto callFunc = CallFunc::create(this, callfunc_selector(Tank::createTank));
	auto seq = Sequence::createWithTwoActions(borningAction, callFunc);
	seq->setDuration(borningFrame->getDuration());

	playerTank->setVisible(true);
	setPositionAndRect(startPos);
	playerTank->setSpriteFrame(cache->getSpriteFrameByName("born1.gif"));

	playerTank->runAction(seq);
}

void Tank::createTank() {
	KeyPressingNums = 0;
	isBorning = false;
	playerTank->setSpriteFrame("p1tankU.gif");
	pMissile->setPositionAndRect(startPos);
	face = TankState::UP;
}

bool Tank::getBorningStatus() {
	return isBorning;
}

TankState::Direction Tank::getFace() {
	return face;
}

bool Tank::init() {
	if (!Layer::init()) {
		return false;
	}

	face = TankState::UP;
	fireCmd = false;
	pMissile = Missile::createWithType(TankState::PlayerTank);
	isBooming = false;
	isBorning = false;

	auto cache = SpriteFrameCache::getInstance();

	playerTank = Sprite::create();
	playerTank->setSpriteFrame("p1tankU.gif");
	playerTank->setVisible(false);
	this->addChild(playerTank);

	this->addChild(pMissile,10);

	this->scheduleUpdate();
	auto listener = EventListenerKeyboard::create();
	
	listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event){

		if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW ||
			keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW ||
			keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW ||
			keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
			
			KeyPressingNums++;
			KeyPressing = keyCode;

		}

		else if(keyCode == EventKeyboard::KeyCode::KEY_SPACE){
			fireCmd = true;
		}

	};

	listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event){
		if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW ||
			keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW ||
			keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW ||
			keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {

			if(KeyPressingNums>0) KeyPressingNums--;
			if (KeyPressingNums == 0) {
				KeyPressing = EventKeyboard::KeyCode::KEY_NONE;
			}

		}
		else if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) {
			fireCmd = false;
		}
	};
	
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

void Tank::update(float dt) {
	Layer::update(dt);
	//auto Player = this->getChildByTag(1001);

	if (isBorning || isBooming) {
		if (pMissile->isVisible()) {
			pMissile->getNextState(dt);
		}
		else pMissile->setPositionAndRect(playerTank->getPosition());
		return;
	}
	if (playerTank->isVisible() == false) {
		pMissile->setMap(map);
		reborn();
		return;
	}

	SpriteFrame* frame;
	float offsetX = 0, offsetY = 0;
	const float SpeedPlayerTank = TankState::TankSpeed*dt;

	switch (KeyPressing) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW: 
		offsetX = -SpeedPlayerTank;
		frame = SpriteFrameCache::getInstance()->spriteFrameByName("p1tankL.gif");
		face = TankState::LEFT;
		playerTank->setSpriteFrame(frame);
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		offsetX = SpeedPlayerTank;
		frame = SpriteFrameCache::getInstance()->spriteFrameByName("p1tankR.gif");
		face = TankState::RIGHT;
		playerTank->setSpriteFrame(frame);
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		offsetY = SpeedPlayerTank;
		frame = SpriteFrameCache::getInstance()->spriteFrameByName("p1tankU.gif");
		face = TankState::UP;
		playerTank->setSpriteFrame(frame);
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		offsetY = -SpeedPlayerTank;
		face = TankState::DOWN;
		frame = SpriteFrameCache::getInstance()->spriteFrameByName("p1tankD.gif");
		playerTank->setSpriteFrame(frame);
		break;
	}

	auto nextPos = getNextPosition(offsetX, offsetY);
	isCollision(nextPos);

	setPositionAndRect(nextPos);
	
	//log(">>>>MissileState:%d\nMissilePosition:%lf %lf", pMissile->isVisible(),pMissile->getPositionX(),pMissile->getPositionY());
	if (fireCmd && !pMissile->isVisible()) {
		Vec2 pos = getShootingPosition();
		pMissile->setPositionAndRect(pos);
		pMissile->setFace(face);
		pMissile->setVisible(true);
	}
	else {
		if (pMissile->isVisible()) {
			pMissile->getNextState(dt);
		}
		else {
			pMissile->setPositionAndRect(playerTank->getPosition());
		}
	}
	//log("MissileState:%d\nMissilePosition:%lf %lf", pMissile->isVisible(),pMissile->getPositionX(),pMissile->getPositionY());
}

Vec2 Tank::getShootingPosition() {

	auto pos = playerTank->getPosition();

	switch (face) {
		case TankState::UP:
			pos.y += playerTank->getContentSize().height / 2 + pMissile->getContentSize().height / 2;
			break;
		case TankState::DOWN:
			pos.y -= playerTank->getContentSize().height / 2 + pMissile->getContentSize().height / 2;
			break;
		case TankState::LEFT:
			pos.x -= playerTank->getContentSize().width / 2 + pMissile->getContentSize().width / 2;
			break;
		case TankState::RIGHT:
			pos.x += playerTank->getContentSize().width / 2 + pMissile->getContentSize().width / 2;
			break;
		default:
			break;
	}

	return pos;
}

Vec2 Tank::getNextPosition(const float &x,const float &y) {
	auto nextPos = ccp(x + playerTank->getPositionX(), y + playerTank->getPositionY());
	auto tankSize = playerTank->getContentSize();
	tankSize = tankSize / 2;

	if (nextPos.x - tankSize.width < 0) {
		nextPos.x = tankSize.width ;
	}
	
	if (nextPos.y - tankSize.height < 0) {
		nextPos.y = tankSize.height ;
	}
	
	auto frameSize = gameSize;

	frameSize.x = frameSize.y;

	if (nextPos.x + tankSize.width > frameSize.x) {
		nextPos.x = frameSize.x - tankSize.width;
	}

	if (nextPos.y + tankSize.height > frameSize.y) {
		nextPos.y = frameSize.y -tankSize.height;
	}

	return  nextPos;
}

void Tank::setEnemyTank(Enemy* temp) {
	enemyTank = temp;
	enemyTank->retain();
}

void Tank::setMissileInvisible() {
	pMissile->setVisible(false);
	pMissile->setPositionAndRect(playerTank->getPosition());
}

bool Tank::isCollision(Vec2& pos) {
	float _w = playerTank->getContentSize().width;
	float _h = playerTank->getContentSize().height;
	float _x = pos.x - _w / 2;
	float _y = pos.y - _h / 2;
	rect.setRect(_x, _y, _w, _h);

	bool ret = false;
	auto enemyRects = enemyTank->getTankAlivePos();
	
	for (auto it : enemyRects) {
		if (rect.intersectsRect(it)) {
			ret = true;
			float offsetX = 0, offsetY = 0;
	
			using namespace TankState;
			if (face == RIGHT && rect.getMaxX() >= it.getMinX()) {
				offsetX = it.getMinX() - rect.getMaxX() - 1;
			}
			else if (face == LEFT && rect.getMinX() <= it.getMaxX()) {
				offsetX = it.getMaxX() - rect.getMinX() + 1;

			}
			else if (face == UP && rect.getMaxY() >= it.getMinY()) {
				offsetY = it.getMinY() - rect.getMaxY() - 1;
			}
			else if (face == DOWN && rect.getMinY() <= it.getMaxY()) {
				offsetY = it.getMaxY() - rect.getMinY() + 1;
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

	auto _width = playerTank->getContentSize().width / 2;
	auto _height = playerTank->getContentSize().height / 2;
	auto tileSize = map->getTileSize();
	auto tileY = brickLayer->getLayerSize().height;


	using namespace TankState;

	if (face == UP) {
		pos1.x -= _width - 3;
		pos1.y += _height;

		pos2.x += _width - 3;
		pos2.y += _height;

		pos3.y += _height;
	}
	else if(face == DOWN) {
		pos1.x -= _width - 3;
		pos1.y -= _height;

		pos2.x += _width - 3;
		pos2.y -= _height;
		
		pos3.y -= _height;
	}
	else if (face == LEFT) {
		pos1.x -= _width;
		pos1.y += _height - 3;

		pos2.x -= _width;
		pos2.y -= _height - 3;

		pos3.x -= _width;
	}
	else if (face == RIGHT) {
		pos1.x += _width;
		pos1.y += _height - 3;

		pos2.x += _width;
		pos2.y -= _height - 3;

		pos3.x += _width;
	}
	else {
		CCAssert(false, "Debug:face not initialized");
	}

	auto _adjust = [](Vec2& _pos, float _tileY) {
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

//brickLayer and steel layer

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
