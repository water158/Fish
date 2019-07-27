#ifndef _MENU_LAYER_H_
#define _MENU_LAYER_H_

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/UIButton.h"
#include "cocostudio/CocoStudio.h"
#include "MyTools.h"
#include "PublicLayer.h"
#include "MyButton.h"
#include "ui/CocosGUI.h"
#include "Manager.h"
#include "Data.h"
#include "MenuDelegate.h"


using namespace cocos2d;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace cocostudio;
class Manager;
class MusicData;
class FishTV;
class MenuLayer;

class MenuLayer : public Layer, public MenuDelegate
{
public:
	MenuLayer();
	virtual ~MenuLayer();

	CREATE_FUNC(MenuLayer);

	virtual bool init();

	virtual void update(float dt);
	//触摸
	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
	//按键
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, cocos2d::Event *event);

	//异步加载
	void updateLoad(float dt);
	//添加加载图层
	void addLoadingLayer();

	/***********代理*********/
	//点击多人关卡
	virtual void clickManyMission();
	//点击单人游戏
	virtual void clickSingleGame();
	//点击多人游戏
	virtual void clickManyGame();

	void createFish(int nType);
	void callBackRemove(Node* pNode);
	//显示单人场选择界面
	void showSingle();
	//显示多人场选择界面
	void showMany();

	//左下角的按钮集合
	void callBackMenu(Ref* pObject, ui::Widget::TouchEventType ty);
	//设置
	void callBackConfigure1(Ref* pObject, ui::Widget::TouchEventType ty);
	//帮助
	void callBackHelp(Ref* pObject, ui::Widget::TouchEventType ty);
	//客服
	void callBackCustomerService(Ref* pObject, ui::Widget::TouchEventType ty);
	//兑换金币
	void callBackExchangeGold(Ref* pObject, ui::Widget::TouchEventType ty);
	//单人模式
	void callBackSingle(Ref* pObject, ui::Widget::TouchEventType ty);
	//多人模式
	void callBackMany(Ref* pObject, ui::Widget::TouchEventType ty);
	//设置按钮
	void callBackConfigure(Ref* pObject, ui::Widget::TouchEventType ty);
	//商店按钮
	void callBackShop(Ref* pObject, ui::Widget::TouchEventType ty);
	//免费领取
	void callBackFree(Ref* pObject, ui::Widget::TouchEventType ty);
	//话费兑换
	void callBackExchange(Ref* pObject, ui::Widget::TouchEventType ty);
	//领取月卡
	void callBackMonthCard(Ref* pObject, ui::Widget::TouchEventType ty);
	//特惠礼包
	void callBackGratiaPackage(Ref* pObject, ui::Widget::TouchEventType ty);
	//VIP特权
	void callBackVIP(Ref* pObject, ui::Widget::TouchEventType ty);
	//游戏商城
	void callBackGameShop(Ref* pObject, ui::Widget::TouchEventType ty);
	//点击关卡
	void callBackClickMission(Ref* pObject, ui::Widget::TouchEventType ty);
	//点击多人关卡
	void callBackClickMissionMany(Ref* pObject, ui::Widget::TouchEventType ty);
	//解锁跑
	void callBackLock(Ref* pObject, ui::Widget::TouchEventType ty);
	//当前关卡
	void callBackCurMission(Ref* pObject, ui::Widget::TouchEventType ty);
	//返回按钮
	void callBackReturn(Ref* pObject, ui::Widget::TouchEventType ty);
	//开始游戏的按钮
	void callBackBegin(Ref* pObject, ui::Widget::TouchEventType ty);
	//神秘游戏的按钮
	void callBackSmallGame(Ref* pObject, ui::Widget::TouchEventType ty);

	//显示电视
	void showTV(int nMission);
	//电视消失
	void stopTV();
	//出场完毕回调
	void callBackShowFinish(Node* pNode);
	//收回回调
	void callBackStopFinish(Node* pNode);
	//更新关卡解锁信息
	void updateLock(int nMaxUnlock);
	//添加背景特效
	void addBackgroundEffect();
	//隐藏菜单按钮回调
	void callBackHide(Node* pNode);
	//添加电视遮罩
	void addTVShiled();


	void callbackDeviceInfoFromJava(Node* target, void* data);

public:
	ui::Widget* m_pUINode;
private:
	Size size;
	int m_nIndex;
	bool m_bShowingTV;
	bool m_bMoving;
	int m_nCurSelectedMission;
	int m_nMaxUnlockMission;
	int m_nLoadIndex;
	//菜单按钮是否展开
	bool m_bOpening;
	//公共图层
	PublicLayer* m_pPublicLayer;
	//遮罩层
	ClippingNode* m_pClippingNode;
    
    Manager* instance; //单例接口

	EventListenerTouchOneByOne* m_pEventListener;
	//当前点击的按钮的TAG
	int m_nCurrentButtonTag;

	//页面ID
	int m_nCurrentPage;
};

#endif