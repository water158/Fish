//
//  NoseeFish.h
//  Qianpaobuyu
//
//  Created by etgame on 15-6-26.
//
//

#ifndef __Qianpaobuyu__NoseeFish__
#define __Qianpaobuyu__NoseeFish__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
#include "FishBase.h"
class NoseeFish : public FishBase
{
public:
    NoseeFish();
    ~NoseeFish();
    static NoseeFish*  createNoseeFish();
    virtual bool initNoseeFish(int kindNum);
    //CREATE_FUNC(NoseeFish);
};
#endif /* defined(__Qianpaobuyu__NoseeFish__) */
