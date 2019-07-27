#include "MenuLayer.h"
#include "MainScene.h"
#include "FiShTV.h"
#include "Manager.h"
#include "UserData.h"
#include "LoadingLayer.h"
#include "MusicData.h"
#include "SimpleAudioEngine.h"
#include "SmallGameLayer.h"
#include "MobClickCpp.h"
#define STARTSCENE 0
#define ONELEVEL 1
#define BOTOOM 12
#define TOP 13
MenuLayer::MenuLayer()
{
	m_pEventListener = NULL;
}

MenuLayer::~MenuLayer()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(m_pEventListener);

	Manager::getInstance()->cleanBuffer();
}

bool MenuLayer::init()
{
	Layer::init();
	size = Director::getInstance()->getWinSize();
	m_nLoadIndex = 0;

	//// Register Touch Event
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	m_pEventListener = EventListenerTouchOneByOne::create();
	m_pEventListener->setSwallowTouches(true);
	m_pEventListener->onTouchBegan = CC_CALLBACK_2(MenuLayer::onTouchBegan, this);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	dispatcher->addEventListenerWithFixedPriority(m_pEventListener, 10);
	this->setKeyboardEnabled(true);

	
	if(Manager::getInstance()->getFirstEnter())
	{
		HttpInstance::getInstance()->getStore()->getDeviceInfo("getDeviceInfo", 1, this, callfuncND_selector(MenuLayer::callbackDeviceInfoFromJava));
		this->addLoadingLayer();
		this->schedule(schedule_selector(MenuLayer::updateLoad), 1.0f/60.0f);
		return true;
	}

/***************** 1 ****************/
	Manager::getInstance()->cleanBuffer();

    instance = Manager::getInstance();
    
    //添加音乐
    if (UserData::getInstance()->getMusic() == 1 || UserData::getInstance()->getSound() == 1) 
	{
        MusicData::getInstance()->loadMusic();
        MusicData::getInstance()->playMusic(START);
    }
/***************** 2 ****************/    
    
	Manager::getInstance()->setInternet(true);
	size = Director::getInstance()->getWinSize();
	m_nIndex = 0;
	m_bShowingTV = false;
	m_bMoving = false;
	m_bOpening = false;
	m_nCurSelectedMission = 0;
	m_nCurrentButtonTag = 0;
	m_nLoadIndex = 0;
	m_nCurrentPage = MENU_MAIN;
	m_nMaxUnlockMission = UserData::getInstance()->getCurrentMaxEnableStage();
	Manager::getInstance()->setFirstEnter(false);

	tm* stm = GetTime1();
	int nDay = stm->tm_mday;
	int nMin = stm->tm_min;
	int nSingle = 2000000;
	if(nDay % 2 == 0)
		nSingle = 2300000;
	int nRandNum = nMin * nSingle + rand() % 1900000;
	Manager::getInstance()->setPoolGoldNum(nRandNum);

	//背景特效
	this->addBackgroundEffect();
/***************** 3 ****************/ 
	//UI的使用
	//m_pUINode = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui/main/NewUi_1.ExportJson");
	m_pUINode = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui/main/NewUi_1.json");
	m_pUINode->setPosition(Vec2(0, 0));
	this->addChild(m_pUINode, 1); 
	m_pUINode->setScaleX(size.width/854.0);
/***************** 4 ****************/
	//菜单按钮
	//ui::MyButton* buttonMenu = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 14);
	//buttonMenu->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackMenu, this));
	//buttonMenu->setClickPicture();
	//设置
	ui::MyButton* buttonConfigure = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 227);
	buttonConfigure->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackConfigure1, this));
	buttonConfigure->setClickPicture();
	//帮助
	ui::MyButton* buttonHelp = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 645);
	buttonHelp->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackHelp, this));
	buttonHelp->setClickPicture();
	//单人模式按钮
	ui::MyButton* buttonSingle = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 7);
	buttonSingle->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackSingle, this));
	buttonSingle->setClickPicture();
	//多人模式按钮
	ui::MyButton* buttonMany = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 8);
	buttonMany->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackMany, this));
	buttonMany->setClickPicture();
	//返回按钮
	ui::MyButton* buttonReturn = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 28);
	buttonReturn->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackReturn, this));
	buttonReturn->setClickPicture();
	//多人游戏
	ui::MyButton* buttonReturn1 = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 140);
	buttonReturn1->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackReturn, this));
	buttonReturn1->setClickPicture();
	//开始游戏
	ui::MyButton* buttonBegin = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 65);
	buttonBegin->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackBegin, this));
	buttonBegin->setClickPicture();
	//神秘游戏
	ui::MyButton* buttonSmallGame = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 548);
	buttonSmallGame->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackSmallGame, this));
	buttonSmallGame->setClickPicture();	
	//免费金币
	ui::MyButton* buttonFree = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 9);
	buttonFree->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackFree, this));
	buttonFree->setClickPicture();
	//客服
	ui::MyButton* buttonService = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 987);
	buttonService->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackCustomerService, this));
	buttonService->setClickPicture();
	//金币兑换
	ui::MyButton* buttonExchangeGold = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 988);
	buttonExchangeGold->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackExchangeGold, this));
	buttonExchangeGold->setClickPicture();
	buttonExchangeGold->setVisible(false);
	//话费兑换
	ui::MyButton* buttonExchange = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 643);
	buttonExchange->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackExchange, this));
	buttonExchange->setClickPicture();
	//游戏商城
	ui::MyButton* buttonShop = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 10);
	buttonShop->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackShop, this));
	buttonShop->setClickPicture();
	//钻石商店
	ui::MyButton* buttonDiamondShop = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 868);
	buttonDiamondShop->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackShop, this));
	buttonDiamondShop->setClickPicture();
	//特惠礼包
	ui::MyButton* buttonGratiaPackage = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 857);
	buttonGratiaPackage->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackGratiaPackage, this));
	buttonGratiaPackage->setClickPicture();
	int nBuyGift = UserData::getInstance()->getBuyGift();
	if(nBuyGift >= 2)
	{
		buttonGratiaPackage->setVisible(false);
	}

	//领取月卡
	ui::MyButton* buttonMonthCard = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 860);
	buttonMonthCard->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackMonthCard, this));
	buttonMonthCard->setClickPicture();

	//VIP特权
	ui::MyButton* buttonVIP = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 861);
	buttonVIP->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackVIP, this));
	buttonVIP->setClickPicture();

	ui::MyButton* buttonShop1 = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 21);
	buttonShop1->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackShop, this));
	buttonShop1->setClickPicture();
	
	//更新金币
	int nCoins = UserData::getInstance()->getGoldNum();
	ui::TextAtlas* pTextCoins = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, 442);
	pTextCoins->setString(getStringFromInt(nCoins));

	//更新钻石
	int nDiamonds = UserData::getInstance()->getDiamonds();
	ui::TextAtlas* pTextDiamonds = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, 869);
	pTextDiamonds->setString(getStringFromInt(nDiamonds));

	//ActionManagerEx::getInstance()->playActionByName("NewUi_1.json" , "Animation0");
	ActionManagerEx::getInstance()->playActionByName("NewUi_1.json" , "Animation1");

	//点击关卡
	for (int i = 0; i < 10; i++)
	{
		ui::MyButton * buttonMission = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 70 + i);
		buttonMission->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackClickMission, this));
		buttonMission->setClickPicture();

		if(i == m_nMaxUnlockMission)
		{
			buttonMission->setFocused(true);
		}
	}

	int nManyTag = 120;
	for (int i = 0; i < 4; i++)
	{
		if(i == 0)
		{
			nManyTag = 120;
		}
		else if(i == 1)
		{
			nManyTag = 128;
		}
		else if(i == 2)
		{
			nManyTag = 131;
		}
		else if(i == 3)
		{
			nManyTag = 134;
		}
		ui::MyButton * buttonMission = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, nManyTag);
		buttonMission->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackClickMissionMany, this));
		buttonMission->setClickPicture();

	}
	updateLock(m_nMaxUnlockMission);
/***************** 5 ****************/ 
	//弹出框
	m_pPublicLayer = PublicLayer::create();
	m_pPublicLayer->setPosition(Vec2(size.width / 2 - 427, 0));
	this->addChild(m_pPublicLayer, 10);
	m_pPublicLayer->setInGame(false);
	m_pPublicLayer->setDelegateMenu(this);
	//m_pPublicLayer->checkNet(100);
/***************** 6 ****************/ 
	Layout* pMain = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 4);
	pMain->setVisible(true);
	pMain->setTouchEnabled(false);
	Layout* pList1 = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 26);
	pList1->setVisible(false);
	pList1->setTouchEnabled(false);
	pList1->runAction(MoveBy::create(0.01, Vec2(854, 0)));
	Layout* pMany = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 111);
	pMany->setVisible(false);
	pMany->setTouchEnabled(false);
	pMany->runAction(MoveBy::create(0.01, Vec2(-854, 0)));
	
	this->schedule(schedule_selector(MenuLayer::update), 1);

	//旋转
	//ImageView* pRotate = (ui::ImageView *)ui::Helper::seekWidgetByTag(m_pUINode, 24);
	//pRotate->runAction(RepeatForever::create(RotateBy::create(4.0, 360.0f)));

	ImageView* pLogo = (ui::ImageView *)ui::Helper::seekWidgetByTag(m_pUINode, 16);
	auto seq = Sequence::create(MoveBy::create(2.5f, Vec2(0.0f, 30.0f)), 
		MoveBy::create(2.5f, Vec2(0.0f, -30.0f)), NULL);
	pLogo->runAction(RepeatForever::create(seq));


	return true;
}

void MenuLayer::onKeyReleased(EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
	switch (keycode)
	{
	case EventKeyboard::KeyCode::KEY_ESCAPE:
		if(m_nCurrentPage == MENU_MAIN)
			m_pPublicLayer->exitGame();
		break;
	case EventKeyboard::KeyCode::KEY_MENU:
		break;
	default:
		break;
	}
}
//添加加载图层
void MenuLayer::addLoadingLayer()
{
	if(Manager::getInstance()->getFirstEnter())
	{
		Manager::getInstance()->setFirstEnter(false);

		LayerColor* pLayer = LayerColor::create(Color4B(0,0,0,255));
		pLayer->setContentSize(size);
		pLayer->setPosition(Vec2(0, 0));
		this->addChild(pLayer, 97, 98);

		//背景
		auto pBarBack1 = Sprite::create("loading_back.png");
		pBarBack1->setPosition(Vec2(size.width / 2, size.height / 2));
		pBarBack1->setScale(0.75f);
		pLayer->addChild(pBarBack1, 98);

		auto pBarBack = Sprite::create("ui/main/jindutiao_1.png");
		pBarBack->setPosition(Vec2(size.width / 2, size.height / 7));
		pLayer->addChild(pBarBack, 98);

		ProgressTimer* pLoadingBar = ProgressTimer::create(Sprite::create("ui/main/jindutiao_2.png"));
		pLoadingBar->setAnchorPoint(Vec2(0.5f, 0.5f));
		pLoadingBar->setPosition(Vec2(size.width / 2, size.height / 7));
		pLoadingBar->setType(kCCProgressTimerTypeBar);
		pLoadingBar->setMidpoint(Vec2(0.0f, 0.5f));
		pLoadingBar->setBarChangeRate(Vec2(1.0f, 0.0f));
		this->addChild(pLoadingBar, 98, 99);
		pLoadingBar->setPercentage(20);

		auto pName = Sprite::create("ui/main/logo_bg.png");
		pName->setPosition(Vec2(size.width / 2, size.height / 2));
		pName->setScale(0.75f);
		this->addChild(pName, 99, 100);

		//auto pBack = Sprite::create("buyulogo.jpg");
		//pBack->setPosition(Vec2(size.width / 2, size.height / 2));
		//this->addChild(pBack, 100, 101);
	}
}
//异步加载
void MenuLayer::updateLoad(float dt)
{
	m_nLoadIndex ++;
	ProgressTimer* pLoadingBar = (ProgressTimer*)this->getChildByTag(99);
	if(m_nLoadIndex == 1)
	{
		Manager::getInstance()->cleanBuffer();
		instance = Manager::getInstance();

		//添加音乐
		if (UserData::getInstance()->getMusic() == 1 || UserData::getInstance()->getSound() == 1) 
		{
		    MusicData::getInstance()->loadMusic();
		    MusicData::getInstance()->playMusic(START);
		}
		this->removeChildByTag(101);
	}
	else if(m_nLoadIndex == 2)
	{
		Manager::getInstance()->setInternet(true);
		size = Director::getInstance()->getWinSize();
		m_nIndex = 0;
		m_bShowingTV = false;
		m_bMoving = false;
		m_bOpening = false;
		m_nCurSelectedMission = 0;
		m_nCurrentButtonTag = 0;
		m_nCurrentPage = MENU_MAIN;
		m_nMaxUnlockMission = UserData::getInstance()->getCurrentMaxEnableStage();

		tm* stm = GetTime1();
		int nDay = stm->tm_mday;
		int nMin = stm->tm_min;
		int nSingle = 2000000;
		if(nDay % 2 == 0)
			nSingle = 2300000;
		int nRandNum = nMin * nSingle + rand() % 1900000;
		Manager::getInstance()->setPoolGoldNum(nRandNum);

		//背景特效
		this->addBackgroundEffect();
	}
	else if(m_nLoadIndex == 3)
	{
		if(pLoadingBar)
			pLoadingBar->setPercentage(30);
		//UI的使用
		//m_pUINode = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui/main/NewUi_1.ExportJson");
		m_pUINode = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui/main/NewUi_1.json");
		m_pUINode->setPosition(Vec2(0, 0));
		this->addChild(m_pUINode, 1); 
		m_pUINode->setScaleX(size.width/854.0);
	}
	else if(m_nLoadIndex == 4)
	{
		if(pLoadingBar)
			pLoadingBar->setPercentage(60);
		//菜单按钮
		//ui::MyButton* buttonMenu = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 14);
		//buttonMenu->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackMenu, this));
		//buttonMenu->setClickPicture();
		//设置
		ui::MyButton* buttonConfigure = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 227);
		buttonConfigure->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackConfigure1, this));
		buttonConfigure->setClickPicture();
		//帮助
		ui::MyButton* buttonHelp = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 645);
		buttonHelp->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackHelp, this));
		buttonHelp->setClickPicture();
		//单人模式按钮
		ui::MyButton* buttonSingle = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 7);
		buttonSingle->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackSingle, this));
		buttonSingle->setClickPicture();
		//多人模式按钮
		ui::MyButton* buttonMany = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 8);
		buttonMany->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackMany, this));
		buttonMany->setClickPicture();
		//返回按钮
		ui::MyButton* buttonReturn = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 28);
		buttonReturn->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackReturn, this));
		buttonReturn->setClickPicture();
		//多人游戏
		ui::MyButton* buttonReturn1 = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 140);
		buttonReturn1->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackReturn, this));
		buttonReturn1->setClickPicture();
		//开始游戏
		ui::MyButton* buttonBegin = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 65);
		buttonBegin->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackBegin, this));
		buttonBegin->setClickPicture();
		//神秘游戏
		ui::MyButton* buttonSmallGame = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 548);
		buttonSmallGame->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackSmallGame, this));
		buttonSmallGame->setClickPicture();	
		//免费金币
		ui::MyButton* buttonFree = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 9);
		buttonFree->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackFree, this));
		buttonFree->setClickPicture();
		//客服
		ui::MyButton* buttonService = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 987);
		buttonService->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackCustomerService, this));
		buttonService->setClickPicture();
		//金币兑换
		ui::MyButton* buttonExchangeGold = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 988);
		buttonExchangeGold->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackExchangeGold, this));
		buttonExchangeGold->setClickPicture();
		buttonExchangeGold->setVisible(false);
		//话费兑换
		ui::MyButton* buttonExchange = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 643);
		buttonExchange->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackExchange, this));
		buttonExchange->setClickPicture();
		//游戏商城
		ui::MyButton* buttonShop = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 10);
		buttonShop->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackShop, this));
		buttonShop->setClickPicture();
		//钻石商店
		ui::MyButton* buttonDiamondShop = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 868);
		buttonDiamondShop->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackShop, this));
		buttonDiamondShop->setClickPicture();
		//特惠礼包
		ui::MyButton* buttonGratiaPackage = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 857);
		buttonGratiaPackage->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackGratiaPackage, this));
		buttonGratiaPackage->setClickPicture();
		int nBuyGift = UserData::getInstance()->getBuyGift();
		if(nBuyGift >= 2)
		{
			buttonGratiaPackage->setVisible(false);
		}

		//领取月卡
		ui::MyButton* buttonMonthCard = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 860);
		buttonMonthCard->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackMonthCard, this));
		buttonMonthCard->setClickPicture();

		//VIP特权
		ui::MyButton* buttonVIP = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 861);
		buttonVIP->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackVIP, this));
		buttonVIP->setClickPicture();

		ui::MyButton* buttonShop1 = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 21);
		buttonShop1->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackShop, this));
		buttonShop1->setClickPicture();

		//更新金币
		int nCoins = UserData::getInstance()->getGoldNum();
		ui::TextAtlas* pTextCoins = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, 442);
		pTextCoins->setString(getStringFromInt(nCoins));

		//更新钻石
		int nDiamonds = UserData::getInstance()->getDiamonds();
		ui::TextAtlas* pTextDiamonds = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, 869);
		pTextDiamonds->setString(getStringFromInt(nDiamonds));

		//ActionManagerEx::getInstance()->playActionByName("NewUi_1.json" , "Animation0");
		ActionManagerEx::getInstance()->playActionByName("NewUi_1.json" , "Animation1");

		//点击关卡
		for (int i = 0; i < 10; i++)
		{
			ui::MyButton * buttonMission = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 70 + i);
			buttonMission->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackClickMission, this));
			buttonMission->setClickPicture();

			if(i == m_nMaxUnlockMission)
			{
				buttonMission->setFocused(true);
			}
		}

		int nManyTag = 120;
		for (int i = 0; i < 4; i++)
		{
			if(i == 0)
			{
				nManyTag = 120;
			}
			else if(i == 1)
			{
				nManyTag = 128;
			}
			else if(i == 2)
			{
				nManyTag = 131;
			}
			else if(i == 3)
			{
				nManyTag = 134;
			}
			ui::MyButton * buttonMission = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, nManyTag);
			buttonMission->addTouchEventListener(CC_CALLBACK_2(MenuLayer::callBackClickMissionMany, this));
			buttonMission->setClickPicture();

		}
		updateLock(m_nMaxUnlockMission);
	}
	else if(m_nLoadIndex == 5)
	{
		if(pLoadingBar)
			pLoadingBar->setPercentage(80);
		//弹出框
		m_pPublicLayer = PublicLayer::create();
		m_pPublicLayer->setPosition(Vec2(size.width / 2 - 427, 0));
		this->addChild(m_pPublicLayer, 10);
		m_pPublicLayer->setInGame(false);
		m_pPublicLayer->setDelegateMenu(this);
		//m_pPublicLayer->checkNet(100);
	}
	else if(m_nLoadIndex == 6)
	{
		Layout* pMain = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 4);
		pMain->setVisible(true);
		pMain->setTouchEnabled(false);
		Layout* pList1 = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 26);
		pList1->setVisible(false);
		pList1->setTouchEnabled(false);
		pList1->runAction(MoveBy::create(0.01, Vec2(854, 0)));
		Layout* pMany = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 111);
		pMany->setVisible(false);
		pMany->setTouchEnabled(false);
		pMany->runAction(MoveBy::create(0.01, Vec2(-854, 0)));

		this->schedule(schedule_selector(MenuLayer::update), 1);

		//旋转
		//ImageView* pRotate = (ui::ImageView *)ui::Helper::seekWidgetByTag(m_pUINode, 24);
		//pRotate->runAction(RepeatForever::create(RotateBy::create(4.0, 360.0f)));

		ImageView* pLogo = (ui::ImageView *)ui::Helper::seekWidgetByTag(m_pUINode, 16);
		auto seq = Sequence::create(MoveBy::create(2.5f, Vec2(0.0f, 30.0f)), 
			MoveBy::create(2.5f, Vec2(0.0f, -30.0f)), NULL);
		pLogo->runAction(RepeatForever::create(seq));

		if(pLoadingBar)
			pLoadingBar->setPercentage(100);
	}
	else if(m_nLoadIndex == 17)
	{
		this->removeChildByTag(98);
		this->removeChildByTag(99);
		this->removeChildByTag(100);

		//m_pPublicLayer->showFirstGiftLayer();
		if(m_pPublicLayer->getSignedNum() < 7 && !m_pPublicLayer->getReceived())
		{
			m_pPublicLayer->showSignLayer();
			Manager::getInstance()->setOpenFreeGoldFromSign(true);
		}
	}
}

/***********代理*********/
//点击多人关卡
void MenuLayer::clickManyMission()
{
	int nTag = m_nCurrentButtonTag;
	int nNeedVIP = 0;
	int nCurVIP = UserData::getInstance()->getCurVipLevel();
	
	int level_tag = 0;
	if(nTag == 120)
	{
		level_tag = 1;
		nNeedVIP = 0;
	}
	else if(nTag == 128)
	{
		level_tag = 2;
		nNeedVIP = 1;
	}
	else if(nTag == 131)
	{
		level_tag = 3;
		nNeedVIP = 2;
	}
	else if(nTag == 134)
	{
		level_tag = 4;
		nNeedVIP = 3;
	}
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng::MobClickCpp::event(getStringFromInt(YM_MANY_1 - 1 + level_tag).c_str());
#endif
	//金币足够可以进入
	if(nCurVIP >= nNeedVIP)
	{
		instance->setTwo_LevelNum(level_tag);
		auto pScene = Scene::create();
		pScene->addChild(LoadingLayer::create());
		Director::getInstance()->replaceScene(pScene);
	}
	//VIP等级不足
	else
	{
		m_pPublicLayer->showPublicSureLayer("vip_not_enough");
		Manager::getInstance()->setNeedOpenVIP(true);
	}
}
//点击单人游戏
void MenuLayer::clickSingleGame()
{
	MusicData::getInstance()->playMusic(ONELEVEL);
	m_nCurrentPage = MENU_SINGLE;        
	showSingle();
}
//点击多人游戏
void MenuLayer::clickManyGame()
{
	MusicData::getInstance()->playSound(BOTOOM);
	m_nCurrentPage = MENU_MANY;
	showMany();
}

//添加背景特效
void MenuLayer::addBackgroundEffect()
{
	// 最后面的背景
	Sprite* pBg0 = Sprite::create("ui/main/lobby_bg.jpg");
	pBg0->setPosition(Vec2(size.width / 2, size.height / 2));
	pBg0->setScale(0.75f);
	this->addChild(pBg0, -10);

	// 最后面的背景
	Sprite* pBg1 = Sprite::create("ui/main/lobby_bg.jpg");
	pBg1->setPosition(Vec2(size.width / 2, size.height / 2));
	pBg1->setScale(0.75f);
	//this->addChild(pBg1);

	//水波纹
	auto pNode = NodeGrid::create();
	//pNode->set
	pNode->addChild(pBg1, 10);
	pNode->setPosition(Vec2(0, 0));
	this->addChild(pNode, 0);
	auto pWaves = Waves::create(20, Size(5,4), 5, 5, true, true);
	pNode->runAction(RepeatForever::create(pWaves));

	//光线
	for (int i = 0; i < 40; i++)
	{
		Sprite* pLight = Sprite::create("ui/main/jc_light.png");
		pLight->setAnchorPoint(Vec2(0.0f, 0.5f));
		pLight->setPosition(Vec2(size.width / 2, size.height + 20.0f));
		pLight->setRotation(CCRANDOM_0_1() * 180);
		pLight->setScaleY(0.2);
		pLight->setScaleX(0.4);
		pLight->setOpacity(60);
		this->addChild(pLight);
		bool bLeft = CCRANDOM_0_1() > 0.5 ? true : false;
		float fAim = bLeft ? 180 : 0;
		float fAim1 = 180;
		if(fAim == 180)
		{
			fAim1 = 0;
		}
		pLight->runAction(RepeatForever::create(Sequence::createWithTwoActions(
			RotateTo::create(CCRANDOM_0_1()*6 + 10, fAim),
			RotateTo::create(CCRANDOM_0_1()*8 + 10, fAim1)
			)));
	}

	Sprite* pBg2 = Sprite::create("ui/main/lobby_bg_3.png");
	pBg2->setPosition(Vec2(size.width / 2, size.height / 2));
	pBg2->setScale(0.75f);
	this->addChild(pBg2);


	Sprite* pBg3 = Sprite::create("ui/main/lobby_bg_2.png");
	pBg3->setPosition(Vec2(size.width / 2, size.height / 2));
	pBg3->setScale(0.75f);
	this->addChild(pBg3);

	Sprite* pBg4 = Sprite::create("ui/main/lobby_bg_5.png");
	pBg4->setPosition(Vec2(size.width / 2, size.height / 2));
	pBg4->setScale(0.75f);
	this->addChild(pBg4);
	pBg4->runAction(RepeatForever::create(Sequence::createWithTwoActions(FadeIn::create(4.0), FadeOut::create(4.0))));


	

	//创建水泡粒子
	ParticleSystemQuad* pWater = ParticleSystemQuad::create("ui/main/Desktop.plist");
	pWater->setPosition(Vec2(size.width - 50, 50));
	this->addChild(pWater);

	ParticleSystemQuad* pWater1 = ParticleSystemQuad::create("ui/main/Desktop.plist");
	pWater1->setPosition(Vec2(100, 0));
	this->addChild(pWater1);

	ParticleSystemQuad* pWater2 = ParticleSystemQuad::create("ui/main/Desktop.plist");
	pWater2->setPosition(Vec2(size.width/3, 150));
	this->addChild(pWater2);
}
void MenuLayer::update(float dt)
{
	m_nIndex ++;


	if(m_nIndex > 46)
	{
		m_nIndex = 0;
	}

	if(m_nIndex%2 == 0)
	{
		int nFishID = rand()%6;
		createFish(nFishID);
	}


	if(m_nIndex % 12 == 0)
	{
		Sprite* pFish = Sprite::create("ui/main/lvf_fish_qun.png");
		pFish->setPosition(Vec2(-30, size.height/4*3));
		pFish->setScale(0.5);
		this->addChild(pFish);

		auto seq = Sequence::create(MoveBy::create(6.0, Vec2(size.height/4, -size.height/8)),
			CallFuncN::create(CC_CALLBACK_1(MenuLayer::callBackRemove, this)),
			NULL);
		auto seq1 = Sequence::create(DelayTime::create(4.0),
			FadeOut::create(2.0),
			NULL);
		pFish->runAction(seq);
		pFish->runAction(seq1);
	}
	else if(m_nIndex % 12 == 3)
	{
		Sprite* pFish = Sprite::create("ui/main/lvf_fish_qun.png");
		pFish->setPosition(Vec2(size.width / 4* 3, size.height/2 + 50));
		pFish->setScale(0.5);
		pFish->setFlippedX(true);
		this->addChild(pFish);

		auto seq = Sequence::create(MoveBy::create(6.0, Vec2(-size.height/4, -size.height/8)),
			CallFuncN::create(CC_CALLBACK_1(MenuLayer::callBackRemove, this)),
			NULL);
		auto seq1 = Sequence::create(
			FadeIn::create(2.0),
			DelayTime::create(2.0),
			FadeOut::create(2.0),
			NULL);
		pFish->runAction(seq);
		pFish->runAction(seq1);
	}

	bool m_bInterNet = Manager::getInstance()->getInternet();
	//更新奖池
	//随机全民彩金奖池金币数量
	Manager::getInstance()->setPoolGoldNum(Manager::getInstance()->getPoolGoldNum() + rand() % 50000 + 20000);
	unsigned int nNum = Manager::getInstance()->getPoolGoldNum();
	//全民彩金的刷新
	//奖金池金币数
	ui::TextAtlas* pPoolGold = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, 67);
	if(m_bInterNet)
		pPoolGold->setString(getStringFromInt(nNum));
	else
		pPoolGold->setString(getStringFromInt(0));
	tm* stm = GetTime1();
	int nMin = 59 - stm->tm_min;	
	int nSec = 60 - stm->tm_sec;
	std::string str = "%02d:%02d";
	__String* pFormat = __String::createWithFormat(str.c_str(), nMin, nSec);
	//时间
	ui::Text* pPoolTime = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 874);
	if(m_bInterNet)
		pPoolTime->setString(pFormat->getCString());
	else
		pPoolTime->setString(m_pPublicLayer->m_pXmlParser->getString("net_no")->getCString());
}

bool MenuLayer::onTouchBegan(Touch* touch, Event* event)
{
	if(m_bShowingTV && !m_bMoving)
	{
        MusicData::getInstance()->playSound(24);
		stopTV();			
	}

	return true;
}

void MenuLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event* unused_event)
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	for (int i = 0; i < touches.size(); i++)
	{
		if(i == 0)
		{
			if(m_bShowingTV && !m_bMoving)
			{
				MusicData::getInstance()->playSound(24);
				stopTV();			
			}
		}
	}
}

void MenuLayer::createFish(int nType)
{
	std::string strPlist = "78";
	int nFrameNum = 6;
	Vec2 initPos = Vec2(CCRANDOM_0_1() * 854, size.height/2 - 200 + CCRANDOM_0_1() * 200);
	float fWidth = size.width + 100;
	if(nType == 0)
	{
		nFrameNum = 6;
		strPlist = "78";
	}
	else if(nType == 1)
	{
		nFrameNum = 5;
		strPlist = "155";
	}
	else if(nType == 2)
	{
		nFrameNum = 9;
		strPlist = "156";
	}
	else if(nType == 3)
	{
		nFrameNum = 6;
		strPlist = "164";
	}
	else if(nType == 4)
	{
		nFrameNum = 8;
		strPlist = "274";
	}
	else if(nType == 5)
	{
		nFrameNum = 12;
		strPlist = "381";
	}
	std::string strPath = "ui/main/fish_frame_" + strPlist + ".plist";
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(strPath);
	//获取动画帧数和缓存中的标记
	Vector<SpriteFrame*> array1(nFrameNum);

	for (int i=1; i <= nFrameNum; i++)
	{
		SpriteFrameCache *pcache = SpriteFrameCache::getInstance();
		std::string format = strPlist + "_%02d.png";
		__String* strName = __String::createWithFormat(format.c_str(), i);
		SpriteFrame* pframe = pcache->getSpriteFrameByName(strName->getCString());
		array1.pushBack(pframe);
	}
	float fTime = 30 + CCRANDOM_0_1()*30;
	//根据数组创建动画并添加到动画缓存
	Animation *panimation = Animation::createWithSpriteFrames(array1
		, fTime / 300);

	bool bToLeft = CCRANDOM_0_1() > 0.5 ? false : true;
	if(bToLeft)
	{
		//initPos = Vec2(initPos.x + fWidth, CCRANDOM_0_1() * 480);
		//initPos = Vec2(CCRANDOM_0_1() * 854, CCRANDOM_0_1() * 480);
		initPos.x = CCRANDOM_0_1() * 854;
		fWidth = -fWidth;
	}

	Sprite* sp = Sprite::create();
	sp->setScale(0.5 + CCRANDOM_0_1() * 0.5);
	sp->setRotation(-90);
	if(bToLeft)
	{
		sp->setFlippedY(true);
	}
	//log("-----(%f)----(%f)-------", initPos.x, initPos.y);
	sp->setPosition(initPos);
	this->addChild(sp);

	sp->runAction(RepeatForever::create(Animate::create(panimation)));

	auto seq = Sequence::create(MoveBy::create(fTime, Vec2(fWidth, 0.0)),
		CallFuncN::create(CC_CALLBACK_1(MenuLayer::callBackRemove, this)),
		NULL);
	sp->runAction(seq);

}

void MenuLayer::callBackRemove(Node* pNode)
{
	pNode->removeFromParent();
}

//显示单人场选择界面
void MenuLayer::showSingle()
{
	instance->setRoomNum(1);//记录进入的房间

	Layout* pList1 = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 26);
	pList1->setVisible(true);

	Layout* pPannel = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 743);
	auto seq = Sequence::createWithTwoActions(MoveBy::create(0.3, Vec2(-size.width - 40, 0)), MoveBy::create(0.1, Vec2(40, 0)));
	pPannel->runAction(seq);

	ui::ScrollView * pScrollView = (ui::ScrollView *)ui::Helper::seekWidgetByTag(m_pUINode, 165);
	pScrollView->setInnerContainerSize(Size(1800, 200));

	float fPer = 0;
	if(m_nMaxUnlockMission <= 3)
		fPer = 0;
	else if(m_nMaxUnlockMission > 3 && m_nMaxUnlockMission < 7)
		fPer = (m_nMaxUnlockMission - 3) * 10;
	else 
		fPer = 100;

	pScrollView->jumpToPercentHorizontal(fPer);
}
//显示多人场选择界面
void MenuLayer::showMany()
{
	instance->setRoomNum(2);//记录进入的房间

	Layout* pMany = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 111);
	pMany->setVisible(true);

	Layout* pPannel = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 743);
	auto seq = Sequence::createWithTwoActions(MoveBy::create(0.3, Vec2(size.width + 40, 0)), MoveBy::create(0.1, Vec2(-40, 0)));
	pPannel->runAction(seq);

	ui::ScrollView * pScrollView = (ui::ScrollView *)ui::Helper::seekWidgetByTag(m_pUINode, 147);
	pScrollView->setInnerContainerSize(Size(854, 200));
}
//左下角的按钮集合
void MenuLayer::callBackMenu(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		MusicData::getInstance()->playSound(BOTOOM);
		if(m_bMoving)
		{
			return;
		}
		ui::MyButton* buttonMenu = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 14);
		ui::MyButton* buttonConfigure = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 227);
		ui::MyButton* buttonHelp = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 645);
		//ui::MyButton* buttonNitice = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 229);
		ui::ImageView* pImage = (ui::ImageView *)ui::Helper::seekWidgetByTag(m_pUINode, 15);
		if(m_bOpening)
		{
			auto seq = Sequence::create(MoveTo::create(0.2, buttonMenu->getPosition()),
				CallFuncN::create(CC_CALLBACK_1(MenuLayer::callBackHide, this)),
				NULL);
//			auto seq1 = Sequence::create(MoveTo::create(0.2, buttonMenu->getPosition()),
//				CallFuncN::create(CC_CALLBACK_1(MenuLayer::callBackHide, this)),
//				NULL);
//			auto seq2 = Sequence::create(MoveTo::create(0.2, buttonMenu->getPosition()),
//				CallFuncN::create(CC_CALLBACK_1(MenuLayer::callBackHide, this)),
//				NULL);
			buttonConfigure->runAction(seq);
			buttonHelp->runAction(seq->clone());
			//buttonNitice->runAction(seq2);
			pImage->runAction(RotateBy::create(0.2, 90));
			m_bMoving = true;
		}
		else
		{
			buttonConfigure->setVisible(true);
			buttonHelp->setVisible(true);
			//buttonNitice->setVisible(true);

			auto seq = Sequence::create(MoveBy::create(0.2, Vec2(0.0f, 70)),
				MoveBy::create(0.1, Vec2(0.0f, -5)),
				CallFuncN::create(CC_CALLBACK_1(MenuLayer::callBackShowFinish, this)),
				NULL);
			auto seq1 = Sequence::create(MoveBy::create(0.2, Vec2(0.0f, 140)),
				MoveBy::create(0.1, Vec2(0.0f, -10)),
				CallFuncN::create(CC_CALLBACK_1(MenuLayer::callBackShowFinish, this)),
				NULL);
//			auto seq2 = Sequence::create(MoveBy::create(0.2, Vec2(0.0f, 210)),
//				MoveBy::create(0.1, Vec2(0.0f, -15)),
//				CallFuncN::create(CC_CALLBACK_1(MenuLayer::callBackShowFinish, this)),
//				NULL);
			buttonConfigure->runAction(seq);
			buttonHelp->runAction(seq1);
			//buttonNitice->runAction(seq2);
			pImage->runAction(RotateBy::create(0.2, -90));

			m_bOpening = true;
			m_bMoving = true;
		}
	}
}
//设置
void MenuLayer::callBackConfigure1(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		//加载音效
		MusicData::getInstance()->playSound(TOP);
		m_pPublicLayer->showConfigureLayer();
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng::MobClickCpp::event(getStringFromInt(YM_CONFIGURE).c_str());
#endif
	}
}
//帮助
void MenuLayer::callBackHelp(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		//加载音效
		MusicData::getInstance()->playSound(TOP);
		m_pPublicLayer->showHelpLayer();
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng::MobClickCpp::event(getStringFromInt(YM_HELP).c_str());
#endif
	}
}
//客服
void MenuLayer::callBackCustomerService(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		//m_pPublicLayer->requestRecharge1();
		m_pPublicLayer->showServiceLayer();
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng::MobClickCpp::event(getStringFromInt(YM_SERVICE).c_str());
#endif
	}
}
//兑换金币
void MenuLayer::callBackExchangeGold(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		m_pPublicLayer->showExchangeGoldLayer();
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng::MobClickCpp::event(getStringFromInt(YM_EXCHANGE_GOLD).c_str());
#endif
	}
}
//单人模式
void MenuLayer::callBackSingle(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		if(m_nCurrentPage == MENU_SINGLE)
		{
			return;
		}

#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
		//umeng::MobClickCpp::event(getStringFromInt(YM_SINGLE).c_str());
#endif

        //加载音乐
		MusicData::getInstance()->playSound(TOP);

		this->clickSingleGame();
	}
}
//多人模式
void MenuLayer::callBackMany(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		if(m_nCurrentPage == MENU_MANY)
		{
			return;
		}
		MusicData::getInstance()->playSound(TOP);

#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng::MobClickCpp::event(getStringFromInt(YM_FOUR_PLAYER).c_str());
#endif
//#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
//		m_pPublicLayer->checkNet(101);
//#else
//		this->clickManyGame();
//#endif
		this->clickManyGame();
		//加载音效
		/*MusicData::getInstance()->playSound(BOTOOM);
		m_nCurrentPage = MENU_MANY;
		showMany();*/
	}
}
//设置按钮
void MenuLayer::callBackConfigure(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		//加载音效
		MusicData::getInstance()->playSound(BOTOOM);
	}
}
//商店按钮
void MenuLayer::callBackShop(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		//ActionManagerEx::getInstance()->playActionByName("ui/main/NewUi_1.json" , "Animation0");
		MyButton* pButton = (MyButton*)pObject;
		//加载音效
		MusicData::getInstance()->playSound(TOP);
		if(pButton->getTag() == 868)
		{
			m_pPublicLayer->showDiamondShopLayer();
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	// umeng::MobClickCpp::event(getStringFromInt(YM_RECHARGE_DIAMOND).c_str());
#endif
		}
		else
		{
			m_pPublicLayer->showShopLayer();
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	// umeng::MobClickCpp::event(getStringFromInt(YM_RECHARGE_GOLD).c_str());
#endif
		}
	}
}
//免费领取
void MenuLayer::callBackFree(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		//加载音效
		MusicData::getInstance()->playSound(TOP);
		m_pPublicLayer->showFreeGoldLayer();

#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng::MobClickCpp::event(getStringFromInt(YM_FREEGOLD).c_str());
#endif
	}
}
//话费兑换
void MenuLayer::callBackExchange(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		//加载音效
		MusicData::getInstance()->playSound(TOP);
		m_pPublicLayer->showExchangeLayer();
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng::MobClickCpp::event(getStringFromInt(YM_EXCHANGE_TELEPHONEFARE).c_str());
#endif
	}
}
//特惠礼包
void MenuLayer::callBackGratiaPackage(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		//加载音效
		MusicData::getInstance()->playSound(TOP);
		m_pPublicLayer->showGiftLayer();
	}
}
//领取月卡
void MenuLayer::callBackMonthCard(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		//加载音效
		MusicData::getInstance()->playSound(TOP);
		m_pPublicLayer->showMonthCardLayer();
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng::MobClickCpp::event(getStringFromInt(YM_MONTCH_CARD).c_str());
#endif
	}
}
//VIP特权
void MenuLayer::callBackVIP(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		//加载音效
		MusicData::getInstance()->playSound(TOP);
		m_pPublicLayer->showVIPLayer();
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng::MobClickCpp::event(getStringFromInt(YM_VIP).c_str());
#endif
	}
}
//游戏商城
void MenuLayer::callBackGameShop(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		//加载音效
		MusicData::getInstance()->playSound(TOP);
	}
}
//点击单人关卡
void MenuLayer::callBackClickMission(Ref* pObject, ui::Widget::TouchEventType ty)
{
    
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
        //添加音乐
        MusicData::getInstance()->playSound(28);
		ui::MyButton* buttonMission = (ui::MyButton*)pObject;
		int nTag = buttonMission->getTag();
        
        instance->setOne_LevelNum(nTag - 69); //记录进入第一房间的关卡号
        
		if(nTag >= m_nMaxUnlockMission + 70)
		{
			return;
		}

#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng::MobClickCpp::event(getStringFromInt(YM_SMALL_GAME + nTag - 69).c_str());
#endif

		if(!m_bShowingTV && !m_bMoving)
		{
			addTVShiled();
			m_nCurSelectedMission = nTag - 70;
			showTV(m_nCurSelectedMission);		
		}
		else if(m_bShowingTV && !m_bMoving)
		{
			stopTV();
		}
	}
}
//点击多人关卡
void MenuLayer::callBackClickMissionMany(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		//添加音乐
		MusicData::getInstance()->playSound(28);
		ui::MyButton* buttonMission = (ui::MyButton*)pObject;
		
		m_nCurrentButtonTag = buttonMission->getTag();

		this->clickManyMission();
//#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
//		m_pPublicLayer->checkNet();
//#else
//		this->clickManyMission();
//#endif
	}
}
//解锁跑
void MenuLayer::callBackLock(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		m_pPublicLayer->showSwitchGunLayer();
	}
}
//当前关卡
void MenuLayer::callBackCurMission(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		
	}
}
//返回按钮
void MenuLayer::callBackReturn(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		MusicData::getInstance()->playSound(TOP);
		ui::MyButton* pButton = (ui::MyButton*)pObject;
		if(pButton->getTag() == 28)
		{
			if(m_bShowingTV && !m_bMoving)
			{
				stopTV();
			}
			else
			{
				Layout* pPannel = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 743);
				auto seq = Sequence::createWithTwoActions(MoveBy::create(0.3, Vec2(size.width + 40, 0)), MoveBy::create(0.1, Vec2(-40, 0)));
				pPannel->runAction(seq);

				//隐藏单机选择界面
				Layout* pPannel1 = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 26);
				pPannel1->setVisible(false);
			}
		}
		else if(pButton->getTag() == 140)
		{
			Layout* pPannel = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 743);
			auto seq = Sequence::createWithTwoActions(MoveBy::create(0.3, Vec2(-size.width - 40, 0)), MoveBy::create(0.1, Vec2(+40, 0)));
			pPannel->runAction(seq);

			//隐藏单机选择界面
			Layout* pPannel1 = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 26);
			pPannel1->setVisible(false);
		}
		m_nCurrentPage = MENU_MAIN;
	}
}
//开始游戏的按钮
void MenuLayer::callBackBegin(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		//MusicData::getInstance()->playSound(START);
        //fishtv->removeSelf();
		SceneReader::getInstance()->destroyInstance();
		GUIReader::getInstance()->purgeGUIReader();        
		SpriteFrameCache::getInstance()->destroyInstance();//清理缓存
        TextureCache::getInstance()->destroyInstance();
        AnimationCache::getInstance()->destroyInstance();
        
        
        Manager::getInstance()->setEnterLoading(true);
		auto pScene = Scene::create();
		pScene->addChild(LoadingLayer::create());
		Director::getInstance()->replaceScene(pScene);

#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng::MobClickCpp::event(getStringFromInt(YM_SING_ENTER_GAME).c_str());
#endif
	}
}
//神秘游戏的按钮
void MenuLayer::callBackSmallGame(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		//music->playSound(46);
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
		//umeng::MobClickCpp::event(getStringFromInt(YM_SMALL_GAME).c_str());
#endif
		if(!Manager::getInstance()->getInSmallGame())
		{
			Manager::getInstance()->setInSmallGame(true);
			auto pScene = Scene::create();
			pScene->addChild(SmallGameLayer::create());

			Director::getInstance()->replaceScene(pScene);
		}
		
	}
}

//显示电视
void MenuLayer::showTV(int nMission)
{
	m_bShowingTV = true;
	m_bMoving = true;

	Layout* pList = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 26);

	auto seq1 = Sequence::createWithTwoActions(MoveBy::create(0.2, Vec2(0.0f, -530)), MoveBy::create(0.08f, Vec2(0.0f, 50)));

	//ImageView* pUp = (ui::ImageView *)ui::Helper::seekWidgetByTag(m_pUINode, 63);
	//pUp->runAction(seq1);
	Layout* pUp = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 953);
	pUp->runAction(seq1);

	//电视机的背景
	TextAtlas* pMission = (ui::TextAtlas *)ui::Helper::seekWidgetByTag(m_pUINode, 67);
	if(Manager::getInstance()->getInternet())
		pMission->setString(getStringFromInt(Manager::getInstance()->getPoolGoldNum()).c_str());
	else
	{
		pMission->setString("0");
	}

	auto seq = Sequence::create(DelayTime::create(0.2f), 
		CallFuncN::create(CC_CALLBACK_1(MenuLayer::callBackShowFinish, this)),
		NULL);
	this->runAction(seq);
}
//电视消失
void MenuLayer::stopTV()
{
	m_bMoving = true;

	Layout* pList = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 26);

	/*ImageView* pUp = (ui::ImageView *)ui::Helper::seekWidgetByTag(m_pUINode, 63);
	pUp->runAction(MoveBy::create(0.3f, Vec2(0.0f, 430)));*/
	Layout* pUp = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 953);
	pUp->runAction(MoveBy::create(0.3f, Vec2(0.0f, 480)));

	//电视机的背景
	auto seq = Sequence::create(DelayTime::create(0.3), 
		CallFuncN::create(CC_CALLBACK_1(MenuLayer::callBackStopFinish, this)),
		NULL);
	this->runAction(seq);
}
//出场完毕回调
void MenuLayer::callBackShowFinish(Node* pNode)
{
	m_bMoving = false;
}
//收回回调
void MenuLayer::callBackStopFinish(Node* pNode)
{
	m_bShowingTV = false;
	m_bMoving = false;
}
//更新关卡解锁信息
void MenuLayer::updateLock(int nMaxUnlock)
{
	int nLockTag = 102;
	int nIconTag = 92;
	for (int i = 0; i < 9; i++)
	{
		ImageView* pLock1 = (ui::ImageView *)ui::Helper::seekWidgetByTag(m_pUINode, i + nLockTag);
		ImageView* pIcon1 = (ui::ImageView *)ui::Helper::seekWidgetByTag(m_pUINode, i + nIconTag);
		if(i < nMaxUnlock-1)
		{
			pLock1->setVisible(false);
			pIcon1->setVisible(true);
		}
		else
		{
			pLock1->setVisible(true);
			pIcon1->setVisible(false);
		}
	}
	

}
//隐藏菜单按钮回调
void MenuLayer::callBackHide(Node* pNode)
{
	ui::MyButton* pButton = (ui::MyButton*)pNode;

	pButton->setVisible(false);

	m_bOpening = false;
	m_bMoving = false;
}
//添加电视遮罩
void MenuLayer::addTVShiled()
{
	return;
	Layout* pList = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 26);
	ImageView* pUp = (ui::ImageView *)ui::Helper::seekWidgetByTag(m_pUINode, 63);
	pList->removeChildByTag(1001);

	//电视机的背景
	int nStage = Manager::getInstance()->getOne_LevelNum();
	auto pTVBack = Sprite::create(__String::createWithFormat("bg_%d.png", nStage)->getCString());
	pTVBack->setScale(0.75);
	pTVBack->setPosition(Vec2(0, 0));
	
	auto pClippingNode = ClippingNode::create();
	pClippingNode->setAlphaThreshold(0.1f);
	pClippingNode->setPosition(pUp->getPosition());
	pClippingNode->addChild(pTVBack);
	pList->addChild(pClippingNode, 0, 1001);

	auto nodef = Node::create(); 
	auto close = Sprite::create("ui/main/tvMask_bg.png");
	close->setScale(0.8);
	nodef->addChild(close); 
	nodef->setPosition(Point(0, -35));
	pClippingNode->setStencil(nodef);

	//电视TV
	auto fishtv = FishTV::createFishTV();
	fishtv->setScale(0.5f);
	fishtv->setPosition(Vec2(-430, -240));
	pClippingNode->addChild(fishtv, 0);
}


void MenuLayer::callbackDeviceInfoFromJava(Node* target, void* data)
{

	log("get getDeviceInfo in c++ return");

	__Dictionary* dic = (__Dictionary*)data;

	int nResult = dic->valueForKey("result")->intValue();
	int nFakeCode = dic->valueForKey("fake_code")->intValue();
	int nMobile = dic->valueForKey("mobile")->intValue();
	std::string str = dic->valueForKey("mac")->getCString();
	std::string strVersion = dic->valueForKey("version")->getCString();
	
	log("c++ mac === %s", str.c_str());

	HttpInstance::getInstance()->setDeviceMac(str.c_str());
	HttpInstance::getInstance()->setMobileType(nMobile);
	HttpInstance::getInstance()->setCurVersion(strVersion);
}
