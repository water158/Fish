//
//  MyMenu.cpp
//  Qianpaobuyu
//
//  Created by etgame on 15-5-19.
//
//

#include "MyMenu.h"
MyMenu::MyMenu():isChosed(false)
{}
MyMenu::~MyMenu()
{
}
MyMenu* MyMenu::createMyMenu()
{
    MyMenu* pRet = new MyMenu();
    if (pRet&&pRet->initMyMenu()) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool MyMenu::initMyMenu()
{
    
    
    FadeIn* wait_in = FadeIn::create(2);
    FadeOut* wait_out = FadeOut::create(2);
    Sequence* wait_seq = Sequence::create(wait_in, wait_out, nullptr);
    RepeatForever* wait_forever = RepeatForever::create(wait_seq);
    this->runAction(wait_forever);
    return true;
}
bool MyMenu::initWithArray( const Vector<MenuItem*>& arrayOfItems )
{
    if (Layer::init())  {
        _enabled = true;
        // menu in the center of the screen
        Size s = Director::getInstance()->getWinSize();
        this->ignoreAnchorPointForPosition(true);
        setAnchorPoint(Vec2(0.5f, 0.5f));
        this->setContentSize(s);
        setPosition(Vec2(s.width/2, s.height/2));
        int z=0;
        for (auto& item : arrayOfItems)     {
            this->addChild(item, z);
            z++;
        }
        _selectedItem = nullptr;
        _state = Menu::State::WAITING;
        // enable cascade color and opacity on menus
        setCascadeColorEnabled(true);
        setCascadeOpacityEnabled(true);
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->setSwallowTouches(true);
        touchListener->onTouchBegan = CC_CALLBACK_2(Menu::onTouchBegan, this);
        touchListener->onTouchMoved = CC_CALLBACK_2(Menu::onTouchMoved, this);
        touchListener->onTouchEnded = CC_CALLBACK_2(Menu::onTouchEnded, this);
        touchListener->onTouchCancelled = CC_CALLBACK_2(Menu::onTouchCancelled, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
        
        FadeIn* wait_in = FadeIn::create(2);
        FadeOut* wait_out = FadeOut::create(2);
        Sequence* wait_seq = Sequence::create(wait_in, wait_out, nullptr);
        RepeatForever* wait_forever = RepeatForever::create(wait_seq);
        this->runAction(wait_forever);
        return true;
    }
    return false;
}
MyMenu* MyMenu::createWithArray( const Vector<MenuItem*>& arrayOfItems )
{
    auto ret = new MyMenu();
    if (ret && ret->initWithArray(arrayOfItems)) {
        ret->autorelease();
    }   else    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

MyMenu* MyMenu::createWithItem( MenuItem* item )
{
    return MyMenu::create(item, nullptr);
}

MyMenu* MyMenu::create( MenuItem* item, ... )
{
    va_list args;
    va_start(args,item);
    MyMenu *ret = MyMenu::createWithItems(item, args);
    va_end(args);
    return ret;
    
    
}
MyMenu* MyMenu::createWithItems( MenuItem *item, va_list args )
{
    Vector<MenuItem*> items;
    if( item )  {
        items.pushBack(item);
        MenuItem *i = va_arg(args, MenuItem*);
        while(i)        {
            items.pushBack(i);
            i = va_arg(args, MenuItem*);
        }
    }
    return MyMenu::createWithArray(items);
}
