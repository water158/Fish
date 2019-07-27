//
//  Manager.h
//  Qianpaobuyu
//
//  Created by etgame on 15-5-14.
//
//

#ifndef __Qianpaobuyu__Manager__
#define __Qianpaobuyu__Manager__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;
class PhysicsBodyNew;
class ManualCannon;
class Manager
{
public:
    bool init();
    static Manager* getInstance();
    void clean();
    static void destroyInstance();
    static Manager* instance;
    
public:
    PhysicsBody* loadBulletBodies(int kind);
    PhysicsBody* loadFishBodies(int kind);

	//清理缓存
	void cleanBuffer();
public:
    Vector<Sprite*> m_fishVect;
    Vector<Sprite*> d_threeVect;
    Vector<Sprite*> d_fourVect;
    Vector<Sprite*> Y_dajinVect;

private:
   // CC_SYNTHESIZE(int, m_totalCoin, M_totalCoin);
    CC_SYNTHESIZE(int, one_levelNum, One_LevelNum);
    CC_SYNTHESIZE(int, two_levelNum, Two_LevelNum);
    CC_SYNTHESIZE(int, roomNum, RoomNum);
    CC_SYNTHESIZE(int, limit_level_coin, Limit_level_coin);
    CC_SYNTHESIZE(Layer*, m_layer, M_layer);
    CC_SYNTHESIZE(__Dictionary*, bodiesDic, BodiesDic);
    CC_SYNTHESIZE(int, f_oneCoinNum, F_oneCoinNum);
    CC_SYNTHESIZE(int, f_twoCoinNum, F_twoCoinNum);
    CC_SYNTHESIZE(int, f_threeCoinNum, F_threeCoinNum);
    CC_SYNTHESIZE(int, globalControl, GlobalControl); //控制鱼的击中概率的全局变量
    CC_SYNTHESIZE(int, m_bulletPower, M_bulletPower);  //子弹威力
    CC_SYNTHESIZE(int, m_BulletNum, M_BulletNum); //记录炮管的个数
    CC_SYNTHESIZE(Scene*, m_scene, M_scene);
    //是否点击进入Loading界面
	CC_SYNTHESIZE(bool, m_bEnterLoading, EnterLoading);
	//是否点击进入游戏界面
	CC_SYNTHESIZE(bool, m_bEnterGame, EnterGame);
    
    CC_SYNTHESIZE(PhysicsBody*, bulletBodies, BulletBodies); //子弹刚体
    CC_SYNTHESIZE(PhysicsBody*, fishBodies, FishBodies); //鱼刚体
    CC_SYNTHESIZE(bool, isCoinChange, IsCoinChange); //金币是否发生变化
    CC_SYNTHESIZE(bool, isStopBoom, IsStopBoom); //是否停止鱼潮标记
	CC_SYNTHESIZE(unsigned int, m_nCurLockUID, CurLockUID);
	CC_SYNTHESIZE(bool, isLockFish, IsLockFish); //是否锁定鱼
	CC_SYNTHESIZE(bool, m_bInSmallGame, InSmallGame);
	CC_SYNTHESIZE(bool, m_bOpenFromVIP, OpenFromVIP);
	//关闭VIP等级不足界面打开VIP界面
	CC_SYNTHESIZE(bool, m_bNeedOpenVIP, NeedOpenVIP);
	//子弹的数量
	CC_SYNTHESIZE(int, m_nBulletNum, BulletNum);
	//loading界面的TIP ID
	CC_SYNTHESIZE(int, m_nTipID, TipID);
	//鱼的ID
	CC_SYNTHESIZE(int, m_nFishID, FishID);
	//全民奖金池金币数量
	CC_SYNTHESIZE(float, m_nPoolGoldNum, PoolGoldNum);
	//四人奖金池金币数量
	CC_SYNTHESIZE(unsigned int, m_nManyPoolGoldNum, ManyPoolGoldNum);
	//是否释放奖金鱼
	CC_SYNTHESIZE(bool, m_bReleaseBoxFish, ReleaseBoxFish);
	//三条任务鱼的ID
	CC_SYNTHESIZE(int, m_nTaskFishID1, TaskFishID1);
	CC_SYNTHESIZE(int, m_nTaskFishID2, TaskFishID2);
	CC_SYNTHESIZE(int, m_nTaskFishID3, TaskFishID3);
	//本轮已结束
	CC_SYNTHESIZE(bool, m_bTaskOver, TaskOver);
	//出boss的下标
	CC_SYNTHESIZE(int, m_nCreateBossIndex, CreateBossIndex);
	//有无网络
	CC_SYNTHESIZE(bool, m_bInternet, Internet);
	//是否刚进游戏
	CC_SYNTHESIZE(bool, m_bFirstEnter, FirstEnter);
	//签到界面关闭以后是否打开免费金币界面
	CC_SYNTHESIZE(bool, m_bOpenFreeGoldFromSign, OpenFreeGoldFromSign);
	//支付方式
	CC_SYNTHESIZE(int, m_nPayWays, PayWays);
	//否是可以进行MM支付
	CC_SYNTHESIZE(bool, m_bMMPay, MMPay);
	//是否弹出礼包界面
	CC_SYNTHESIZE(bool, m_bPopGift, PopGift);
	//是否刷新换炮界面
	CC_SYNTHESIZE(bool, m_bRefreshGun, RefreshGun);
};
#endif
