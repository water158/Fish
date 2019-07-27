//
//  Coin.h
//  BuyuDemo
//
//  Created by etgame on 15-4-20.
//
//

#ifndef __BuyuDemo__Coin__
#define __BuyuDemo__Coin__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

class Coin : public Node
{
public:
    Coin();
    virtual ~Coin();
    static Coin* createCoin( int _fKind, Point _oriPoint, Point _dispoint);
    virtual bool initCoin( int _fKind, Point _oriPoint, Point _dispoint);
    void setAnimation();
    void setMove();
    void removeCoin();
    void addSound();
private:
    
    int c_num;
    Point oriPoint, disPoint;
    int num;
    Animate* coinAnimate;
};
#endif /* defined(__BuyuDemo__Coin__) */
