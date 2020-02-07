#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Scene
{
public:
    virtual bool init() override;

    static cocos2d::Scene* scene();

	void MenuCallBack(cocos2d::Object* sender);
    // implement the "static create()" method manually
	CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
