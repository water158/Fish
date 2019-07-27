//
//  FishMedol.h
//  Qianpaobuyu
//
//  Created by etgame on 15-5-16.
//
//

#ifndef __Qianpaobuyu__FishMedol__
#define __Qianpaobuyu__FishMedol__

#include <iostream>
#include "cocos2d.h"
#include "GlobalDelegate.h"

USING_NS_CC;
class Manager;
class FishMedol : public Layer
{
public:
    FishMedol();
    virtual ~FishMedol();
    static FishMedol* createFishMedol();
    virtual bool initFishMedol();
    void runFishMedol(int roadNum);
    void removeFishMedol();
    void addFish(float dt);
    
    void removeType(Node* ref);
private:
    Size size;
    int typeNum;
    Manager* instance;
    int fm_recordtimer;
	//代理
	CC_SYNTHESIZE(GameDelegate*, m_pDelegateGame, DelegateGame);
    CC_SYNTHESIZE(int, totalTime, TotalTime); //鱼走完路程需要的时间
};
#endif /* defined(__Qianpaobuyu__FishMedol__) */
