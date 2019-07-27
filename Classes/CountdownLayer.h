//
//  CountdownLayer.h
//  Qianpaobuyu
//
//  Created by etgame on 15-6-5.
//
//

#ifndef __Qianpaobuyu__CountdownLayer__
#define __Qianpaobuyu__CountdownLayer__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;
class CountdownLayer : public LayerColor
{
public:
    CountdownLayer();
    virtual ~CountdownLayer();
    static CountdownLayer* createCountdownLayer();
    virtual bool initCountdown();
    void countDown(float dt);
    CREATE_FUNC(CountdownLayer);
    
//    virtual void onEnter();
//    virtual void onExit();
//    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
//    virtual void onTouchMoved(Touch *touch, Event *unused_event);
//    virtual void onTouchEnded(Touch *touch, Event *unused_event);
//    virtual void onTouchCancelled(Touch *touch, Event *unused_event);
private:
    Size size;
    int recordCountdown;
    
   // bool isWake; //
    
};
#endif /* defined(__Qianpaobuyu__CountdownLayer__) */
