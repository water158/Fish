//
//  SanYuanSiXi.h
//  Qianpaobuyu
//
//  Created by etgame on 15-5-20.
//
//

#ifndef __Qianpaobuyu__SanYuanSiXi__
#define __Qianpaobuyu__SanYuanSiXi__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
#include "FishBase.h"
//class PhysicsBodyNew;
class SanYuanSiXi : public FishBase
{
public:
    SanYuanSiXi();
    virtual ~SanYuanSiXi();
    static SanYuanSiXi* createSanYuanSiXi();
    virtual bool initSanYuanSiXi(int _num, int _fishNum);
    CREATE_FUNC(SanYuanSiXi);
    void update(float dt);
   
	virtual void catchAnimation(int fishType);

private:
    
    
    
    Sprite* bottom_sp;
};
#endif /* defined(__Qianpaobuyu__SanYuanSiXi__) */
