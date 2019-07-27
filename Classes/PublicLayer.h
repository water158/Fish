#ifndef _PUBLIC_LAYER_H_
#define _PUBLIC_LAYER_H_

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/UIButton.h"
#include "ui/UIText.h"
#include "cocostudio/CocoStudio.h"
#include "MyTools.h"
#include "MyButton.h"
#include "GlobalDelegate.h"
#include "MyXmlParser.h"
#include "UserData.h"
#include "UserDataMission.h"
#include "ui/CocosGUI.h"
#include "Data.h"
#include "Manager.h"
#include "MusicData.h"
#include "MenuDelegate.h"
#include "HttpInstance.h"

class MenuLayer;
class StoreManager;
using namespace cocos2d;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace cocostudio;

enum 
{
	LAYER_FREEGOLD,
	LAYER_EXCHANGE,
	LAYER_SHOP,
	LAYER_SHOP_DIAMOND,
	LAYER_SWITCHGUN,
	LAYER_SINGN,
	LAYER_AWARD,
	LAYER_CONFIGURE,
	LAYER_GAME,
	LAYER_TASK,
	LAYER_CROSS,
	LAYER_HELP,
	LAYER_SPECIAL,
	LAYER_FISHRATE,
	LAYER_MANYGAME,
	LAYER_UPGRADE,
	LAYER_GIFT,
	LAYER_MONTHCARD,
	LAYER_MONTHCARD_RECEIVE,
	LAYER_VIP,
	LAYER_GETBOX,
	LAYER_SERVICE,
	LAYER_EXCHANGEGOLD,
	LAYER_FIRSTGIFT,
	LAYER_UNENOUGH,
	LAYER_GOLDGIFT,
};

#define kTagCoinsLayer 104
#define kTagGuideArrow 106
#define kTagGunIcon 107
class MusicData;
class PublicLayer : public Layer
{
public:
	PublicLayer();
	virtual ~PublicLayer();

	CREATE_FUNC(PublicLayer);

	virtual bool init();

	//异步加载
	void plistImageAsyncCallback(cocos2d::Texture2D* texture);
	void plistImageAsyncCallback1(cocos2d::Texture2D* texture);
	void plistImageAsyncCallback2(cocos2d::Texture2D* texture);
	
	/********免费金币*******/
	//微信分享按钮
	void callBackShare(Ref* pObject, ui::Widget::TouchEventType ty);
	//签到按钮
	void callBackSign(Ref* pObject, ui::Widget::TouchEventType ty);
	//上线活动按钮
	void callBackOnline(Ref* pObject, ui::Widget::TouchEventType ty);

	/********游戏商城*******/
	//购买按钮
	void callBackBuy(Ref* pObject, ui::Widget::TouchEventType ty);
	//隐藏首冲双倍
	void hideDouble();

	/********签到界面*******/
	//购买按钮
	void callBackSignToday(Ref* pObject, ui::Widget::TouchEventType ty);
	//显示回金币效果
	void showCoinEffect();
	//更新回金币效果
	void updateCoinEffect(float dt);
	/********领取金币界面*******/
	//购买按钮
	void callBackCollectCoins(Ref* pObject, ui::Widget::TouchEventType ty);
	//自动关闭回调
	void callBackAutoClose();

	/********游戏设置界面*******/
	//开关回调
	void valueChanged(Ref* sender, Control::EventType controlEvent);

	/********更换炮台界面*******/
	//更换炮台回调
	void callBackChangeGun(Ref* pObject, ui::Widget::TouchEventType ty);
	//装备炮台
	void callBackEquipGun(Ref* pObject, ui::Widget::TouchEventType ty);
	//添加炮的ICON
	void addIconForGun(MyButton* pButton);	
	//刷新换炮界面
	void callBackRefreshGun(Node* pNode);	

	/********客服界面*******/
	//兑换回调
	void callBackExchangeGold(Ref* pObject, ui::Widget::TouchEventType ty);

	/********游戏主界面*******/
	//领取金币
	void callBackReceiveCoins(Ref* pObject, ui::Widget::TouchEventType ty);
	//定屏
	void callBackStopFish(Ref* pObject, ui::Widget::TouchEventType ty);
	//加速
	void callBackSpeedUp(Ref* pObject, ui::Widget::TouchEventType ty);
	//狂暴
	void callBackRage(Ref* pObject, ui::Widget::TouchEventType ty);
	//锁定
	void callBackLockFish(Ref* pObject, ui::Widget::TouchEventType ty);
	//万炮
	void callBackTenThousand(Ref* pObject, ui::Widget::TouchEventType ty);
	//返回
	void callBackReturn(Ref* pObject, ui::Widget::TouchEventType ty);
	//菜单
	void callBackMenu(Ref* pObject, ui::Widget::TouchEventType ty);
	//帮助
	void callBackHelp(Ref* pObject, ui::Widget::TouchEventType ty);
	//设置
	void callBackTenConfigure(Ref* pObject, ui::Widget::TouchEventType ty);
	//加金币
	void callBackAddGold(Ref* pObject, ui::Widget::TouchEventType ty);
	//充值
	void callBackShop(Ref* pObject, ui::Widget::TouchEventType ty);
	//换炮
	void callBackSwitchGun(Ref* pObject, ui::Widget::TouchEventType ty);
	//VIP
	void callBackShowVIP(Ref* pObject, ui::Widget::TouchEventType ty);
	//四人彩金的进度显示
	void callBackShowTaskNum(Ref* pObject, ui::Widget::TouchEventType ty);
	//刷新技能剩余次数
	void refreshSkillNum();
	//刷新多人场任务情况
	void refreshManyTask();
	//创建显示的鱼
	Sprite* createFishSprite(int nID);

	/********弹出框选择界面*******/
	//确定
	void callBackOk(Ref* pObject, ui::Widget::TouchEventType ty);
	//取消
	void callBackCancel(Ref* pObject, ui::Widget::TouchEventType ty);

	/********任务界面*******/
	//打开任务界面
	void callBackTaskHelp(Ref* pObject, ui::Widget::TouchEventType ty);
	//完成一个任务
	void finishedOneTask();
	//完成两个任务
	void finishedTwoTask();
	//显示中间的任务
	void callBackShowBigDialog();
	//隐藏中间的任务
	void callBackHideDialog();
	//隐藏左边的任务一
	void callBackHideLeftFirstTask();
	//隐藏左边的任务二
	void callBackHideLeftSecondTask();
	//显示左边的任务一
	void callBackShowLeftFirstTask();
	//显示左边的任务二
	void callBackShowLeftSecondTask();
	//移除锁
	void callBackRemoveTaskLock();

	/********过关界面*******/
	//打开过关界面
	void callBackGetReward(Ref* pObject, ui::Widget::TouchEventType ty);
	//继续游戏
	void callBackGoonGame(Ref* pObject, ui::Widget::TouchEventType ty);
	//下一关
	void callBackNext(Ref* pObject, ui::Widget::TouchEventType ty);
	//显示按钮
	void callBackShowCoins(Node* pNode);
	//添加金币图层
	void addCoinsLayer();
	void dropCoins(float dt);

	/********帮助界面*******/
	//鱼的特殊玩法
	void callBackSpecial(Ref* pObject, ui::Widget::TouchEventType ty);
	//鱼的倍率
	void callBackRate(Ref* pObject, ui::Widget::TouchEventType ty);
	//加载鱼的动画资源
	void loadFishAnimation();
	//添加鱼的倍率界面
	void addFishRateAnimation();

	/********话费兑换界面*******/
	//兑换按钮
	void callBackExchange(Ref* pObject, ui::Widget::TouchEventType ty);

	/********礼包界面*******/
	bool getIsShowGiftLayer();
	bool getIsShowShopLayer();
	bool getIsShowFreeGoldLayer();

	/********VIP界面*******/
	//充值按钮
	void callBackRecharge(Ref* pObject, ui::Widget::TouchEventType ty);

	/********获得宝箱界面*******/
	//打开宝箱的按钮
	void callBackOpenBox(Ref* pObject, ui::Widget::TouchEventType ty);

	/*******免费领取10000金币界面*******/
	//领取按钮
	void callBackReceiveGreenHandGold(Ref* pObject, ui::Widget::TouchEventType ty);
	
	/********界面切换*******/
	//显示免费金币界面
	void showFreeGoldLayer();
	//显示商店界面
	void showShopLayer();
	//显示钻石商店界面
	void showDiamondShopLayer();
	//显示更换炮台界面
	void showSwitchGunLayer();
	//显示签到界面
	void showSignLayer(bool bScale = true);
	//显示领取金币界面
	void showCollectCoinsLayer(bool bScale = true);
	//显示话费兑换界面
	void showExchangeLayer();
	//显示设置界面
	void showConfigureLayer();
	//显示游戏界面
	void showGameLayer();
	//双按钮确认框
	void showSelectLayer();
	//确认按钮框
	void showSureLayer();
	//显示捕获话费条
	void showMessageBarLayer();
	//显示金币不足的界面
	void showCoinNotEnoughLayer();
	//显示任务界面
	void showTaskLayer();
	//显示左边的任务界面
	void showLeftTaskLayer();
	//显示过关界面
	void showCrossLayer();
	//显示帮助界面
	void showHelpLayer();
	//显示特殊玩法界面
	void showSpecialPlayLayer();
	//显示倍率界面
	void showFishRateLayer();
	//显示多人游戏界面
	void showManyGameLayer();
	//显示爆机界面
	void showBlastingMachineLayer();
	//显示未下注提示
	void showNotBetLayer();
	//显示话费不足界面
	void showFareNotEnoughLayer();
	//显示升级界面
	void showUpgradeLayer();
	//显示获得话费界面
	void showGetTelephoneFareLayer(int nFare, int nPlayerID);
	//显示礼包界面
	void showGiftLayer();
	//显示月卡界面
	void showMonthCardLayer();
	//显示月卡领取界面
	void showMonthCardReceiveLayer();
	//公共确认框
	void showPublicSureLayer(const char* pWords, int nNum = -1);
	//显示VIP界面
	void showVIPLayer();
	//显示获取宝箱界面
	void showGetBoxLayer();
	//显示客服界面
	void showServiceLayer();
	//显示金币兑换界面
	void showExchangeGoldLayer();
	//显示首次进游戏礼包界面
	void showFirstGiftLayer();
	//显示破产礼包界面
	void showUnenoughGiftLayer();
	//显示10000金币礼包界面
	void showGoldGiftLayer();

	//其他函数
	void callBackRemove(Node* pNode);
	void callBackRemoveCoins(Node* pNode);
	//公共关闭按钮
	void callBackCommonClose(Ref* pObject, ui::Widget::TouchEventType ty);
	void callBackCommonClose1(Ref* pObject, ui::Widget::TouchEventType ty);
	void callBackCommonClose2(Ref* pObject, ui::Widget::TouchEventType ty);
	//全部隐藏掉
	void hideLayer();
	//设置界面可见
	void setLayerVisible(int nID, bool bScale = true);
	//出场完毕回调
	void callBackShowFinish(Node* pNode);
	//隐藏菜单按钮回调
	void callBackHide(Node* pNode);
	//添加屏蔽层
	void addShiledLayer();
	void addShiled1Layer();
	void addShiled2Layer();
	//隐藏按钮回调visible(false)
	void callBackHideButton(Node* pNode);
	//隐藏图层回调visible(false)
	void callBackHideLayerOut(Node* pNode);
	//添加按钮特效
	void addButtonEffect(Node* pNode);
	void update(float dt);
	//信息条刷新
	void updateMessageBar(float dt);

	void graySprite(Node * sprite);
	//更新领奖时间
	void updateRewardTime(float dt);
	//通过当前时间获取相应的下标
	int getIndexByCurTime();
	//通过当前时间下标获取能够领取的金币
	int getCoinsNumByCurIndex(int nIndex);
	//通过当前时间下标获取具体时间
	int getTimeByCurIndex(int nIndex);
	//获取当前关的任务完成情况
	int getTaskIndex(int nStage);
	//刷新任务显示
	void refreshTaskNum();
	//刷新menu的金币
	void refreshMenuGold();
	//刷新当前VIP值
	void refreshCurrentVIP();
    //经验值
    void refreshExperiencePoints(int nExp);
	//更新多人场的技能条
	void refreshSkillMneuPosition(int nPosID);


	/********游戏引导*******/
	//随机添加技能引导
	void addRandomSkillGuide();
	//更新引导
	void updateGuide(float dt);

	//退出游戏
	void exitGame();

public:
	ui::Widget* m_pUINode;
	XMLParser* m_pXmlParser;
private:

	//签到的次数
	//int m_nSignedNum;
	CC_SYNTHESIZE(int , m_nSignedNum, SignedNum);
	//代理
	CC_SYNTHESIZE(GameDelegate*, m_pDelegateGame, DelegateGame);
	CC_SYNTHESIZE(MenuDelegate*, m_pDelegateMenu, DelegateMenu);
	//菜单按钮是否展开
	CC_SYNTHESIZE(bool, m_bOpening, Opening);
	//菜单是否正在移动
	CC_SYNTHESIZE(bool, m_bMoving, Moving);
	//今天是否领取
	CC_SYNTHESIZE(bool, m_bReceived, Received);
	//是否是在游戏中打开
	CC_SYNTHESIZE(bool, m_bOpenInGame, OpenInGame);
	//是否是在解锁跑金币不足时进入
	CC_SYNTHESIZE(bool, m_bOpenOnUnenough, OpenOnUnenough);
	//是否是在进入多人间金币不足时进入
	CC_SYNTHESIZE(bool, m_bOpenOnEnterMany, OpenOnEnterMany);
	//是否是在帮助界面中打开
	CC_SYNTHESIZE(bool, m_bOpenInHelp, OpenInHelp);
	//是否是在游戏中
	CC_SYNTHESIZE(bool, m_bInGame, InGame);
	//是否直接关闭
	CC_SYNTHESIZE(bool, m_bDirectClose, DirectClose);
	//更新按钮特效的下标
	CC_SYNTHESIZE(int , m_nEffectIndex, EffectIndex);
	//已经领取的金币的下标（分为12步）
	CC_SYNTHESIZE(int , m_nReceivedCoinIndex, ReceivedCoinIndex);
	//当前时间对应的下标（分为12步）
	CC_SYNTHESIZE(int , m_nCurTimeIndex, CurTimeIndex);
	//当前任务完成的进度
	CC_SYNTHESIZE(int , m_nFinishTaskIndex, FinishTaskIndex);
	//过关金币走动数量
	CC_SYNTHESIZE(int , m_nCrossCoinIndex, CrossCoinIndex);
	//更新信息条的下标
	CC_SYNTHESIZE(int , m_nMessageIndex, MessageIndex);
    //爆机界面 标记
	CC_SYNTHESIZE(int , m_nmainLevel, M_nmainLevel);
    //更新引导的下标
	CC_SYNTHESIZE(int , m_nGuideIndex, GuideIndex);
	//金币效果数量
	CC_SYNTHESIZE(int , m_nGoldEffectNum, GoldEffectNum);
    
    
    /***********付费接口*************/
	void callbackConfigure(Node* target, void* data);


	/***********网络检查回调*************/
	void callbackDeviceInfoFromJava(Node* target, void* data);
	/***********获取付费订单的回调*************/
	void callbackGetOrder(Node* target, void* data);
	//检查网络
	void getDeviceInfo(int fake_code);
	void callbackExit(Node* target, void* data);
	//刷新付费结果
	void updateRechargeResult(float dt);
	//付费
	void recharge();

	//请求付费
	void requestRecharge1();
	//请求付费
	void requestRecharge2();
	//请求付费
	void requestRecharge3();


	//对单请求次数
	int m_nCurrentRequestTimes;
    // 对单延迟时间
    float m_fValidatingDelayTime;

    MusicData* pu_music;
	Size size;
};

//帮助界面中的鱼的缩放
const float m_aFishSale[41] = {
	0.8, 0.8, 0.8, 0.8, 0.8,
	0.5, 0.5, 0.5, 0.5, 0.5,
	0.5, 0.5, 0.5, 0.5, 0.5,
	0.4, 0.4, 0.4, 0.4, 0.4,
	0.35, 0.35, 0.35, 0.35, 0.35,
	0.33, 0.33, 0.33, 0.33, 0.33,
	0.27, 0.27, 0.27, 0.27, 0.12,
	0.23, 0.23, 0.23, 0.23, 0.23,
	0.2
	};

#endif