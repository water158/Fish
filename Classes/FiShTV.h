//
//  FiShTV.h
//  Qianpaobuyu
//
//  Created by etgame on 15-5-14.
//
//

#ifndef __Qianpaobuyu__FiShTV__
#define __Qianpaobuyu__FiShTV__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
class FishTV : public Layer
{
public:
    FishTV();
    virtual ~FishTV();
    static FishTV* createFishTV();
    virtual bool initFishTV();
    void addFishTV(float dt);
    void addFishSplineTV(Node* sp, PointArray* pArray);
    void removeSelf();
private:
    
};
#endif /* defined(__Qianpaobuyu__FiShTV__) */
