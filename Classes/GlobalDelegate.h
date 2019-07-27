#ifndef _GLOBAL_DELEGATE_
#define _GLOBAL_DELEGATE_

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class GameDelegate
{
public:
	virtual void useToolStopFish(Ref* pObj) = 0;
	virtual void useToolSpeedUp(Ref* pObj) = 0;
	virtual void useToolRage(Ref* pObj) = 0;
	virtual void useToolLockFish(Ref* pObj) = 0;
	virtual void useToolTenThousand(Ref* pObj) = 0;
    virtual void chageManualCannon(int kind, int num) = 0;

	//机器人释放道具
	virtual void useToolStopFish1() = 0;
	virtual void useToolLockFish1() = 0;    
	//停止发射子弹
	virtual void stopShooting() = 0;    
	//停止鱼潮
	virtual void stopFishBoom() = 0;
	//显示礼包
	virtual void showGiftLayer() = 0;
	//显示破产礼包
	virtual void showUnenoughLayer() = 0;
	//显示商店
	virtual void shopLayer() = 0;
	//刷新游戏金币和钻石数量
	virtual void refreshGoldAndDiamonds() = 0;
	//随机任务鱼的ID
	virtual void resetManyTask() = 0;
	//获取任务进度
	virtual int getCurrentFinishNum(int id) = 0;
};

#endif