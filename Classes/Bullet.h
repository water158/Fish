//
//  Bullet.h
//  Helloworld
//
//  Created by etgame on 15-1-31.
//
//

#ifndef __Helloworld__Bullet__
#define __Helloworld__Bullet__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
#include "CannonBase.h"
using namespace std;
class Bullet : public Sprite
{
public:
    Bullet();
    virtual ~Bullet();
    //CREATE_FUNC(Bullet);
    static Bullet*  createBullet();
    void initBullet(Point _point, Point clickPoint, int _bPower, int type, int _num);
    //void  initBulletImage(Texture2D* texture);
    void removeBullet();
    void update(float delta);
    
    int bulletPower;
   
    int atNum; //射出子弹的炮台编号
    Point originPoint; //子弹的原始位置
    int bulletType;
    int num; //子弹编号(一次发射的子弹个数)
    Point normalizedShootDis; //标准向量
    
    PhysicsBody *bulletbody;
private:
    
    //bool isRotation;
    float bullet_rotation; //轨道夹角
    Size size; // 屏幕尺寸
    Point shootDis;//射击方向
    int recordfire; //记录子弹发射时间
    CC_SYNTHESIZE(bool, b_isCatch, B_isCatch);
    CC_SYNTHESIZE(bool, isCollision, IsCollision);//碰撞标记
    CC_SYNTHESIZE(int , m_nHolderID, HolderID);
    
};
#endif /* defined(__Helloworld__Bullet__) */
