//
//  PixCell.h
//  BuyuDemo
//
//  Created by etgame on 15-4-28.
//
//

#ifndef __BuyuDemo__PixCell__
#define __BuyuDemo__PixCell__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
class PixCell : public Node
{
public:
    PixCell();
    virtual ~PixCell();
    static PixCell* createPixCell(int index, Point _point);
    virtual bool initPixCell(int index, Point _Point);
    
    void setMove();
    void removeSelf();
    
    
private:
    Sprite* pixSprite;
    Point point;
    
};
#endif /* defined(__BuyuDemo__PixCell__) */
