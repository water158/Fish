//
//  FishPath.h
//  Helloworld
//
//  Created by etgame on 15-1-30.
//
//

#ifndef __Helloworld__FishPath__
#define __Helloworld__FishPath__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

class FishPath
{
public:
    FishPath();
    virtual ~FishPath();
    void addPath();
    void boomPath();
    void fishPah();
    void splinePath();
    void choseGroupPath();
public:
    Point startPoint, endPoint, controlPoint1, controlPoint2;
    float  time;
    PointArray* splineArray;
    int splineNum;
    PointArray* groupArr; //三元四喜样条点
};
#endif /* defined(__Helloworld__FishPath__) */
