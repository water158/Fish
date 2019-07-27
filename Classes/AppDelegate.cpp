#include "AppDelegate.h"

#include "MainScene.h"
#include "MenuLayer.h"
#include "UserData.h"
#include "LogoLayer.h"
#include "TestUIScene.h"
#include "LoadingLayer.h"
#include "UserChannel.h"
// #include "MobClickCpp.h"

USING_NS_CC;
AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }
	//glview->setFrameSize(960, 540);
    glview->setDesignResolutionSize(854, 480, ResolutionPolicy::EXACT_FIT);
    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    //auto scene = MainScene::scene();
	UserData::getInstance()->load();
	//UserData::getInstance()->setGoldNum(10000);
	//UserData::getInstance()->setDiamonds(100);
	//UserData::getInstance()->save();

#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	// MOBCLICKCPP_START_WITH_APPKEY_AND_CHANNEL("59115e45b27b0a31c3000de5", getUmengChannelString().c_str());
#endif
    

    auto scene = Scene::create();
	Manager::getInstance()->setEnterGame(true);
	Manager::getInstance()->setFirstEnter(true);
    scene->addChild(MenuLayer::create());
	//scene->addChild(TestUIScene::create());
    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();

#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	// umeng::MobClickCpp::applicationDidEnterBackground();
	/*umeng::eventDict dict;
	dict["gold"] = "gold";
	dict["num"] = getStringFromInt(UserData::getInstance()->getGoldNum());
	umeng::MobClickCpp::event(getStringFromInt(YM_GOLD_NUM).c_str(), &dict);*/

#endif

#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	
	//umeng::MobClickCpp::end();
	//Director::getInstance()->end();
#endif

}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();

#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	// umeng::MobClickCpp::applicationWillEnterForeground();
	/*umeng::eventDict dict;
	dict["gold"] = "gold";
	dict["num"] = getStringFromInt(UserData::getInstance()->getGoldNum());
	umeng::MobClickCpp::event(getStringFromInt(YM_GOLD_NUM).c_str(), &dict);*/
#endif
}
