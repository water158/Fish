//
//  FishBoom.cpp
//  BuyuDemo
//
//  Created by etgame on 15-5-7.
//
//

#include "FishBoom.h"
#include "Manager.h"
const int hPath[][6] = {
    {-300, 100, 940, 100},
    {-300, 200, 980, 200},
    {-300, 300, 980, 300},
    {-300, 400, 940, 400},
    {300, 540, 300, -100},
    {500, 540, 500, -100},
    {300, -100, 300, 400},
    {500, -100, 500, 400},
    
};
FishBoom::FishBoom()
{
	log("FishBoom::FishBoom()");
}
FishBoom::~FishBoom()
{
	log("FishBoom::~FishBoom()");
}
FishBoom* FishBoom::createFishBoom(int _kind)
{
    FishBoom* pRet = new FishBoom();
    if (pRet&&pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}
bool FishBoom::initFishBoom(int _kind)
{
    this->initFishBase(_kind);
    
    this->scheduleUpdate();
    
    
    return true;
}
void FishBoom::runCircle()
{
    
}
void FishBoom::runInBoom(int roadNum)
{
    Point startPoint, endPoint;
    switch (fb_Kind) {
        case 16:
            startPoint = Point(hPath[0][0], hPath[0][1]);
            endPoint = Point(hPath[0][2], hPath[0][3]);
            this->setRotation(90);
            
            break;
        case 20:
            startPoint = Point(hPath[1][0], hPath[1][1]);
            endPoint = Point(hPath[1][2], hPath[1][3]);
            this->setRotation(90);
            break;
            
        case 12:
            startPoint = Point(hPath[2][0], hPath[2][1]);
            endPoint = Point(hPath[2][2], hPath[2][3]);
            this->setRotation(90);
            break;
            
        case 15:
            startPoint = Point(hPath[3][0], hPath[3][1]);
            endPoint = Point(hPath[3][2], hPath[3][3]);
            this->setRotation(90);
            break;
        case 0:
            startPoint = Point(hPath[4][0], hPath[4][1]);
            endPoint = Point(hPath[4][2], hPath[4][3]);
            this->setRotation(180);
            break;
        case 3:
            startPoint = Point(hPath[5][0], hPath[5][1]);
            endPoint = Point(hPath[5][2], hPath[5][3]);
            this->setRotation(180);
            break;
            
        default:
            break;
            
    }
    MoveTo* to = NULL;
    if (roadNum == 0) {
        this->setPosition(endPoint);
        to = MoveTo::create(10, startPoint);
    }else if (roadNum == 1){
        this->setPosition(startPoint);
        to = MoveTo::create(10, endPoint);
    }
    
    Sequence* seq = Sequence::create(to, CallFunc::create(CC_CALLBACK_0(FishBoom::removeBoom, this)), nullptr);
    this->runAction(seq);
}
void FishBoom::update(float dt)
{
    graySprite->setRotation(this->getRotation());
    graySprite->setPosition(this->getPosition()-Point(20, 40));
}
void FishBoom::removeBoom()
{
    instance->m_fishVect.eraseObject(this);
    this->removeFishBody();
    graySprite->removeFromParentAndCleanup(true);
    this->removeFromParentAndCleanup(true);
}