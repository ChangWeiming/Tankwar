#include "HelloWorldScene.h"
#include <iostream>
#include "GameUI.h"

USING_NS_CC;


Scene* HelloWorld::scene()
{
     return HelloWorld::create();
}

bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
	
	std::string s;
	s = "cocos2d";

    auto label = Label::createWithTTF(s, "fonts/arial.ttf", 80);

    label->setPosition(visibleSize.width/2,
                            visibleSize.height - label->getContentSize().height);

    this->addChild(label, 2);
	log("add Label of \"cocos2d\" Successfully");

	auto startScene = Sprite::create("ipad/HelloWorld.png");
	startScene->setPosition(label->getPositionX(),label->getPositionY()-label->getContentSize().height * 2);
	startScene->setZOrder(1);
	this->addChild(startScene, 1);
	log("Background loaded successfully");

	auto pMenuImage = MenuItemImage::create("ipad/CloseNormal.png", "ipad/CloseSelected.png", this, menu_selector(HelloWorld::MenuCallBack));
	pMenuImage->setAnchorPoint(ccp(0.5, 0.5));
	pMenuImage->setPosition(ccp(0,0));

	auto pMenu = Menu::create(pMenuImage, nullptr);
	pMenu->setPosition(visibleSize.width/2,visibleSize.height/2-startScene->getContentSize().height);
	pMenu->setZOrder(2);

	this->addChild(pMenu);


    return true;
}

void HelloWorld::MenuCallBack(Object* sender) {
	log("EnteredCallBackFunc");
	auto startGameUI = GameUI::create();
	
	Director::getInstance()->replaceScene(startGameUI);
}

