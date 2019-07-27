#include "MyActionNew.h"
#include <math.h>
#include "MyTools.h"

MyActionNew::MyActionNew():radius(0), angleBegin(0), angleEnd(0), m_bIsRotate(true)
	, m_nStandardDirection(DIR_RIGHT)
{
}

MyActionNew::~MyActionNew()
{
}

void MyActionNew::startWithTarget(Node *pTarget)
{
	ActionInterval::startWithTarget(pTarget);
 
	m_startPosition = pTarget->getPosition();//这里得到刚开始的位置
    
    m_centerPosition.x = m_startPosition.x - cos(CC_DEGREES_TO_RADIANS(angleRotationArmBegin)) * radius;
    m_centerPosition.y = m_startPosition.y - sin(CC_DEGREES_TO_RADIANS(angleRotationArmBegin)) * radius;
    
    ////CCLog("m_startPosition = (%f, %f)", m_startPosition.x, m_startPosition.y);
    ////CCLog("m_centerPosition = (%f, %f)", m_centerPosition.x, m_centerPosition.y);
}

MyActionNew* MyActionNew::create(float duration, float r, float angle_begin, float angle_delta, bool is_rotate, int standarddirection, bool radius_chage)
{
	MyActionNew * pmyAction = new MyActionNew();
	if(pmyAction && pmyAction->Init(duration, r, angle_begin, angle_delta, is_rotate, standarddirection, radius_chage))
    {
		pmyAction->autorelease();
		return pmyAction;
	}
	return NULL;
}

//这个函数得到初始化数据
bool MyActionNew::Init(float duration, float r, float angle_begin, float angle_delta, bool is_rotate, int standarddirection, bool radius_chage)
{
	if( !ActionInterval::initWithDuration(duration))
	{
        return false;
    }
    
    this->angleBegin = angle_begin;
    this->angleDelta = angle_delta;
	this->m_bIsRotate = is_rotate;
	//this->angleEnd = angle_delta;
	this->m_nStandardDirection = standarddirection;
    this->radiusISChage = radius_chage;
    
	//初始的角度
	float initAngle = 0;
	if(m_nStandardDirection == DIR_UP)
		initAngle = 270;
	else if(m_nStandardDirection == DIR_DOWN)
		initAngle = 90;
	else if(m_nStandardDirection == DIR_LEFT)
		initAngle = 180;
	else if(m_nStandardDirection == DIR_RIGHT)
		initAngle = 0;

    //计算Arm角度
    if(angle_delta > 0)
    {
        //逆时针
        angleRotationArmBegin = this->angleBegin - 90.0f - initAngle;
        //angle_end = angle_begin + angle_delta;
    }
    else
    {
        //顺时针
        angleRotationArmBegin = this->angleBegin + 90.0f - initAngle;
        //angle_end = angle_begin + angle_delta;
    }
    
	this->radius = r;

    //this->radPrevious = CC_DEGREES_TO_RADIANS(b_angle);
	return true;
}

void MyActionNew::update(float time)
{
    ////CCLog("MyAction::update(time = %f)", time);
    
	if (!getTarget())
	{
        return;
    }
    
	//根据起始角度算出顺时针还是逆时针,逆时针为-1
	//int isClockwise = (angleEnd - angleBegin > 0) ? -1 : 1;
    
	//记录未移动之前的位置，计算旋转角度
	//m_oldPosition = m_pTarget->getPosition();
    
	//得到半径
    float r;
    if (radiusISChage) {
        radius-=0.2;
        if (radius < 0) {
            radius = 0;
        }
        r = radius;
    }else
	 r = radius;
    
	
    //计算需要旋转的弧度数
    double rad_arm_begin = CC_DEGREES_TO_RADIANS(angleRotationArmBegin);
	double rad_begin = CC_DEGREES_TO_RADIANS(angleBegin);
	double rad_delta = CC_DEGREES_TO_RADIANS(angleDelta) * time;
    
	//计算更新后的具体位置
	Vec2 temp = Point(m_centerPosition.x + cos(rad_arm_begin + rad_delta) * r,
                       m_centerPosition.y + sin(rad_arm_begin + rad_delta) * r);
    
	getTarget()->setPosition(temp);
	////设置旋转角度
    //radPrevious = radPrevious + rad_delta;
	//float rotation = calcAngleFromOriginToTarget(m_oldPosition, temp);
    //float rotation = CC_RADIANS_TO_DEGREES(rad_delta) - 90;
    float rotation = CC_RADIANS_TO_DEGREES(rad_begin + rad_delta);
    
   // //CCLog("MyAction::update temp = (%f, %f) rot = %f", temp.x, temp.y, rotation);
	if(m_bIsRotate)
	{
		getTarget()->setRotation(-rotation);
	}
}

ActionInterval* MyActionNew::clone() const
{return NULL;}
ActionInterval* MyActionNew::reverse() const
{return NULL;}

