//
//  Fire.h
//  Helloworld
//
//  Created by etgame on 15-1-28.
//
//

#ifndef __Helloworld__Fire__
#define __Helloworld__Fire__

#include <iostream>
#include "cocos2d.h"
#include "CannonBase.h"
#include "NoseeFish.h"
USING_NS_CC;
class Manager;
class FishBase;
class Fire : public CannonBase
{
public:
    Fire();
    virtual ~Fire();
    CREATE_FUNC(Fire);
    
    static Fire* createFire();
    void initFire( int powerOfBullet, int _type, int _num);
    void checkNearestFish();
   // void rotateAndShoot(float dt);
    void update(float dt);
    void shoot(Node* ref);
    void setFlameBlink(Node* ref);
	//模拟玩家换炮
    void simulationPlyaer(float dt);
	//随机威力
	void randCannonPower();
    //随机位置
	void randPosition(Node* pNode);
	//瞄准BOSS
	bool aimAtBoss();
    void addEnergyCannonLv();
	//设置能量炮
    void setEnergy();
	//回复锁定boss
	void callBackCouldLockFish(Node* pNode);
	//随机任务完成的数量
	void randTaskNum();
	//获取此ID的鱼是否已经捕捉够
	bool getIsFinishTaskFish(int nID);
	//改变金币框的位置
    virtual void changeCannonPos();   
	//更新用户信息
	virtual void refreshUserdata();
    int fireNum; //炮台编号
    float origRotation; //大炮的原始角度

	//加速射击
	void speedUp();
	void callBackResumeSpeed(Node* pNode);
	bool m_bSpeedUp;
private:
    FishBase* neanestFish;//大炮附近的小鱼
    
    Manager* instance;
    Size widSize;// 屏幕大小
    Point nearPoint;
	//记录能量炮之前的ID
	CC_SYNTHESIZE(int, f_nOldCannonID, OldCannonID);
	//炮台选择的位置标记， 自由玩家选择的确定位置Menu
    CC_SYNTHESIZE(int, f_chosePositionNum, f_chosePositionNum);
	//金币变化标记
    CC_SYNTHESIZE(bool, f_coinIsChange, F_coinIsChange); 
	//记录存在时间
    CC_SYNTHESIZE(int, recordAppearTime, RecordAppearTime); 
	//是否可以发射
	CC_SYNTHESIZE(bool, m_bCouldShoot, CouldShoot);
    //控制发射速度
	CC_SYNTHESIZE(int, f_nFrameNum, F_nFrameNum);
	//是否射击BOSS
	CC_SYNTHESIZE(bool, m_bShootBoss, ShootBoss);
	//是否可以锁定目标
	CC_SYNTHESIZE(bool, m_bCouldLockFish, CouldLockFish);

    int ori_rate;
    //记录时间
    int f_recordTime; 
	//控制射击模式
    int f_recordshootKind; 
    int energyTimer;
    Size size;
};
#endif
