//
//  FishGroup.h
//  Qianpaobuyu
//
//  Created by etgame on 15-5-15.
//
//

#ifndef __Qianpaobuyu__FishGroup__
#define __Qianpaobuyu__FishGroup__

#include <iostream>
#include "cocos2d.h"
#include "FishBase.h"
USING_NS_CC;
class FishGroup : public FishBase
{
public:
    FishGroup();
    virtual ~FishGroup();
    static FishGroup* createFishGroup();
    virtual bool initFishGroup(int _num, int _fishNum);
    CREATE_FUNC(FishGroup);
    void update(float dt);
    //void removeFishGroup();
    void moveWithBezier();
    virtual void catchAnimation(int fishType);
    Point fb_startPoint, fb_endPoint, fb_controlPoint1, fb_controlPoint2;
private:
    Point sub;
    Point oPoint;
    //Speed* speed;
    Size size;
    Sprite* bottom_sp;
    int groupNum;
    int fishNum;
};
#endif /* defined(__Qianpaobuyu__FishGroup__) */
