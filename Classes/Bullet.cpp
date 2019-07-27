//
//  Bullet.cpp
//  Helloworld
//
//  Created by etgame on 15-1-31.
//
//

#include "Bullet.h"

#include "Data.h"
#include "Manager.h"

#define MOVEUNIT 4

Bullet::Bullet():isCollision(false)
{
	log("Bullet::Bullet()");
	bulletbody = NULL;
}
Bullet::~Bullet()
{
    //CC_SAFE_DELETE(bulletbody);
	//CC_SAFE_RELEASE(sp1);
	//CC_SAFE_RELEASE(sp2);
	//CC_SAFE_RELEASE(sp3);
	log("Bullet::~Bullet()");
	//CC_SAFE_RELEASE(this->getPhysicsBody());
	if(bulletbody)
	{
		CC_SAFE_RELEASE_NULL(bulletbody);
	}
}
Bullet* Bullet::createBullet()
{
    Bullet* pRet = new Bullet();
    if (pRet && pRet->init()) 
	{
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

void Bullet::initBullet( Point _point, Point clickPoint,  int _bPower, int type, int _num)
{
    
    b_isCatch = false;
    atNum = 0;
    bulletType = type;
    num = _num;
    originPoint = _point ;
    bulletPower = _bPower;
	m_nHolderID = -1;
    //bulletbody->setEnable(false);
    this->setPosition(originPoint);
    //this->setPosition(Vec2(427.000000, 142.000000));
    //CCLOG("_rotation = %f", _rotation);
    this->setTag(BULLET_TAG);
    //this->setScale(SCALE);
    switch (num) 
	{
        case 1:
        {
           // bulletbody = PhysicsBodyNew::createPolygon(bullet_one, 4);
            auto sp1 = Sprite::createWithSpriteFrameName(bullet_image[bulletType]);
            sp1->setPosition(Point::ZERO);
            this->addChild(sp1);
        }
            break;
        case 2:
        {
            //bulletbody = PhysicsBodyNew::createPolygon(bullet_two, 4);
            auto sp1 = Sprite::createWithSpriteFrameName(bullet_image[bulletType]);
            sp1->setPosition(Point(-13, 40));
            this->addChild(sp1);
            
            auto sp2 = Sprite::createWithSpriteFrameName(bullet_image[bulletType]);
            sp2->setPosition(Point(12, 40));
            this->addChild(sp2);
        }
            break;
        case 3:
        {
            //bulletbody = PhysicsBodyNew::createPolygon(bullet_three, 4);
            auto sp1 = Sprite::createWithSpriteFrameName(bullet_image[bulletType]);
            sp1->setPosition(Point(-24, 0));
            this->addChild(sp1);
            
            auto sp2 = Sprite::createWithSpriteFrameName(bullet_image[bulletType]);
            sp2->setPosition(Point::ZERO);
            this->addChild(sp2);
            
            auto sp3 = Sprite::createWithSpriteFrameName(bullet_image[bulletType]);
            sp3->setPosition(Point(24, 0));
            this->addChild(sp3);
        }
            break;
        default:
            break;
    }
    Manager* instance = Manager::getInstance();
    bulletbody = instance->loadBulletBodies(num);
    this->setPhysicsBody(bulletbody);
	//instance->setBulletBodies(NULL);
    
    this->scheduleUpdate();
    
    recordfire = 0;

    size = CCDirector::getInstance()->getWinSize();
   
    shootDis = clickPoint - originPoint;
    float rotateRadians = shootDis.getAngle();
    bullet_rotation = CC_RADIANS_TO_DEGREES(-1*rotateRadians) + 90;
    //normalizedShootDis = shootDis.normalize()*MOVEUNIT;
    normalizedShootDis = shootDis.getNormalized()*MOVEUNIT;
   
    this->setRotation(bullet_rotation);
}

void Bullet::update(float delta)
{
    
    Point bulletPoint = this->getPosition();
    this->setRotation(bullet_rotation);
    //CCLog("x = %f", this->getPosition().x);
    this->setPosition(this->getPosition() + normalizedShootDis*MOVEUNIT);
    
    recordfire++;
    if (recordfire > 5) {
        if (bulletPoint.x > size.width- MOVEUNIT) {
            bullet_rotation = -bullet_rotation;
            shootDis = Point(-shootDis.x, shootDis.y);
            this->setPosition(Vec2(size.width - MOVEUNIT, this->getPositionY()));
        }else if (bulletPoint.y > size.height - MOVEUNIT){
            bullet_rotation = -180.0f-bullet_rotation;
            shootDis = Point(shootDis.x, -shootDis.y);
            this->setPosition(Vec2(this->getPositionX(), size.height - MOVEUNIT));
        }else if (bulletPoint.x < MOVEUNIT){
            bullet_rotation = -bullet_rotation;
            shootDis = Point(-shootDis.x, shootDis.y);
            this->setPosition(Vec2(MOVEUNIT, this->getPositionY()));
        }else if (bulletPoint.y < MOVEUNIT){
            bullet_rotation = 180.0f - bullet_rotation;
            shootDis = Point(shootDis.x, -shootDis.y);
            this->setPosition(Vec2(this->getPositionX(), MOVEUNIT));
        }
        //    else if (bulletPoint.x < MOVEUNIT&&bulletPoint.y < MOVEUNIT) {
        //        bullet_rotation += 180;
        //        shootDis = Point(-shootDis.x, -shootDis.y);
        //        this->setPosition(Vec2(MOVEUNIT, MOVEUNIT));
        //    }
        normalizedShootDis = shootDis.getNormalized()*MOVEUNIT;
    }
     float dis = bulletPoint.distance(originPoint);
    if (dis > 25) {
        isCollision = true;
		//bulletbody->setEnable(true);
    }
    
    if (b_isCatch) {
        
        if (this->getPhysicsBody()) {
            //bulletbody->removeFromWorld();
            this->removeFromParent();
            //bulletbody = nullptr;
        }
        
    }
}

void Bullet::removeBullet()
{
    if (this) 
	{
		this->removeAllChildren();
        this->removeFromParent();
    }
}