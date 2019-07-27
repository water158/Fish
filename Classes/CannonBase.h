//
//  CannonBase.h
//  Helloworld
//
//  Created by etgame on 15-3-6.
//
//

#ifndef __Helloworld__CannonBase__
#define __Helloworld__CannonBase__

#include <iostream>
#include "cocos2d.h"
#include "SpellNumber.h"
USING_NS_CC;
class Data;
class Manager;
class MusicData;

#define kTagFreeGoldBack 110
#define kTagMenuWords 111
#define kTagRewarded 112
#define kTagDiamondsNum 113
#define kTagVIPNum 114
#define kTagFinishRank 115

class CannonBase : public Sprite
{
public:
    
    void initCannonBase();
    CannonBase();
    virtual ~CannonBase();
    
    void kindOfWeapon(int number);//武器类型
    void initPlayer();
    void updateCannon(Ref* sender);
    void setFlame(int _num); //设置喷火效果
    void removeFlame(int _num); //删除喷火效果
    void changePower(); //子弹威力改变时调用
    void changeScore(int coinNum); //金币数改变
	virtual void changeCannonPos();   //改变金币框的位置
	//在金币条上显示的数量
	Vec2 getAddCoinPosition(); 
	void addChangeAnimation(Vec2 pos);
	void callBackRemove(Node* pNode);
	//更新用户信息
	virtual void refreshUserdata();
	//更新名次
	void refreshRanking(int nNum1, int nNum2, int nNum3);
	//检测任务鱼
	void checkTaskFish(int nID);
	//获取完成的个数
	int getFinishNum();
	//重置任务数据
	void resetTaskData();
	//获取是否完成任务
	bool getIsFinishTask();

	//领取免费金币的回调
	void callBackReceiveFreeGold(Ref* pObj);
	//回收金币的效果
	void collectCoinEffect(float dt);
	//移除自己
	void callBackRemoveSelf(Node* pNode);
public:
    Sprite* cannon;  //大炮
    Sprite* battery;//炮台
    Sprite* display; //显示面板

    Sprite* up_sp; //升级按钮
    Sprite* down_sp; //降级按钮
    Sprite* labelSlot; //金币显示槽
    //Label* power_label;
    SpellNumber* spell_power;  //子弹威力显示
    SpellNumber* spell_score; // 分数显示
    //Label* tsLabel; //分数显示
    CC_SYNTHESIZE(int , scope, Scope);//大炮的范围
    int bulletPower;//子弹威力
    
    int type; // 炮台的类型 和型号
    int num;  //同种子弹 的不同类型编号
   
    int totalCorn;// 总金币数
    Sprite* player;//虚拟玩家
    Sprite* flame1, *flame2, *flame3; //火焰
//    MainScene* cn_mainscene;
    Manager* instance;
    MusicData* ca_music;
	CC_SYNTHESIZE(int, m_chosePositionNum, M_chosePositionNum);
	CC_SYNTHESIZE(int, m_nPlayerID, PlayerID);
	//是否已经换炮
	CC_SYNTHESIZE(bool, isEnergyCannon, IsEnergyCannon);
	CC_SYNTHESIZE(int, c_powerNum, powerNum);
	//钻石数量
	CC_SYNTHESIZE(int, m_nDiamondsNum, DiamondsNum);
	//当前捕获的数量
	CC_SYNTHESIZE(int, m_nTaskFishNum1, TaskFishNum1);
	CC_SYNTHESIZE(int, m_nTaskFishNum2, TaskFishNum2);
	CC_SYNTHESIZE(int, m_nTaskFishNum3, TaskFishNum3);
	//是否锁定了鱼
	CC_SYNTHESIZE(bool, m_bLockFish, LockFish);
	//当前排名
	CC_SYNTHESIZE(int, f_nCurrentRanking, CurrentRanking);
};
#endif
