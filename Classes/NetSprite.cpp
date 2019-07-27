//
//  NetSprite.cpp
//  BuyuDemo
//
//  Created by etgame on 15-3-28.
//
//

#include "NetSprite.h"
#include "Data.h"
#define NET_INDEX 49
NetSprite::NetSprite():net_sp1(nullptr), net_sp2(nullptr), net_sp3(nullptr)
{
	log("NetSprite::NetSprite()");
}
NetSprite::~NetSprite()
{
	log("NetSprite::~NetSprite()");
}
NetSprite* NetSprite::createNet()
{
    NetSprite* pRet = new NetSprite();
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}


bool NetSprite::initNet(int type, int num)
{
   
    net_type = type;
    net_num = num;
    
    this->setScale(1.0);
    
    switch (net_num) {
        case 1:
        {
            net_sp1 = Sprite::createWithSpriteFrameName(net_image[net_type].c_str());
            net_sp1->setPosition( Point(-(20+rand()%11 - 5), -(rand()%11 - 5)));
            net_sp1->setScale(rand()%5/10+0.8);
            this->addChild(net_sp1, NET_INDEX);
            net_sp1->runAction(this->netAnimation());
        }
            break;
        case 2:
        {
            
            
            net_sp1 = Sprite::createWithSpriteFrameName(net_image[net_type].c_str());
            net_sp2 = Sprite::createWithSpriteFrameName(net_image[net_type].c_str());
            net_sp1->setPosition( - Point(20+rand()%11 - 5, rand()%11 - 5));
            net_sp2->setPosition(  Point(20+rand()%11 - 5, rand()%11 - 5));
            
     
            net_sp1->setScale(rand()%5/10+0.8);
            net_sp2->setScale(rand()%5/10+0.8);
            this->addChild(net_sp1, NET_INDEX);
            this->addChild(net_sp2, NET_INDEX);
            net_sp1->setTag(100);
            net_sp2->setTag(101);
			auto pAction = this->netAnimation();
            net_sp1->runAction(pAction);
            net_sp2->runAction(pAction->clone());
        }
            break;
        case 3:
        {
            net_sp1 = Sprite::createWithSpriteFrameName(net_image[net_type].c_str());
            net_sp2 = Sprite::createWithSpriteFrameName(net_image[net_type].c_str());
            net_sp3 = Sprite::createWithSpriteFrameName(net_image[net_type].c_str());
            net_sp1->setPosition( - Point(20+rand()%11 - 5, rand()%11 - 5));
            net_sp2->setPosition(  Point(0, rand()%11 - 5));
            net_sp3->setPosition(  Point(20+rand()%11 - 5, rand()%11 - 5));
            net_sp1->setScale(rand()%5/10+0.8);
            net_sp2->setScale(rand()%5/10+0.8);
            net_sp3->setScale(rand()%5/10+0.8);

            this->addChild(net_sp1, NET_INDEX);
            this->addChild(net_sp2, NET_INDEX);
            this->addChild(net_sp3, NET_INDEX);
			auto pAction = this->netAnimation();
            net_sp1->runAction(pAction);
            net_sp2->runAction(pAction->clone());
            net_sp3->runAction(pAction->clone());
            
        }
            break;
            
        default:
            break;
    }
    
    Sequence* seq = Sequence::create(DelayTime::create(0.21),
                                     CallFuncN::create(CC_CALLBACK_1(NetSprite::removeNet, this)),
                                     NULL);
    this->runAction(seq);
    return true;
}
Sequence* NetSprite::netAnimation()
{
    
    
    //网的缩放效果
    ScaleTo* to1 = ScaleTo::create(0.1, 0.6);
    ScaleTo* to2 = ScaleTo::create(0.1, 1.2);
//    net_sp2->setColor(Color3B(100, 100, 100));
//    net_sp3->setColor(Color3B(100, 100, 100));
    //CallFunc* func = CallFuncN::create( CC_CALLBACK_1(NetSprite::removeNet, this));
    Sequence* seq = Sequence::create(to1, to2, nullptr);
    return seq;
}
void NetSprite::removeNet(Node* node)
{
    
    node->removeFromParent();
    
    
}