//
//  EnergyCannonCard.h
//  Qianpaobuyu
//
//  Created by etgame on 15-5-15.
//
//

#ifndef __Qianpaobuyu__EnergyCannonCard__
#define __Qianpaobuyu__EnergyCannonCard__

#include <iostream>
#include "cocos2d.h"
#include "FishBase.h"
USING_NS_CC;
class EnergyCannonCard : public FishBase
{
public:
    EnergyCannonCard();
    virtual ~EnergyCannonCard();
    static EnergyCannonCard* createEnergyCannonCard();
    virtual bool initEnergyCannonCard(int _kind);
    CREATE_FUNC(EnergyCannonCard);
    void update(float dt);
    //void removeEnergyCannonCard();
     void moveWithBezier();
private:
    Point fb_startPoint, fb_endPoint, fb_controlPoint1, fb_controlPoint2;
    Point sub;
    Point oPoint;
    //Speed* speed;
};
#endif /* defined(__Qianpaobuyu__EnergyCannonCard__) */
