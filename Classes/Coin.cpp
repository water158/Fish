//
//  Coin.cpp
//  BuyuDemo
//
//  Created by etgame on 15-4-20.
//
//

#include "Coin.h"
#include "MusicData.h"
#include "Data.h"
Coin::Coin()
{
	log("Coin::Coin()");
}
Coin::~Coin()
{
	log("Coin::~Coin()");
}
Coin* Coin::createCoin( int _fKind, Point _oriPoint, Point _dispoint)
{
    Coin* pRet = new Coin();
    if (pRet&&pRet->initCoin(_fKind, _oriPoint, _dispoint)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool Coin::initCoin( int _fKind, Point _oriPoint, Point _dispoint)
{
    
    c_num = _fKind;
    oriPoint = _oriPoint;
    disPoint = _dispoint;
    num = 0;
    
    
    CCLOG("c_num = %d", c_num);
	if(c_num > 40)
	{
		MessageBox("数组越界了", "");
	}
    for (int i = 0; i < fishCoin[c_num]; i++)
    {
        Point c_ori_point = Point(20*(i-1), 0);
        
        if (c_num < 20&&c_num >= 0)
        {
            Sprite* fish_coin = Sprite::createWithSpriteFrameName("silver_coin1.png");
            fish_coin->setPosition(c_ori_point);
            this->addChild(fish_coin);
            num = 1;
            this->setAnimation();
            fish_coin->runAction(coinAnimate);
        }else if (c_num <= 41 && c_num >= 20)
        {
            Sprite* fish_coin = Sprite::createWithSpriteFrameName("golden_coin1.png");
            fish_coin->setPosition(c_ori_point);
            this->addChild(fish_coin);
            num = 2;
            this->setAnimation();
            fish_coin->runAction(coinAnimate);
        }
        
        
    }
//    //CCLOG("c_num = %d", c_num);
//    //this->initWithSpriteFrameName("silver_coin1.png");
//    if (c_num < 20&&c_num >=0) {
//        this->initWithSpriteFrameName("silver_coin1.png");
//        num = 1;
//    }
//    if (c_num <= 41 && c_num >= 20) {
//        this->initWithSpriteFrameName("golden_coin1.png");
//        num = 2;
//    }
    this->setPosition(oriPoint);
    
    
    this->setMove();
    this->setScale(1.0f);
    return true;
}
void Coin::setAnimation()
{
    auto animation = Animation::create();
    std::string coin_str;
    if (num == 1) {
        coin_str = "silver_coin";
    }
    if (num == 2) {
        coin_str = "golden_coin";
    }
   // CCLOG("coin_str = %s", coin_str.c_str());
    for (int i = 1; i <= 9; i++) {
        __String* coin_name = __String::createWithFormat("%s%d.png", coin_str.c_str(), i);
       
        //log("coin_name = %s", coin_name->getCString());
        SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(coin_name->getCString());
        animation->addSpriteFrame(frame);
    }
    animation->setDelayPerUnit(0.1);
    animation->setRestoreOriginalFrame(true);
    animation->setLoops(-1);
    coinAnimate = Animate::create(animation);
    
}
void Coin::setMove()
{
    float time = sqrtf((disPoint.x-oriPoint.x)*(disPoint.x-oriPoint.x)+(disPoint.y-oriPoint.y)*(disPoint.y-oriPoint.y))/200 ;
    MoveTo* moveto = MoveTo::create(time, disPoint);
    EaseIn* easeMove = EaseIn::create(moveto, 10.0f);
    
 //   ScaleTo* scaleBig = ScaleTo::create(0.3*time, 1.5f);
 //   ScaleTo* scaleSmall = ScaleTo::create(0.7*time, 0.5f);
 //   Sequence* scale = Sequence::create(scaleBig, scaleSmall, nullptr);
 //   
 //   MoveBy* moveby = MoveBy::create(0.5, Vec2(0, 50));
	//EaseIn* easeMove1 = EaseIn::create(moveby, 5.0f);
 //   Sequence* moveSqe = Sequence::create(easeMove1, easeMove1->reverse(), nullptr);
 //   Repeat* by_repeat = Repeat::create(moveSqe, 1);
 //   Spawn* move = Spawn::create( easeMove, /*scale, */nullptr);
    CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Coin::removeCoin, this));
    
    Sequence* action = Sequence::create(easeMove, CallFunc::create([&]{this->addSound();}), callfunc, nullptr);
    this->runAction(action);
    
}
void Coin::addSound()
{
    MusicData* music = MusicData::getInstance();
    music->playSound(37);
	//log("----------playSound(37);---------------");
}
void Coin::removeCoin()
{
    this->removeFromParent();
}