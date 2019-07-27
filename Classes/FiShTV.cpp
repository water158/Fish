//
//  FiShTV.cpp
//  Qianpaobuyu
//
//  Created by etgame on 15-5-14.
//
//

#include "FiShTV.h"
#include "FishSpline.h"
#include "FishBezier.h"
#include "Manager.h"
static int timer = 0;
FishTV* FishTV::createFishTV()
{
    FishTV* pRet = new FishTV();
    if (pRet && pRet->initFishTV()) 
	{
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}
bool FishTV::initFishTV()
{
	Layer::init();

	timer = 0;
    this->setContentSize(Size(854, 480));
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("fish_frame_1.plist", "fish_frame_1.png");
    cache->addSpriteFramesWithFile("fish_frame_28.plist", "fish_frame_28.png");
    cache->addSpriteFramesWithFile("fish_frame_29.plist", "fish_frame_29.png");
    cache->addSpriteFramesWithFile("fish_frame_2.plist", "fish_frame_2.png");
    cache->addSpriteFramesWithFile("fish_frame_3.plist", "fish_frame_3.png");
    cache->addSpriteFramesWithFile("fish_frame_4.plist", "fish_frame_4.png");
    cache->addSpriteFramesWithFile("fish_frame_18.plist", "fish_frame_18.png");
    cache->addSpriteFramesWithFile("fish_frame_31.plist", "fish_frame_31.png");
    cache->addSpriteFramesWithFile("fish_frame_166.plist", "fish_frame_166.png");
    cache->addSpriteFramesWithFile("fish_frame_6.plist", "fish_frame_6.png");
    //电视 鱼
   
    this->schedule(schedule_selector(FishTV::addFishTV), 0.05f);
    return true;
}

void FishTV::addFishTV(float dt)
{
	log("---getChildrenCount----(%d)---------------", this->getChildrenCount());
	if(this->getChildrenCount() > 100)
		return;

    bool bLoading = Manager::getInstance()->getEnterLoading();
	if(bLoading)return;
    int fishNum = timer%15;
    if (fishNum < 5) 
	{
        FishSpline* fish1 = FishSpline::createFishSpline(fishNum);
        this->addChild(fish1);
        fish1->initFishSpline(fishNum, false);
        fish1->moveSpline();
		fish1->setTVFish(true);
        
        DelayTime* delay = CCDelayTime::create(0.5);
        CallFuncN* funcN = CallFuncN::create(CC_CALLBACK_1(FishTV::addFishSplineTV, this, fish1->fb_splineArray));
        Sequence* seq = Sequence::create(delay, funcN,delay, funcN,delay, funcN,  nullptr);
        fish1->runAction(seq);
        
        FishBezier* fish_bezier = FishBezier::createFishBezier(fishNum);
        this->addChild(fish_bezier, fishNum);
        fish_bezier->initFishBezier(fishNum, false);
        fish_bezier->moveWithBezier();
		fish_bezier->setTVFish(true);

        for (int i = 0; i < 5; i++) 
		{
            FishBezier* fish_group = FishBezier::createFishBezier(fishNum);
            this->addChild(fish_group, fishNum);
            fish_group->initFishBezier(fishNum, false);
            fish_group->fb_startPoint = fish_bezier->fb_startPoint;
            fish_group->fb_endPoint = fish_bezier->fb_endPoint;
            fish_group->fb_controlPoint1 = fish_bezier->fb_controlPoint1;
            fish_group->fb_controlPoint2 = fish_bezier->fb_controlPoint2;
            fish_group->moveWithBezier();
			fish_group->setTVFish(true);
        }
        
    }
	else if (fishNum > 5 && fishNum < 10)
    {
        FishBezier* fish = FishBezier::createFishBezier(fishNum);
        //fish->bodiesDic = fishbodiesDic;
        this->addChild(fish, fishNum);
        fish->initFishBezier(fishNum, false);
        fish->moveWithBezier();
		fish->setTVFish(true);
    }
    timer++;
}

void FishTV::addFishSplineTV(Node* sp, PointArray* pArray)
{
	bool bLoading = Manager::getInstance()->getEnterLoading();
	if(bLoading)return;

    FishSpline* d_fish = dynamic_cast<FishSpline*>(sp);
    FishSpline* fish = FishSpline::createFishSpline(d_fish->fb_Kind);
    this->addChild(fish, d_fish->fb_Kind);
    fish->initFishSpline(d_fish->fb_Kind, false);


	for (int i = 0; i < fish->fb_splineArray->count(); i++)
	{
		fish->fb_splineArray->removeControlPointAtIndex(i);
	}

	for (int i = 0; i < d_fish->fb_splineArray->count(); i++)
	{
		Vec2 splinePos = d_fish->fb_splineArray->getControlPointAtIndex(i);
		fish->fb_splineArray->addControlPoint(splinePos);
	}

	fish->setTVFish(true);
}
void FishTV::removeSelf()
{
    this->removeFromParent();
}
FishTV::FishTV()
{}
FishTV::~FishTV()
{}