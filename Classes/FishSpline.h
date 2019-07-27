//
//  FishSpline.h
//  BuyuDemo
//
//  Created by etgame on 15-5-7.
//
//

#ifndef __BuyuDemo__FishSpline__
#define __BuyuDemo__FishSpline__

#include <iostream>
#include "cocos2d.h"
#include "FishBase.h"
USING_NS_CC;
class FishSpline : public FishBase
{
public:
    FishSpline();
    virtual ~FishSpline();
    static FishSpline* createFishSpline(int _kind);
    virtual bool initFishSpline(int _kind, bool isBodies = true);
    
    void moveSpline();
    virtual void update(float dt);
    void callReverse();
    //void removeSpline();
    CREATE_FUNC(FishSpline);
    //PointArray* fb_splineArray;
private:
    Point sub;
    Point oPoint;
    Size size;
    CardinalSplineTo* cardinalSplineto;
};
#endif /* defined(__BuyuDemo__FishSpline__) */
