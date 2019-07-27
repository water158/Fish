//
//  EnergyCannonCard.cpp
//  Qianpaobuyu
//
//  Created by etgame on 15-5-15.
//
//

#include "EnergyCannonCard.h"
#include "FishPath.h"
#include "Manager.h"
EnergyCannonCard::EnergyCannonCard()
{}
EnergyCannonCard::~EnergyCannonCard()
{
    
}

EnergyCannonCard* EnergyCannonCard::createEnergyCannonCard()
{
    EnergyCannonCard* pRet = new EnergyCannonCard();
    if (pRet&&pRet->init()) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}
bool EnergyCannonCard::initEnergyCannonCard(int _kind)
{
    fb_Kind = _kind;
   isSetbody = true;
    FishPath* path = new FishPath();
    path->fishPah();
    this->initFishBase(fb_Kind);
    fb_startPoint = path->startPoint;
    fb_endPoint = path->endPoint;
    fb_controlPoint1 = path->controlPoint1;
    fb_controlPoint2 = path->controlPoint2;
	log("delete path;1");
	CC_SAFE_DELETE(path);
    //delete path;
    return true;
}
void EnergyCannonCard::moveWithBezier()
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
    BezierTo* actionMove = BezierTo::create(30, bezier);
    //动作执行完 精灵删除 、、、注、、、易造成二次删除
    CallFunc* callFunc = CallFunc::create(CC_CALLBACK_0(FishBase::removeAllThings, this));
    
    Sequence* sequence = CCSequence::create(actionMove, callFunc, nullptr);
    moveSpeed = Speed::create(sequence, 1.0f);
    moveSpeed->retain();
    this->runAction(moveSpeed);
    //speed->setTag(kTagFishMoveAction);

    this->scheduleUpdate();
}
void EnergyCannonCard::update(float dt)
{
    
    sub = this->getPosition() - oPoint;
    //float angle = sub.getAngle();
    //float degress = CC_RADIANS_TO_DEGREES(-1* angle)-90;
    //this->setRotation(degress);
    
    oPoint = this->getPosition();
    //CCLog("x = %f, y = %f", oPoint.x, oPoint.y);
    //this->setPosition(oPoint);
    
    if (isCatch) {
        this->removeFish();
    }
   
    
}

//void EnergyCannonCard::removeEnergyCannonCard()
//{
//    instance->m_fishVect.eraseObject(this);
//    this->removeFishBody();
//    this->removeFromParent();
//    
//}
