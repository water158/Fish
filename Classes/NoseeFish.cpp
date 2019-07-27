//
//  NoseeFish.cpp
//  Qianpaobuyu
//
//  Created by etgame on 15-6-26.
//
//

#include "NoseeFish.h"
#include "Manager.h"
NoseeFish::NoseeFish()
{}
NoseeFish::~NoseeFish()
{}
NoseeFish*  NoseeFish::createNoseeFish()
{
    NoseeFish* pRet = new NoseeFish();
    if (pRet && pRet->init())
	{
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}
bool NoseeFish::initNoseeFish(int kindNum)
{
	fb_Kind = 1000;
    Size size = Director::getInstance()->getWinSize();
    isGray = false;
    this->setIsGray(false);
    this->isSetbody = false;
    //this->initFishBase(10);
    
    switch (kindNum) 
	{
        case 1:
        {
            //this->setPosition(Vec2(size.width/2, size.height/2));
            CallFunc* func1 = CallFunc::create([&]{
                this->removeFromParent();
                Manager::getInstance()->m_fishVect.eraseObject(this);
                Manager::getInstance()->setIsLockFish(true);
            });
            Sequence* seq1 = Sequence::create(DelayTime::create(20), func1, nullptr);
            moveSpeed = Speed::create(seq1, 1.0f);
			moveSpeed->retain();
            this->runAction(moveSpeed);
        }
            break;
        case 2:
        {
            //this->setPosition(Vec2(0, size.height/2));
            MoveBy* by = MoveBy::create(5, Vec2(size.width, 0));
            Sequence* seq_by = Sequence::create(by, by->reverse(), nullptr);
            Repeat* n_repeat = Repeat::create(seq_by, 4);
            CallFunc* func1 = CallFunc::create([&]{
                this->removeFromParent();
                Manager::getInstance()->m_fishVect.eraseObject(this);
                Manager::getInstance()->setIsLockFish(true);
            });
            Sequence* seq1 = Sequence::create(n_repeat, func1, nullptr);
            moveSpeed = Speed::create(seq1, 1.0f);
			moveSpeed->retain();
            this->runAction(moveSpeed);
        }
            break;
        default:
            break;
    }
    
    return true;
}