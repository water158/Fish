//
//  ProductType.cpp
//  Qianpaobuyu
//
//  Created by etgame on 15-5-8.
//
//

#include "ProductType.h"
#include "Manager.h"
ProductType::ProductType()
{
	//log("ProductType::ProductType()");
}
ProductType::~ProductType()
{
	//log("ProductType::~ProductType()");
}

ProductType* ProductType::createProductType()
{
    ProductType* pRet = new ProductType();
    if (pRet&&pRet->init()) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}
bool ProductType::initProductType(int _kind,  bool isBodies)
{
    isSetbody = isBodies;
    fb_Kind = _kind;
    this->initFishBase(fb_Kind);
    this->scheduleUpdate();
    
    return true;
}
void ProductType::runMove(Point pos, Vec2 disPos)
{
    this->setPosition(pos);
    MoveBy* by = MoveBy::create(10, disPos);
    CallFunc* func = CallFunc::create([&]{
        this->removeFromParent();
        Manager::getInstance()->m_fishVect.eraseObject(this);
    });
    Sequence* seq = Sequence::create(by, func, nullptr);
    this->runAction(seq);
    
}
void ProductType::update(float dt)
{
    graySprite->setRotation(this->getRotation());
    //log("%f, %f", this->convertToWorldSpace(this->getPosition()).x, this->convertToWorldSpace(this->getPosition()).y);
    graySprite->setPosition(Vec2(this->getPosition().x - 20 , this->getPosition().y - 40));
    if (isCatch) {
        setIsCatch(false);
        this->theSpecialEffects();
        //this->removeProductType();
        this->catchAnimation(fb_Kind);
//        if (bindToCannonNum == 1) {
//            instance->setM_totalCoin(instance->getM_totalCoin() + value*(instance->getM_bulletPower()));
//        }
    }
    
}

