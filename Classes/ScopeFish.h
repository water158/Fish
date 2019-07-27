//
//  ScopeFish.h
//  Qianpaobuyu
//
//  Created by etgame on 15-5-15.
//
//

#ifndef __Qianpaobuyu__ScopeFish__
#define __Qianpaobuyu__ScopeFish__

#include <iostream>
#include "cocos2d.h"
#include "FishBase.h"
USING_NS_CC;
class ScopeFish : public FishBase
{
public:
    ScopeFish();
    virtual ~ScopeFish();
    static ScopeFish* createScopeFish();
    virtual bool initScopeFish(int _kind);
    CREATE_FUNC(ScopeFish);
    void update(float dt);
    //void removeScopeFish();
    void checkNearestFish();
    void moveWithBezier();
private:
    Point fb_startPoint, fb_endPoint, fb_controlPoint1, fb_controlPoint2;
    Point sub;
    Point oPoint;
    //Speed* speed;
    Size size;
};
#endif /* defined(__Qianpaobuyu__ScopeFish__) */
