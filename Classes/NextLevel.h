//
//  NextLevel.h
//  Qianpaobuyu
//
//  Created by etgame on 15-6-5.
//
//

#ifndef __Qianpaobuyu__NextLevel__
#define __Qianpaobuyu__NextLevel__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;
class NextLevel : public LayerColor
{
public:
    NextLevel();
    virtual ~NextLevel();
    static NextLevel* createNextLevel();
    virtual bool initNextLevel();
    void inNext(Ref* Obj);
    //CREATE_FUNC(CountdownLayer);
private:
    Size size;
   
    
};
#endif /* defined(__Qianpaobuyu__NextLevel__) */
