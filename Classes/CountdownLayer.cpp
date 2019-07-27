//
//  CountdownLayer.cpp
//  Qianpaobuyu
//
//  Created by etgame on 15-6-5.
//
//

#include "CountdownLayer.h"
#include "MenuLayer.h"
#include "Manager.h"
#include "SpellNumber.h"
CountdownLayer::CountdownLayer()
{}
CountdownLayer::~CountdownLayer()
{}
CountdownLayer* CountdownLayer::createCountdownLayer()
{
    CountdownLayer* pRet = new CountdownLayer();
    if (pRet&&pRet->initCountdown()) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}
bool CountdownLayer::initCountdown()
{
    this->initWithColor(Color4B(0, 0, 0, 78));
    recordCountdown = 30;
    size = Director::getInstance()->getWinSize();
    this->schedule(schedule_selector(CountdownLayer::countDown), 1);
    
    XMLParser* xmlparser = XMLParser::parseWithFile("ui/loading.xml");
	
	__String* strID = __String::createWithFormat("mainscene_tip_countdown");
	__String* info = xmlparser->getString(strID->getCString());
    
    Sprite* notice_bg = Sprite::create("notice_bg.png");
    notice_bg->setPosition(size.width/2, size.height/2);
    notice_bg->setScale(1.5);
    this->addChild(notice_bg, 1);
    
    Label* warming = Label::createWithSystemFont(info->getCString(), "myFontName", 20);
    warming->setPosition(size.width/2, size.height/2);
    this->addChild(warming, 2);
    return true;
}
void CountdownLayer::countDown(float dt)
{
    recordCountdown--;
    int num_count = recordCountdown;
    int i = 0;
    std::string str = getStringFromInt(num_count);
    int numLong = strlen(str.c_str());
    
    if (this->getChildByTag(11)) {
        this->getChildByTag(11)->removeFromParent();
    }
    
//    Node* num_node = Node::create();
//    num_node->setTag(11);
//    while (num_count) {
//        __String* num_str = __String::createWithFormat("yellow_%d.png", num_count%10);
//        Sprite* num_sp = Sprite::createWithSpriteFrameName(num_str->getCString());
//        
//        if (numLong == 1)
//        {
//           num_sp->setPosition(Point(0, 0));
//        }else if (numLong == 2)
//        {
//            num_sp->setPosition(Point(10*sin(M_PI/2 + i*M_PI), 0));
//        }
//        i++;
//        num_node->addChild(num_sp, 1);
//        
//        num_count /= 10;
//    }
//    num_node->setPosition(Vec2(size.width/2+62, size.height/2));
//    this->addChild(num_node, 2);
    
    SpellNumber* spellNum = SpellNumber::createSpellNumber(num_count);
    spellNum->setPosition(Vec2(size.width/2+67, size.height/2));
    spellNum->setTag(11);
    this->addChild(spellNum, 2);
    
   
    if (recordCountdown == 0)
    {
        auto scene = Scene::create();
        MenuLayer* layer = MenuLayer::create();
        if (Manager::getInstance()->getRoomNum() == 1)
        {
            layer->showSingle();
        }
        else
        {
            layer->showMany();
        }
        scene->addChild(layer);
        Director::getInstance()->replaceScene(scene);
    }
    
//    if (isWake) {
//        this->removeFromParentAndCleanup(true);
//    }
}


//#pragma mark 触摸事件
//void CountdownLayer::onEnter()
//{
//    CCLayer::onEnter();
//    
//    
//    auto dispatcher = Director::getInstance()->getEventDispatcher();
//    auto listener = EventListenerTouchOneByOne::create();
//    listener->onTouchBegan = CC_CALLBACK_2(CountdownLayer::onTouchBegan,this);
//    listener->onTouchMoved = CC_CALLBACK_2(CountdownLayer::onTouchMoved,this);
//    listener->onTouchEnded = CC_CALLBACK_2(CountdownLayer::onTouchEnded,this);
//    listener->setSwallowTouches(true);//不向下传递触摸
//    dispatcher->addEventListenerWithSceneGraphPriority(listener,this);
//    //this->scheduleUpdate();
//    
//    
//    
//}
//void CountdownLayer::onExit()
//{
//    
//   
//    CCLayer::onExit();
//}
//
//bool CountdownLayer::onTouchBegan(Touch *touch, Event *unused_event)
//{
//    isWake = true;
//    
//    return true;
//}
//void CountdownLayer::onTouchMoved(Touch *touch, Event *unused_event)
//{
//    
//}
//void CountdownLayer::onTouchEnded(Touch *touch, Event *unused_event)
//{
//   
//}
//void CountdownLayer::onTouchCancelled(Touch *touch, Event *unused_event)
//{
//    
//}
