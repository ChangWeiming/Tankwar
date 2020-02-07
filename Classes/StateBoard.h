#pragma once

#include "cocos2d.h"

class StateBoard :
	public cocos2d::Layer
{
private:
	cocos2d::Label* scoreNum;
	cocos2d::Label* LabelScore;
	int number;
public:
	static StateBoard* scene();
	virtual bool init();
	CREATE_FUNC(StateBoard);
	void addNumber(int);
	virtual void update(double);
};
