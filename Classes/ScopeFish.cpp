//
//  ScopeFish.cpp
//  Qianpaobuyu
//
//  Created by etgame on 15-5-15.
//
//

#include "ScopeFish.h"
#include "FishPath.h"
#include "Manager.h"
#include "PixCell.h"
#include "Coin.h"
#include "Data.h"
ScopeFish::ScopeFish()
{
	log("ScopeFish::ScopeFish()");
}
ScopeFish::~ScopeFish()
{
    log("ScopeFish::~ScopeFish()");
}

ScopeFish* ScopeFish::createScopeFish()
{
    ScopeFish* pRet = new ScopeFish();
    if (pRet&&pRet->init()) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}
bool ScopeFish::initScopeFish(int _kind)
{
    isSetbody = true;
    size = Director::getInstance()->getWinSize();
    FishPath* path = new FishPath();
    path->fishPah();
    this->initFishBase(_kind);
    fb_startPoint = path->startPoint;
    fb_endPoint = path->endPoint;
    fb_controlPoint1 = path->controlPoint1;
    fb_controlPoint2 = path->controlPoint2;

	log("delete path;5");
    //delete path;
	CC_SAFE_DELETE(path);
    return true;
}
void ScopeFish::moveWithBezier()
{
    int xChange = rand()%200 - 150;
    int yChange = rand()%200 - 100;
    float sx = fb_startPoint.x + xChange;//起点x
    float sy = fb_startPoint.y + yChange;//起点y
    Point sp = Point(sx, sy);//起点位置
    
    
    int ex = fb_endPoint.x + rand()%100 - 40;
    int ey = fb_endPoint.y + rand()%100 - 100;
    Point ep = Point(ex, ey);//终点坐标
    
    int c1x = fb_controlPoint1.x + xChange;//控制点x
    int c1y = fb_controlPoint1.y + yChange;
    Point c1p = Point(c1x, c1y);
    
    int c2x = fb_controlPoint2.x + xChange;
    int c2y = fb_controlPoint2.y + yChange;
    Point c2p = Point(c2x, c2y);
    
    this->setPosition(sp);//精灵的初始位置
    
    
    //动作设置 贝塞尔曲线
    ccBezierConfig bezier;
    if (fb_controlPoint2.equals(Point::ZERO)) {
        bezier.controlPoint_1 = sp;
        bezier.controlPoint_2 = c1p;
        bezier.endPosition = ep;
    }else{
        bezier.controlPoint_1 = c1p;
        bezier.controlPoint_2 = c2p;
        bezier.endPosition = ep;
    }
    BezierTo* actionMove = BezierTo::create(40, bezier);
    //动作执行完 精灵删除 、、、注、、、易造成二次删除
    CallFunc* callFunc = CallFunc::create(CC_CALLBACK_0(FishBase::removeAllThings, this));
    
    Sequence* sequence = CCSequence::create(actionMove, callFunc, nullptr);
    moveSpeed = Speed::create(sequence, 1.0f);
    moveSpeed->retain();
    this->runAction(moveSpeed);
    //speed->setTag(kTagFishMoveAction);
    this->scheduleUpdate();
}
void ScopeFish::update(float dt)
{
    
    sub = this->getPosition() - oPoint;
    float angle = sub.getAngle();
    float degress = CC_RADIANS_TO_DEGREES(-1* angle)-90;
    this->setRotation(degress);
    
    oPoint = this->getPosition();
    //CCLog("x = %f, y = %f", oPoint.x, oPoint.y);
    //this->setPosition(oPoint);
    
    if (isCatch) {
        setIsCatch(false);
        this->checkNearestFish();
        //instance->setM_totalCoin(instance->getM_totalCoin() + fish->value*bullet->bulletPower);
        this->theSpecialEffects();
        this->catchAnimation(fb_Kind);
//        if (bindToCannonNum == 1) {
//            instance->setM_totalCoin(instance->getM_totalCoin() + value*(instance->getM_bulletPower()));
//        }
    }
    
}
void ScopeFish::checkNearestFish()
{
    
    
    for (auto &fish : instance->m_fishVect)
    {
        FishBase* fish_temp = dynamic_cast<FishBase*>(fish);
		fish_temp->resumeSchedulerAndActions();
        fish_temp->setIsCatch(true);
    }
    
    
}
//void ScopeFish::removeScopeFish()
//{
//    instance->m_fishVect.eraseObject(this);
//    this->removeFishBody();
//    this->removeFromParent();
//    
//}
