//
//  ManualCannon.h
//  Helloworld
//
//  Created by etgame on 15-1-29.
//
//

#ifndef __Helloworld__ManualCannon__
#define __Helloworld__ManualCannon__

#include <iostream>
#include "cocos2d.h"
#include "CannonBase.h"
#include "MyTools.h"
#include "GlobalDelegate.h"
#include "MyXmlParser.h"
USING_NS_CC;

class Manager;
class MusicData;
class ManualCannon : public CannonBase
{
public:
    
    static ManualCannon* createCannon();
    void initConnon(int powerOfBullet, int _type, int _num, bool& _cdtimer);
    ManualCannon();
    virtual ~ManualCannon();
    void update(float dt);

    void setFlameBlink(Node* ref);
    void setEnergyCannon();
	//能量炮时，显示为下一等级炮
    void addEnergyCannonLv();
	//更新用户信息
	virtual void refreshUserdata();
	//加速
	virtual void speedUp();
	//恢复速度
	virtual void resumeSpeed();
	//狂暴
	virtual void rage();
	//消除狂暴
	virtual void cancelRage();

	//更新补贴时间
	void updateSubsidies(float dt);
	//通过下标设置显示
	void setWordsByIndex(int nIndex);
    
    CC_SYNTHESIZE(Point, clickpoint, ClickPoint);
    bool isShoot;  //是否达到射击条件
    int manualNum;
    int rcordTime;
private:
    float rotateDegress;
    Manager* instance;
    MusicData* mc_music;
	//记录能量炮之前的ID
	CC_SYNTHESIZE(int, m_nOldCannonID, OldCannonID);
    //控制发射速度
	CC_SYNTHESIZE(int, m_nFrameNum, FrameNum);
	//是否狂暴
	CC_SYNTHESIZE(bool, m_bRage, Rage);
	//cd时间
    bool CDTimer; 
    int energyTimer;
	XMLParser* m_pXmlParser;
	//代理
	CC_SYNTHESIZE(GameDelegate*, m_pDelegateGame, DelegateGame);
};
#endif 
