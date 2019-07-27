//
//  ProductType.h
//  Qianpaobuyu
//
//  Created by etgame on 15-5-8.
//
//

#ifndef __Qianpaobuyu__ProductType__
#define __Qianpaobuyu__ProductType__

#include <iostream>
#include "cocos2d.h"
#include "FishBase.h"
USING_NS_CC;
class ProductType : public FishBase
{
public:
    ProductType();
    virtual ~ProductType();
    static ProductType* createProductType();
    virtual bool initProductType(int _kind, bool isBodies = true);
    CREATE_FUNC(ProductType);
    void update(float dt);
    void runMove(Point pos, Vec2 disPos);
    //void removeProductType();
};
#endif
