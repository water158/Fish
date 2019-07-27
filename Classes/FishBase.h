//
//  FishBase.h
//  Helloworld
//
//  Created by etgame on 15-1-27.
//
//

#ifndef __Helloworld__FishBase__
#define __Helloworld__FishBase__

#include <iostream>
#include "cocos2d.h"
#include "GlobalDelegate.h"

USING_NS_CC;

#define BASICSP 11

class PhysicsBodyNew;
class Manager;
class FishBase : public Sprite
{
public:
    FishBase();
    virtual ~FishBase();
    CREATE_FUNC(FishBase);
	//鱼的类型
    void kindOfFish(int number);
    //鱼的血量
    void hpOfFish(int number);
	//得分
    void numOfScore(int number);
    
    void initFishBase(int fKind);
    void setBody();
    //鱼 游动 动画
    void fishAnimation(int fishType); 
    void setShader();
    void removeShader();
	//删除刚体
    void removeFishBody();
	//删除精灵
    void removeFish();
	//删除所有
    void removeAllThings();
    bool randomCatch(int uprate, int nID);
    virtual void catchAnimation(int fishType);
	//鱼死亡后的特效
    void theSpecialEffects(); 
	//加速进场
	void enterSpeedUp();
	void callBackResumeSpeed();
	//设置锁定BOSS
	void setAimedFish(int num);
	//获取是否被机器人锁定
	bool getAimedByRobot(int num);
	Sprite* getBasicSprite1();

    Point sub;
    int hp; //血量
	//控制血量参数
    int fish_basisHp, typeHp, arcHp; 
    int value; //价值
    int time;  //游动时间
    int fb_Kind;
    PhysicsBody* fishbody;
    PointArray* fb_splineArray;
   
protected:
    Point oPoint;
    Manager* instance;
    Size size;
    Point fb_startPoint, fb_endPoint, fb_controlPoint1, fb_controlPoint2;
    
	//代理
	CC_SYNTHESIZE(GameDelegate*, m_pDelegateGame, DelegateGame);
    
    Sprite* graySprite;
    CC_SYNTHESIZE(bool, m_bLocked, Locked);
	//鱼的唯一ID（相当于人的身份证）
	CC_SYNTHESIZE(unsigned int, m_nUID, UID); 
    bool isSetbody; //是否设置刚体
    CC_SYNTHESIZE(bool, isYiwangdajin, IsYiwangdajin);
    CC_SYNTHESIZE(bool, isDasanyuan, IsDasanyuan);
    CC_SYNTHESIZE(bool, isCatch, IsCatch);  //鱼被捕获标记
    CC_SYNTHESIZE(int, bindToCannonNum, BindToCannonNum);
    CC_SYNTHESIZE(int, arc_die, Arc_die); //鱼的死亡概率
    
    CC_SYNTHESIZE(int, choseOne, ChoseOne);//
    CC_SYNTHESIZE(bool, isGray, IsGray); //是否需要阴影
    CC_SYNTHESIZE(Speed*, moveSpeed, MoveSpeed); //设置鱼游动的speed
    CC_SYNTHESIZE(Speed*, animSpeed, animSpeed); //设置鱼动画的speed
	CC_SYNTHESIZE(Speed*, greySpriteSpeed, GreySpriteSpeed); //设置鱼yinying动画的speed
    int fishInRoom;//不同房间的标记
    CC_SYNTHESIZE(bool, isSpecial, IsSpecial); //特殊鱼的标记
	//随机话费
	CC_SYNTHESIZE(int, m_nFishTeleploneFare, FishTeleploneFare);
	//加速之后恢复到的速度
	CC_SYNTHESIZE(float, m_nResumeSpeed, ResumeSpeed);
	//是否是小电视的鱼
	CC_SYNTHESIZE(bool, m_bTVFish, TVFish); 
	//倍率
	CC_SYNTHESIZE(int , m_nBulletPower, BulletPower); 
	//被机器人定为目标
	CC_SYNTHESIZE(bool, m_bAimedByRobot1, AimedByRobot1);
	CC_SYNTHESIZE(bool, m_bAimedByRobot2, AimedByRobot2);
	CC_SYNTHESIZE(bool, m_bAimedByRobot3, AimedByRobot3);
	CC_SYNTHESIZE(bool, m_bAimedByRobot4, AimedByRobot4);
};
#endif
