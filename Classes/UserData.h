#ifndef _USER_DATA_H_
#define _USER_DATA_H_

#include "cocos2d.h"
#include "tinyxml.h"
#include "MyTools.h"
#include "Data.h"
#include "xxtea/xxtea.h"
#include "UserDataMission.h"


USING_NS_CC;
using namespace std;

typedef Map<int, UserDataMission*> MapUserDataMession;
class UserData
{
public:
    UserData(void);
    virtual ~UserData(void);

    static UserData* getInstance(void);

    //初始化数据
    void init();

	void check();

    //保存数据
    void save();

    //加载
    void load();

	//加载包里的数据
	void loadPackage();

	//检测userdata版本
	void checkVersion();

    //路径
    string m_sUserDataPath;

    //===========金币===========
    //钱是否够
    bool isGoldEnough(int gold);
    //增加钱
    void increaseGoldNum(int gold);
    //减少钱
    void decreaseGoldNum(int gold);

	

    //===========关卡解锁情况===========
    CC_SYNTHESIZE(MapUserDataMession, user_missions, UserMissions);
    //===========存于内存中的数据===========
	//当前选择的关卡ID
    CC_SYNTHESIZE(int, m_nSelectedMissionID, SelectedMissionID);
    
    //获得当前关卡的是否解锁，分数和等级
    UserDataMission* getUserDataMissionByID(int id);

    //根据ID，设定关卡数据
    void setUserDataMissionByID(int id, UserDataMission *&data);
	//下一关
    void replaceNextMission();
	//获取当前解锁到第几关
	int getCurrentMaxEnableStage();
	//根据炮的ID设置威力
	void setCannonEnergyByID(int id, int lv);
	//根据炮的ID获取威力
	int getCannonEnergyByID(int id);
	//增加、减少道具数量
	void increaseSkillNum(int id, int num);
	void decreaseSkillNum(int id, int num);
	//增加、减少钻石数量
	void increaseDiamonds(int num);
	void decreaseDiamonds(int num);
	//获取VIP等级
	int getCurVipLevel();
	//获取下一等级需要的充值
	int getNextVIPLevelAmount();
	//获取当前等级需要的充值
	int getCurVIPLevelAmount();

	//对象的唯一身份标识符
	unsigned int getUID();
	unsigned int m_nUID;

	int getGoldNum();
	void setGoldNum(int num);

	int getDiamonds();
	void setDiamonds(int num);

private:
    static UserData* s_Instance;
    //userdata版本
    CC_SYNTHESIZE(int, userdata_version, DataVersion);

    //===========金币===========
	//int goldnum;
	std::string m_pGoldNum;
    //CC_SYNTHESIZE(int, goldnum, GoldNum);
	//钻石
	//int m_nDiamonds;
	std::string m_pDiamonds;
	//CC_SYNTHESIZE(int, m_nDiamonds, Diamonds);
	//vip等级
	CC_SYNTHESIZE(int, m_nVipLevel, VipLevel);
	//充值金额
	CC_SYNTHESIZE(int, m_nRechargeAmount, RechargeAmount);
	//炮 是否解锁
	CC_SYNTHESIZE(int, cannon1If, Cannon1If);
    CC_SYNTHESIZE(int, cannon2If, Cannon2If);
    CC_SYNTHESIZE(int, cannon3If, Cannon3If);
    CC_SYNTHESIZE(int, cannon4If, Cannon4If);
    CC_SYNTHESIZE(int, cannon5If, Cannon5If);
	//装备炮的威力
	CC_SYNTHESIZE(int, m_nCannon1Energy, Cannon1Energy);
	CC_SYNTHESIZE(int, m_nCannon2Energy, Cannon2Energy);
	CC_SYNTHESIZE(int, m_nCannon3Energy, Cannon3Energy);
	CC_SYNTHESIZE(int, m_nCannon4Energy, Cannon4Energy);
	CC_SYNTHESIZE(int, m_nCannon5Energy, Cannon5Energy);
    
	//当前装备炮的ID
	CC_SYNTHESIZE(int, cannonID, CannonID);
	//签到的天数
	CC_SYNTHESIZE(int, signInNum, SignInNum);
    //上次签到的时间
	CC_SYNTHESIZE(int, signInLastTime, SignInLastTime);
	//每日领取的进度下标
	CC_SYNTHESIZE(int, awardIndex, AwardIndex);
	//每日领取的上次签到时间
	CC_SYNTHESIZE(int, awardLastTime, AwardLastTime);
	//当前的话费
	CC_SYNTHESIZE(int, m_nCurrentTelephoneFare, CurrentTelephoneFare);
	//用户的ID
	CC_SYNTHESIZE(int, m_nPlayerID, PlayerID);
	//购买礼包
	CC_SYNTHESIZE(int, m_nBuyGift, BuyGift);
	//首冲
	CC_SYNTHESIZE(int, m_nFirstShopping, FirstShopping);
	//金币峰值
	CC_SYNTHESIZE(int, m_nOverGoldPeak, OverGoldPeak);
	//别贴时间
	CC_SYNTHESIZE(int, m_nSubsidiesTime, SubsidiesTime);
	//补贴下标
	CC_SYNTHESIZE(int, m_nSubsidiesIndex, SubsidiesIndex);
	//技能的使用次数
	CC_SYNTHESIZE(int, m_nSkillNum1, SkillNum1);
	CC_SYNTHESIZE(int, m_nSkillNum2, SkillNum2);
	CC_SYNTHESIZE(int, m_nSkillNum3, SkillNum3);
	CC_SYNTHESIZE(int, m_nSkillNum4, SkillNum4);
	CC_SYNTHESIZE(int, m_nSkillNum5, SkillNum5);
	//是否购买月卡
	CC_SYNTHESIZE(int, m_nBuyMonthCard, BuyMonthCard);
	//上次领取月卡奖励的时间
	CC_SYNTHESIZE(int, m_nGetMonthCardLastTime, GetMonthCardLastTime);
	//领取月卡的次数
	CC_SYNTHESIZE(int, m_nNumOfReceivMonthCard, NumOfReceivMonthCard);
	//是否是首次进入游戏
	CC_SYNTHESIZE(int, m_nFirstEnterGame, FirstEnterGame);


    //音效控制
    CC_SYNTHESIZE(int, sound, Sound);
    //音乐控制
    CC_SYNTHESIZE(int, music, Music);
	//流畅
	CC_SYNTHESIZE(int, smooth, Smooth);
    //经验值
    CC_SYNTHESIZE(int, exp_value, Exp_value);
public:
	vector<int> m_aSceneVector;
};

#endif