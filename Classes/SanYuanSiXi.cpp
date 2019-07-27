//
//  SanYuanSiXi.cpp
//  Qianpaobuyu
//
//  Created by etgame on 15-5-20.
//
//

#include "SanYuanSiXi.h"
#include "FishGroup.h"
#include "Data.h"
#include "Manager.h"
#include "FishPath.h"
#include "UserData.h"
//#include "PhysicsBodyNew.h"
const float sanyuansixi[2][3] = {{1100, 400, -100}, {-1100, 400, 850}};
SanYuanSiXi::SanYuanSiXi()
{
	log("SanYuanSiXi::SanYuanSiXi()");
}
SanYuanSiXi::~SanYuanSiXi()
{
    log("SanYuanSiXi::~SanYuanSiXi()");
}

SanYuanSiXi* SanYuanSiXi::createSanYuanSiXi()
{
    SanYuanSiXi* pRet = new SanYuanSiXi();
    if (pRet&&pRet->init()) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}
bool SanYuanSiXi::initSanYuanSiXi(int _num, int _fishNum)
{
    choseOne = _num;
    fb_Kind = _fishNum;
    isSpecial = true;
    isSetbody = true;
    //isGroup = true;
    
    this->initFishBase(fb_Kind);
    
    
    bottom_sp = Sprite::createWithSpriteFrameName("task_icon_7.png");
    this->addChild(bottom_sp,1);
    RotateBy* rotate = RotateBy::create(3, 360);
    RepeatForever* forever = RepeatForever::create(rotate);
    bottom_sp->runAction(forever);
    //this->addChild(basisSp, 2);
    this->scheduleUpdate();
    return true;
}

void SanYuanSiXi::update(float dt)
{
    sub = this->getPosition() - oPoint;
    float angle = sub.getAngle();
    float degress = CC_RADIANS_TO_DEGREES(-1* angle)-90;
    this->setRotation(degress);
    
    oPoint = this->getPosition();
    
    if (isCatch) {
//        if (bindToCannonNum == 1) {
//            //instance->setM_totalCoin(instance->getM_totalCoin() + value*(instance->getM_bulletPower()));
//            UserData::getInstance()->setGoldNum(UserData::getInstance()->getGoldNum() + value*(instance->getM_bulletPower()));
//        }
        this->theSpecialEffects();
        this->catchAnimation(fb_Kind);;
        
    }
    
}

void SanYuanSiXi::catchAnimation(int fishType)
{
	FishBase::catchAnimation(fishType);

	FadeTo* to = FadeTo::create(2, 0.1f);

	bottom_sp->runAction(to);
}
