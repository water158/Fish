//
//  MainScene.h
//  Helloworld
//
//  Created by etgame on 15-1-28.
//
//

#ifndef __Helloworld__MainScene__
#define __Helloworld__MainScene__

#include <iostream>

#include "cocos2d.h"
#include "GlobalDelegate.h"


USING_NS_CC;

#define kTagFishMoveAction 101
#define CD1 10
#define CD2 10
#define CD3 10
#define CD4 10
#define SkillTime1 10
#define SkillTime2 10
#define SkillTime3 10
#define SkillTime4 10
#define SkillTime5 10
#define kTagFishMedol 1001
#define kTagBg_1 1002
#define kTagBg_2 1003
#define kTagShiledLayer 1004
class Manager;
class ManualCannon;
class PublicLayer;
class Fire;
class MyMenu;
class UserDataMission;
class UserData;
class CountdownLayer;
class MainScene : public Layer, public GameDelegate
{
public:
    virtual bool init();
    static Scene* scene();
    CREATE_FUNC(MainScene);
    MainScene();
    virtual ~MainScene();
    void setPhyWorld(PhysicsWorld* world) {m_world = world;};
    void addWaitIn();//添加“等待加入”
    void changeManualPosition(Ref* ref, Point pos);//改变主炮台的位置
    void addAutoCannon(float dt);//添加自动炮台---模拟玩家
    void updateFish(float dt);
	void updateUserdata(float dt);
    void addFishSpline(Node* sp, PointArray* pArray);
    void startBoom(Node* p);
    void recordBoomTime(float dt);//记录鱼潮时间
    void recordLastCoin(float delta);
    //物理系统碰撞开始
    bool onContactBegin(const PhysicsContact& contact);
	//根据ID添加自动炮
	void addAutoCannonByID(int nID, int nChoose);
	//获取当前自动炮的数量
	int getCurrentCannonNum();

	//检查金币是否足够
	void checkCoin(int nGold);
	//捕获话费的动画
	void catchTelephoneFareAni(Vec2 posTo, int nFare);
	//捕获能量炮的动画
	void catchEnergyCannonAni(Vec2 posTo);
	//捕获宝箱的动画
	void catchBoxAni(Vec2 posTo, bool bSelf);
	//捕获钻石和技能的动画
	void catchDiamondAndSkillAni(Vec2 posTo, Vec2 posInit, int nType, int nNum, bool self);
	//鱼潮之后加速
	void callBackResumeSpeed();
	//随机获取一个地图ID
	int getRandomMapID();
	//移除炮台
	void callBackRemoveFire(Node* pNode);
	//显示捕捉到宝箱的动画
	void callBackGetBoxLayer(Node* pNode);

	/*****************多人场任务**************/
	//随机任务鱼ID
	void randFishID();
	//检测任务鱼
	void checkTaskFish(int nID);
	//获取完成个数
	int getFinishNum();
    
    /*****************道具释放****************/
	//定屏
	virtual void useToolStopFish(Ref* pObj);
	//极速
	virtual void useToolSpeedUp(Ref* pObj);
	//狂暴
	virtual void useToolRage(Ref* pObj);
	//锁定
	virtual void useToolLockFish(Ref* pObj);
	//万炮
	virtual void useToolTenThousand(Ref* pObj);
	//机器人释放道具
	virtual void useToolStopFish1();
	virtual void useToolLockFish1();	//回复运动

	//停止发射子弹
	virtual void stopShooting(); 
	//停止鱼潮
	virtual void stopFishBoom();
	//显示礼包
	virtual void showGiftLayer();
	//显示破产礼包
	virtual void showUnenoughLayer();
	//显示商店
	virtual void shopLayer();
	//刷新游戏金币和钻石数量
	virtual void refreshGoldAndDiamonds();
	//随机任务鱼的ID
	virtual void resetManyTask();
	//获取任务进度
	virtual int getCurrentFinishNum(int id);

	void callBackResumeAction(Node* pNode);
	void callBackResumeAction1(Node* pNode);	//回复CD
	void callBackResumeCD1(Node* pNode);
	//显示定住特效
	void updateStopEffect(float dt);
	//显示定住特效
	void updateStopEffect1(float dt);	//移除自己
	void callBackRemoveSelf(Node* pNode);
	//移除锁定状态
	void callBackRemoveLock(Node* pNode);
	//移除万炮状态
	void callBackRemoveTenThousand(Node* pNode);
	//处理锁定事件
	void handleLockEvent(Vec2 pos);
	//获取时候有被锁定的鱼的位置
	Vec2 getLockedFishPosition();
	//更新技能条的位置（多人场）
	void refreshSkillMenuPosition();
	//添加刷新CD显示
	void addCDBar(Node* pNode, int nType, int nTime);
    //换炮
    void chageManualCannon(int kind, int num);
    //返回上个场景
    void backToFront();
	//更新机器人的技能释放
	void updateUseSkill(float dt);
	//检测道具数量
	bool checkSkillNum(int id);

	//屏幕被定住的时候不会出新的鱼
	bool m_bPaused; 
	bool m_bPaused1;
	//特效显示下标
	int m_nEffectIndex;
	int m_nEffectIndex1;	//是否已经锁定
	bool m_bLocked;
	//CD时间
	bool m_bCDTime;
	//保存之前的威力
	int m_nOldPower;
	//技能释放的下标
	int m_nSkillIndex;
	//加速的下标
	int m_nSpeedIndex;
public:
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual void onTouchCancelled(Touch *touch, Event *unused_event);
    //按键响应
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, cocos2d::Event *event);
       
    
    virtual void update(float dt);
    void setManualRotation(Point clickPoint);//触摸响应后炮台的角度
    void loadCache();
    void addCoinPrompt(Node* node, int fKind, Point fishPoint, int coinNum);
    void adjustTheData();
    
    void removeSanYuanSiXi(Node* s_node);
    Vector<Sprite*> m_bulletVect;
    PhysicsWorld* m_world;
    unsigned int m_nCurLockUID;
private:
    
    Size size;
    Manager* instance;
    int powerOfBullet;
    int arc_num; //鱼的随机种类
    int fish_path_num;//记录鱼的路线
    bool isthrough; //是否有通过波峰
    int peak; //限制金币的峰值
    __Dictionary* plistDic;
    
    ManualCannon* manualCannon;//手动炮台
    PublicLayer* m_pPublicLayer;
    int m_recordAddFishRime;//记录加载鱼群的时间
    int boomTimer;//记录鱼潮的时间
    Vector<MyMenu*> menuPosVect;//大炮的坐标点
    int delayTimeAddFish; //延迟加鱼的时间
    int levelNum;  //记录当前关卡
    int medolcdTime; //加载鱼潮时，需要缓冲的时间
    UserDataMission* missionData;
    UserData* u_data;
    
    bool isActivation; //是否激活  控制踢出房间
    int recordSleepTime;//记录睡眠时间
    bool isCountdownApper; //倒计时层是否存在
   
    bool isBlastingMachine; //是否爆机
    bool isBoom; //是否有鱼潮出现
	bool m_bSpeedUp;
	//出正常的鱼
	bool m_bNormalFish;
	//当前的背景ID
	int m_nCurrentBgID;
	//当前操作的自动炮
	int m_nCurrentCannonTag;
	//当前游戏进行的时间
	int m_nCurrentGameTime;
	EventListenerTouchOneByOne* m_pEventListener;
};
#endif