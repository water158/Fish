//
//  CoinPrompt.h
//  Buyudaren
//
//  Created by etgame on 15-4-28.
//
//

#ifndef __Buyudaren__CoinPrompt__
#define __Buyudaren__CoinPrompt__

#include <iostream>
#include "cocos2d.h"
#include "CoinPrompt.h"
USING_NS_CC;
class CoinPrompt : public Node
{
public:
    static CoinPrompt* createCoinPrompt(int index, Point _point, int _coinNum);
    virtual bool initCoinPrompt(int index, Point _point, int _coinNum);
    void removeCoinPrompt();
    CoinPrompt();
    virtual ~CoinPrompt();
private:
    int fishKind;
    Point point;
    Sprite* bg_sp;
    Sprite* fish_name;
    int coinNum;
};
#endif /* defined(__Buyudaren__CoinPrompt__) */
