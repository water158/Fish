//
//  FishGroup.cpp
//  Qianpaobuyu
//
//  Created by etgame on 15-5-15.
//
//

#include "FishGroup.h"
#include "FishPath.h"
#include "Manager.h"
#include "PixCell.h"
#include "Coin.h"
#include "Data.h"
FishGroup::FishGroup()
{
	log("FishGroup::FishGroup()");
}
FishGroup::~FishGroup()
{
    log("FishGroup::~FishGroup()");
}

FishGroup* FishGroup::createFishGroup()
{
    FishGroup* pRet = new FishGroup();
    if (pRet&&pRet->init()) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}
bool FishGroup::initFishGroup(int _num, int _fishNum)
{
    isSpecial = true;
    isSetbody = true;
    //isGroup = true;
    groupNum = _num;
    fishNum = _fishNum;
    size = Director::getInstance()->getWinSize();
    FishPath* path = new FishPath();
    path->fishPah();
    this->initFishBase(fishNum);
    fb_startPoint = path->startPoint;
    fb_endPoint = path->endPoint;
    fb_controlPoint1 = path->controlPoint1;
    fb_controlPoint2 = path->controlPoint2;
   if (groupNum == YIWANGDAJIN) {
        bottom_sp = Sprite::createWithSpriteFrameName("task_icon_34.png");
        bottom_sp->setScale(0.6f);
        
    }else if (groupNum == TONGLEIZHAIDAN) {
        bottom_sp = Sprite::createWithSpriteFrameName("task_icon_5.png");
    }
    this->addChild(bottom_sp,1);
    RotateBy* rotate = RotateBy::create(3, 360);
    RepeatForever* forever = RepeatForever::create(rotate);
    bottom_sp->runAction(forever);
    //this->addChild(basisSp, 2);
    log("delete path;3");
    //delete path;
	CC_SAFE_DELETE(path);
    this->scheduleUpdate();
    return true;
}
void FishGroup::moveWithBezier()
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
    BezierTo* actionMove = BezierTo::create(10, bezier);
    //动作执行完 精灵删除 、、、注、、、易造成二次删除
    CallFunc* callFunc = CallFunc::create(CC_CALLBACK_0(FishBase::removeAllThings, this));
    
    Sequence* sequence = CCSequence::create(actionMove, callFunc, nullptr);
    moveSpeed = Speed::create(sequence, 1.0f);
    moveSpeed->retain();
    this->runAction(moveSpeed);

	//this->enterSpeedUp();
    //speed->setTag(kTagFishMoveAction);
    this->scheduleUpdate();
}
void FishGroup::update(float dt)
{
    if (groupNum != SANYUAN||groupNum != SIXI) {
        sub = this->getPosition() - oPoint;
        float angle = sub.getAngle();
        float degress = CC_RADIANS_TO_DEGREES(-1* angle)-90;
        this->setRotation(degress);
        
        oPoint = this->getPosition();
    }
    
    //CCLog("x = %f, y = %f", oPoint.x, oPoint.y);
    //this->setPosition(oPoint);
    
    if (isCatch) {
        setIsCatch(false);
       
        this->theSpecialEffects();

        if (groupNum == YIWANGDAJIN) {
            for (auto &fish : instance->Y_dajinVect)
            {
                FishGroup* fish_temp = dynamic_cast<FishGroup*>(fish);
                fish_temp->setIsCatch(true);
                fish_temp->setBindToCannonNum(bindToCannonNum);
            }
        }else if (groupNum == TONGLEIZHAIDAN) {
            for (auto &fish : instance->m_fishVect)
            {
                FishBase* fish_temp = dynamic_cast<FishBase*>(fish);
                if (fish_temp->fb_Kind == fb_Kind) {
                    fish_temp->setIsCatch(true);
                    fish_temp->setBindToCannonNum(bindToCannonNum);
                }
            }
        }
        
//        if (bindToCannonNum == 1) {
//            instance->setM_totalCoin(instance->getM_totalCoin() + value*(instance->getM_bulletPower()));
//        }
        this->catchAnimation(fb_Kind);
    }
    
}

void FishGroup::catchAnimation(int fishType)
{
	FishBase::catchAnimation(fishType);

	bottom_sp->runAction(FadeTo::create(2, 0.1f));
}


