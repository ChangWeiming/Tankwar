#include "Missile.h"
#include "cocos2d.h"
using namespace cocos2d;

Missile::Missile() {
	face = TankState::DOWN;
}

Missile::~Missile() {

}

Missile::Missile(TankState::TankType temp) {
	face = TankState::DOWN;
	type = temp;
}

bool Missile::init() {

	if (!Sprite::init()) {
		log("Missile unexpected errors");
		return false;
	}

	isBooming = false;

	if (type == TankState::PlayerTank) {
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("tankmissile.gif");
		this->setSpriteFrame(frame);
	}
	else {
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("enemymissile.gif");
		this->setSpriteFrame(frame);
	}

	this->setVisible(false);

	return true;
}

Rect Missile::getRect() const {
	return rect;
}

bool Missile::getBoomingState() const {
	return isBooming;
}

void Missile::setType(TankState::TankType temp) {
	type = temp;
}

void Missile::setMap(TileMap* mp) {
	map = mp;
}

void Missile::setPositionAndRect(const Vec2& temp) {
	this->setPosition(temp);
	float _w = this->getContentSize().width;
	float _h = this->getContentSize().height;
	float _x = temp.x - _w / 2;
	float _y = temp.y - _h / 2;
	rect.setRect(_x, _y, _w, _h);
}

void Missile::setFace(TankState::Direction temp) {
	face = temp;
}

void Missile::goBoom() {
	isBooming = true;
	auto animation = Animation::create();
	animation->addSpriteFrameWithFile("blast1.png");
	animation->addSpriteFrameWithFile("blast2.png");
	animation->addSpriteFrameWithFile("blast3.png");
	animation->setDelayPerUnit(0.1f);
	animation->setLoops(1);

	auto animate = Animate::create(animation);
	auto callFunc = CallFunc::create(this, callfunc_selector(Missile::setInvisible));
	auto seq = Sequence::createWithTwoActions(animate, callFunc);

	this->runAction(seq);

}

void Missile::setInvisible() {
	isBooming = false;
	this->setVisible(false);
	
	if (type == TankState::PlayerTank) {
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("tankmissile.gif");
		this->setSpriteFrame(frame);
	}
	else {
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("enemymissile.gif");
		this->setSpriteFrame(frame);
	}

}

void Missile::getNextState(float dt) {
	if (this->isVisible() == false) return;
	if (isBooming) return;

	static bool firstReborn = true;

	if (firstReborn) {
		firstReborn = false;
		auto pos = this->getPosition();
		if (collisionTest(pos)) {
			this->setPositionAndRect(pos);
			this->goBoom();
			firstReborn = true;
		}
	}

	auto MissileSpeed = TankState::MissileSpeed*dt;
	auto pos = Sprite::getPosition();

	switch (face) {
		case TankState::UP:
			pos.y += MissileSpeed;
			break;
		case TankState::DOWN:
			pos.y -= MissileSpeed;
			break;
		case TankState::LEFT:
			pos.x -= MissileSpeed;
			break;
		case TankState::RIGHT:
			pos.x += MissileSpeed;
			break;
		default:
			break;
	}
	
	if (collisionTest(pos)) {
		this->setPositionAndRect(pos);
		firstReborn = true;;
		this->goBoom();
	}
	else {
		this->setPositionAndRect(pos);
	}
}

bool Missile::collisionTest(Vec2& nowPos) {
	if (nowPos.x < 0) {
		nowPos.x = 0;
		return true;
	}

	if (nowPos.y < 0) {
		nowPos.y = 0;
		return true;
	}

	if (nowPos.x >= gameSize.x) {
		nowPos.x = gameSize.x;
		return true;
	}

	if (nowPos.y >= gameSize.y) {
		nowPos.y = gameSize.y;
		return true;
	}

	bool ret = false;

	auto brickLayer = map->getBrickLayer();
	auto steelLayer = map->getSteelLayer();

	//following two positions are two vertices 
	Vec2 pos1 = nowPos, pos2 = nowPos;

	auto _width = this->getContentSize().width / 2;
	auto _height = this->getContentSize().height / 2;
	auto tileSize = map->getTileSize();
	auto tileY = brickLayer->getLayerSize().height;

	using namespace TankState;

	if (face == UP) {
		pos1.x -= _width;
		pos1.y += _height;

		pos2.x += _width;
		pos2.y += _height;
	}
	else if(face == DOWN) {
		pos1.x -= _width;
		pos1.y -= _height;

		pos2.x += _width;
		pos2.y -= _height;

	}
	else if (face == LEFT) {
		pos1.x -= _width;
		pos1.y += _height;

		pos2.x -= _width;
		pos2.y -= _height;
	}
	else if (face == RIGHT) {
		pos1.x += _width;
		pos1.y += _height;

		pos2.x += _width;
		pos2.y -= _height;
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

	Sprite* sprite[4];
	sprite[0] = brickLayer->getTileAt(pos1);
	sprite[1] = brickLayer->getTileAt(pos2);
	sprite[2] = steelLayer->getTileAt(pos1);
	sprite[3] = steelLayer->getTileAt(pos2);


	auto _w = this->getContentSize().width / 2;
	auto _h = this->getContentSize().height / 2;

	rect.setRect(nowPos.x - _w, nowPos.y - _h, _height, _width);

	if (rect.intersectsRect(GameState::getInstance()->getBaseRect())) {
		GameState::getInstance()->setDestoryed(true);
		return true;
	}

	Rect spriteRect;
	for (int i = 0; i < 2; i++) {
		if (sprite[i] != nullptr && sprite[i]->isVisible()) {
			sprite[i]->setVisible(false);
			spriteRect.setRect(sprite[i]->getPosition().x, sprite[i]->getPosition().y, tileSize.width, tileSize.height);
			ret = true;
			if (face == UP) {
				nowPos.y = spriteRect.getMidY();
			}
			else if (face == DOWN) {
				nowPos.y = spriteRect.getMidY();
			}
			else if (face == RIGHT) {
				nowPos.x = spriteRect.getMidX();
			}
			else if (face == LEFT) {
				nowPos.x = spriteRect.getMidX();
			}
			else {
				CCAssert(false, "Debug:face not initialized");
			}
		}
	}

	for (int i = 2; i < 4; i++) {
		if (sprite[i] != nullptr) {
			ret = true;
		}
	}

	return ret;
}

