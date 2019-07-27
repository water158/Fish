#ifndef _MYACTION_NEW_H_
#define _MYACTION_NEW_H_

#include "cocos2d.h"
USING_NS_CC;
using namespace std;

//将target的哪个方向置为正方向
enum
{
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT,
};

/***
 这个类实现了NODE的圆弧运动，使用时，直接用时间初始化，之后直接runAction
 */
class  MyActionNew : public ActionInterval
{
public:
	virtual ~MyActionNew();
	//参数：时间，半径 ，初始法线角度，偏移角度
	static MyActionNew * create(float duration, float r, float b_angle, float e_angle, bool is_rotate=true, int standarddirection=DIR_RIGHT, bool radius_chage=false);
    
protected:
	bool Init(float duration,float r, float b_angle,float e_angle, bool is_rotate, int standarddirection, bool radius_chage);
    
	virtual void startWithTarget(Node *pTarget);
	
    /**
     called once per frame. time a value between 0 and 1
     
     For example:
     - 0 means that the action just started
     - 0.5 means that the action is in the middle
     - 1 means that the action is over
     */
    virtual void update(float time);
    
    
    virtual ActionInterval* clone() const override;
	virtual ActionInterval* reverse() const override;
private:
	MyActionNew();
    
	Vec2 m_startPosition;
    Vec2 m_centerPosition;
	
    //半径
    float radius;
	
    //起始角度
    float angleBegin;
    
    //结束角度
	float angleEnd;
    
    
    float angleDelta;
    
    float angleRotationArmBegin;
    
    float angleRotationArmEnd;
    
	bool m_bIsRotate;
    //float radPrevious;
	//标准方向
	int m_nStandardDirection;

    bool radiusISChage;
};




#endif