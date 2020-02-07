
#include "AppDelegate.h"

#include <vector>
#include <string>

#include "HelloWorldScene.h"
#include "GameUI.h"

USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // Initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("TankWar");
        director->setOpenGLView(glview);
    }

    std::string icons = "icons/Icon-60@3x.png";
    glview->setIcon(icons);

    director->setOpenGLView(glview);
	glview->setFrameSize(1024, 780);
    // Set the design resolution
	glview->setDesignResolutionSize(1024, 780, ResolutionPolicy::SHOW_ALL);

    Size frameSize = glview->getFrameSize();
	vector<string> searchPath;
    FileUtils::getInstance()->setSearchPaths(searchPath);

    director->setDisplayStats(true);
    director->setAnimationInterval(1.0f / 60);

	auto test = HelloWorld::scene();

	director->runWithScene(test);

    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();

    // If you use SimpleAudioEngine, it must be paused
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
}
