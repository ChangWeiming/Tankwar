#include "StateBoard.h"
#include "Tank.h"
#include "cocos2d.h"

using namespace cocos2d;

StateBoard* StateBoard::scene() {
	return StateBoard::create();
}

bool StateBoard::init() {

	if (!Layer::init()) {
		return false;
	}

	/*
	 *Because of some reasons, following numbers are calculated manually
	 *if resolution policy are changed it is advised to have it revised too.
	 */
	auto BlackBackground = LayerColor::create(Color4B(96,96,96,255),244,780);

	BlackBackground->setAnchorPoint(Vec2::ZERO);
	BlackBackground->setPosition(780, 0);

	//log("BackgroundPos:%lf %lf", BlackBackground->getPosition().x,BlackBackground->getPosition().y);

	LabelScore = Label::createWithTTF("Score", "fonts/AzoteSSi.ttf",  50);
	number = 0;
	scoreNum = nullptr;

	LabelScore->setPosition(ccp(902, 780 - LabelScore->getContentSize().height));

	this->addChild(LabelScore,1);
	this->addChild(BlackBackground, 0);

	_scheduler->scheduleUpdate(this,4,false);
	return true;
}

void StateBoard::addNumber(int num) {
	number += num;
}

void StateBoard::update(double dt) {
	Layer::update(dt);
	static int _num = -1;
	if (_num != number) {
		_num = number;
		char _number[20];
		if (scoreNum) {
			this->removeChild(scoreNum);
		}
		sprintf(_number, "%d", number);
		scoreNum = Label::createWithTTF(_number, "fonts/arial.ttf",50);
		scoreNum->setPosition(Vec2(LabelScore->getPositionX(), LabelScore->getPositionY() - LabelScore->getContentSize().height * 2));
		this->addChild(scoreNum, 1);
	}
}
