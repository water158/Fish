//
//  FishBezier.h
//  BuyuDemo
//
//  Created by etgame on 15-5-7.
//
//

#ifndef __BuyuDemo__FishBezier__
#define __BuyuDemo__FishBezier__

#include <iostream>
#include "cocos2d.h"
#include "FishBase.h"
USING_NS_CC;

class FishBezier : public FishBase
{
public:
    FishBezier();
    virtual ~FishBezier();
    static FishBezier* createFishBezier(int _kind);
    virtual bool initFishBezier(int _kind, bool isBodies = true);
    
    void moveWithBezier();
    
    virtual void update(float dt);
    //void removeSelf();
    CREATE_FUNC(FishBezier);
    Point fb_startPoint, fb_endPoint, fb_controlPoint1, fb_controlPoint2;
protected:
    
    Point sub;
    Point oPoint;
    //Speed* bezierSpeed;
};
#endif /* defined(__BuyuDemo__FishBezier__) */
