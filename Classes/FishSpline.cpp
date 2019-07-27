//
//  FishSpline.cpp
//  BuyuDemo
//
//  Created by etgame on 15-5-7.
//
//

#include "FishSpline.h"
#include "FishPath.h"
#include "Manager.h"
#include "Data.h"
#include "Coin.h"
FishSpline::FishSpline()
{
	log("FishSpline::FishSpline()");
}
FishSpline::~FishSpline()
{
	if(fb_splineArray)
	{
		fb_splineArray->release();
	}
    log("FishSpline::~FishSpline()");
}
FishSpline* FishSpline::createFishSpline(int _kind)
{
    FishSpline* pRet = new FishSpline();
    if (pRet&&pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}
bool FishSpline::initFishSpline(int _kind, bool isBodies)
{
    //log("isBodies = %d", isBodies);
    isSetbody = isBodies;
    this->initFishBase(_kind);
   size = Director::getInstance()->getWinSize();
    
    FishPath* path = new FishPath();
    path->splinePath();
    //fb_splineArray = path->splineArray;
	fb_splineArray = PointArray::create(path->splineArray->count());
	fb_splineArray->retain();
	for (int i = 0; i < path->splineArray->count(); i++)
	{
		Vec2 splinePos = path->splineArray->getControlPointAtIndex(i);
		fb_splineArray->addControlPoint(splinePos);
	}
    log("delete path;4");
    //delete path;
	CC_SAFE_DELETE(path);
    return true;
}
void FishSpline::moveSpline()
{
    this->setPosition(fb_splineArray->getControlPointAtIndex(0));

    cardinalSplineto = CCCardinalSplineTo::create(35, fb_splineArray, 0);
    
//    CallFunc* callFunc_reverse = CCCallFunc::create(CC_CALLBACK_0(FishSpline::callReverse, this));
//    //CallFunc* callFunc = CCCallFunc::create(CC_CALLBACK_0(FishSpline::removeSelf, this));
//    Sequence* sequence = CCSequence::create(cardinalSplineto, callFunc_reverse, nullptr);
//    Repeat* repeat = Repeat::create(sequence, 2);
    
    CallFunc* callFunc = CCCallFunc::create(CC_CALLBACK_0(FishSpline::removeFish, this));
    Sequence* seq = Sequence::create(cardinalSplineto, callFunc, nullptr);
   // RepeatForever* forever = RepeatForever::create(sequence);
    moveSpeed = Speed::create(seq, 1.0f);
	moveSpeed->retain();
    this->runAction(moveSpeed);
    //seq->setTag(kTargetAndroid);
    oPoint = fb_splineArray->getControlPointAtIndex(0) - Point(5, 5);

	this->enterSpeedUp();

    this->scheduleUpdate();
}
void FishSpline::update(float dt)
{
    sub = this->getPosition() - oPoint;
    
    float angle = sub.getAngle();
    float degress = CC_RADIANS_TO_DEGREES(-1* angle) - 90;
    
    this->setRotation(degress);
    
    oPoint = this->getPosition();
    
    
    
    graySprite->setRotation(degress);
    graySprite->setPosition(this->getPosition().x - 10 , this->getPosition().y-20);
    if (isCatch) 
	{
        this->theSpecialEffects();
//        if (bindToCannonNum == 1) {
//            instance->setM_totalCoin(instance->getM_totalCoin() + value*(instance->getM_bulletPower()));
//        }
        this->catchAnimation(fb_Kind);
        setIsCatch(false);
    }
}
void FishSpline::callReverse()
{
    cardinalSplineto->reverse();
}

