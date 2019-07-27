//
//  SpellNumber.h
//  Qianpaobuyu
//
//  Created by etgame on 15-6-9.
//
//

#ifndef __Qianpaobuyu__SpellNumber__
#define __Qianpaobuyu__SpellNumber__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;
class SpellNumber : public Node
{
public:
    SpellNumber();
    virtual ~SpellNumber();
    static SpellNumber* createSpellNumber(int num, int nType = 0);
    virtual bool initSpellNumber(int num, int nType);
    
    //CREATE_FUNC(CountdownLayer);
    void addAnimation();
    void removeNum();
    
    int m_nType;
};
#endif /* defined(__Qianpaobuyu__SpellNumber__) */
