//
//  PixCell.cpp
//  BuyuDemo
//
//  Created by etgame on 15-4-28.
//
//

#include "PixCell.h"
#include "Data.h"

PixCell::PixCell()
{
	log("PixCell::PixCell()");
}
PixCell::~PixCell()
{
	log("PixCell::~PixCell()");
}

PixCell* PixCell::createPixCell(int index, Point _point)
{
    PixCell* pRet = new PixCell();
    if (pRet&&pRet->initPixCell(index, _point)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool PixCell::initPixCell(int index, Point _point)
{
    
    point = _point;
    this->setAnchorPoint(Point(0.5f, 1.0f));
    this->setScale((float)(rand()%4+8)/10);
    //CCLOG("rand = %f", (float)(rand()%6+6)/10);
	__String* pStr = __String::createWithFormat("bomb%d_bg.png", index);
    pixSprite = Sprite::createWithSpriteFrameName(pStr->getCString());
    pixSprite->setScale(SCALE);
    pixSprite->setAnchorPoint(Point(0.5f, 0));
    this->addChild(pixSprite);
    this->setPosition(point);
    this->setMove();
    return true;
}

void PixCell::setMove()
{
    FadeTo* fadeto = FadeTo::create(0.5, 0);
    ScaleTo* scaleto = ScaleTo::create(1, 1.0);
    MoveTo* to = MoveTo::create(1, Point(0, 200));
    Spawn* spawn = Spawn::create(to, scaleto, nullptr);
    Sequence* seq = Sequence::create(spawn, CallFunc::create(CC_CALLBACK_0(PixCell::removeSelf, this)), nullptr);
    Sequence* seq1 = Sequence::create(DelayTime::create(0.5), fadeto, nullptr);
    
    pixSprite->runAction(seq1);
    
    pixSprite->runAction(seq);
}
void PixCell::removeSelf()
{
    this->removeFromParentAndCleanup(true);
}