//
//  NetSprite.h
//  BuyuDemo
//
//  Created by etgame on 15-3-28.
//
//

#ifndef __BuyuDemo__NetSprite__
#define __BuyuDemo__NetSprite__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;
class NetSprite : public Sprite
{
public:
    NetSprite();
    virtual ~NetSprite();
    static NetSprite* createNet();
    virtual bool initNet(int type, int num);
    //void initNetImage(string name);
    Sequence* netAnimation();
    void removeNet(Node* node);
    CREATE_FUNC(NetSprite);
private:
    int net_type;
    int net_num;
    Sprite* net_sp1; //渔网
    Sprite* net_sp2; //渔网
    Sprite* net_sp3; //渔网
};
#endif /* defined(__BuyuDemo__NetSprite__) */
