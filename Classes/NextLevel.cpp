//
//  NextLevel.cpp
//  Qianpaobuyu
//
//  Created by etgame on 15-6-5.
//
//

#include "NextLevel.h"
#include "MenuLayer.h"
#include "MainScene.h"
NextLevel::NextLevel()
{}
NextLevel::~NextLevel()
{}
NextLevel* NextLevel::createNextLevel()
{
    NextLevel* pRet = new NextLevel();
    if (pRet&&pRet->initNextLevel()) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}
bool NextLevel::initNextLevel()
{
    this->initWithColor(Color4B(0, 0, 0, 78));
    size = Director::getInstance()->getWinSize();
    
    XMLParser* xmlparser = XMLParser::parseWithFile("ui/loading.xml");
	
	__String* strID = __String::createWithFormat("mainscene_tip_1");
	__String* info = xmlparser->getString(strID->getCString());
	    
    Label* n_label = Label::createWithSystemFont(info->getCString(), "myFontName", 20);
    n_label->setPosition(Vec2(400, 200));
    this->addChild(n_label);
    Sprite* sp_nomal = Sprite::createWithSpriteFrameName("money_bg.png");
    Sprite* sp_select = Sprite::createWithSpriteFrameName("money_bg.png");
    MenuItemSprite* menu_item = MenuItemSprite::create(sp_nomal, sp_select, CC_CALLBACK_1(NextLevel::inNext, this));
    Menu* doIt = Menu::create(menu_item, nullptr);
    doIt->setPosition(size.width/2, size.height/2);
    this->addChild(doIt);
    
    return true;
}

void NextLevel::inNext(Ref* Obj)
{
    int nextNum = Manager::getInstance()->getTwo_LevelNum() + 1;
    if (nextNum > 4) 
	{
        return;
    }
    Manager::getInstance()->m_fishVect.clear();
    Manager::getInstance()->setRoomNum(2);
    Manager::getInstance()->setTwo_LevelNum(nextNum);
    auto pScene = MainScene::scene();
	Director::getInstance()->replaceScene(pScene);
}