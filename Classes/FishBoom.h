//
//  FishBoom.h
//  BuyuDemo
//
//  Created by etgame on 15-5-7.
//
//

#ifndef __BuyuDemo__FishBoom__
#define __BuyuDemo__FishBoom__

#include <iostream>
#include "cocos2d.h"
#include "FishBase.h"
USING_NS_CC;
class FishBoom : public FishBase
{
public:
    FishBoom();
    virtual ~FishBoom();
    static FishBoom* createFishBoom(int _kind);
    virtual bool initFishBoom(int _kind);
    void runCircle();
    void runInBoom(int roadNum);
    void update(float dt);
    void removeBoom();
};
#endif /* defined(__BuyuDemo__FishBoom__) */
