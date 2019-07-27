#ifndef _TelephoneFare_Fish_
#define _TelephoneFare_Fish_

#include <iostream>
#include "cocos2d.h"
#include "FishBase.h"
USING_NS_CC;

class TelephoneFareFish : public FishBase
{
public:
    TelephoneFareFish();
    virtual ~TelephoneFareFish();
    static TelephoneFareFish* createTelephoneFareFish();
    virtual bool initTelephoneFareFish(int _kind);
    CREATE_FUNC(TelephoneFareFish);
    void update(float dt);
   
    void moveWithBezier();
private:
    Point fb_startPoint, fb_endPoint, fb_controlPoint1, fb_controlPoint2;
    Point sub;
    Point oPoint;
    Size size;
};
#endif
