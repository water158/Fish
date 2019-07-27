#include "PublicLayer.h"
#include "MenuLayer.h"
#include "MainScene.h"
#include "LoadingLayer.h"
#include "MusicData.h"
#include "Store.h"
//#include "MobClickCpp.h"

#include "CCStdC.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "UserChannel.h"

using namespace  rapidjson;

PublicLayer::PublicLayer()
{
	m_pDelegateGame = NULL;
	m_pDelegateMenu = NULL;
    
	HttpInstance::getInstance()->setPublicLayer(this);
}

PublicLayer::~PublicLayer()
{
	CC_SAFE_DELETE(m_pXmlParser);
}

bool PublicLayer::init()
{
	Layer::init();
    
    pu_music = MusicData::getInstance();
	m_nSignedNum = UserData::getInstance()->getSignInNum();
	m_bOpening = false;
	m_bMoving = false;
	m_bOpenInGame = false;
	m_bReceived = false;
	m_bOpenOnUnenough = false;
	m_bOpenOnEnterMany = false;
	m_bOpenInHelp = false;
	m_bInGame = true;
	m_bDirectClose = false;
	HttpInstance::getInstance()->setCouldPay(true);
	HttpInstance::getInstance()->setPayType(PAY_TYPE_THRAN);
	m_nReceivedCoinIndex = UserData::getInstance()->getAwardIndex();
	m_nEffectIndex = 0;
	m_nCurTimeIndex = 0;
    m_nmainLevel = 0;
	m_nGoldEffectNum = 1;
	m_nCurrentRequestTimes = 0;
    m_fValidatingDelayTime = 0;
	size = Director::getInstance()->getWinSize();
	//ooo
	m_nFinishTaskIndex = getTaskIndex(Manager::getInstance()->getOne_LevelNum());
	m_nCrossCoinIndex = 0;
	m_nMessageIndex = 0;
	m_nGuideIndex = 0;

	m_pXmlParser = XMLParser::parseWithFile("ui/loading.xml");
	m_pXmlParser->retain();	//更新时间
	tm* pTime = GetTime1();
	int nMon = pTime->tm_mon + 1;
	int nDay = pTime->tm_mday;
	int nData = nMon*100 + nDay;

	//签到次数检测
	if(nData <= UserData::getInstance()->getSignInLastTime())
	{
		m_bReceived = true;
	}
	else
	{
		if(UserData::getInstance()->getSignInNum() == 7)
		{
			UserData::getInstance()->setSignInNum(0);
			UserData::getInstance()->save();
			m_nSignedNum = 0;
		}
	}

	int nLastRewardTime = UserData::getInstance()->getAwardLastTime();
	if(nLastRewardTime < nMon*100 + nDay)
	{
		m_nReceivedCoinIndex = -1;
	}
	//Director::getInstance()->getTextureCache()->addImageAsync("ui/public/NewUi0.png", CC_CALLBACK_1(PublicLayer::plistImageAsyncCallback, this));
	//Director::getInstance()->getTextureCache()->addImageAsync("ui/public/NewUi1.png", CC_CALLBACK_1(PublicLayer::plistImageAsyncCallback1, this));
	//UI的使用
	//m_pUINode = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui/public/NewUi_1.ExportJson");
	m_pUINode = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui/public/NewUi_1.json");
	m_pUINode->setPosition(Vec2(0, 0));
	this->addChild(m_pUINode, 1); 

	showMessageBarLayer();
	
	Layout* pGift = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 2596);
	pGift->setScale(0.0f);
	Layout* pShop = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 19);
	pShop->setScale(0.0f);

	//缓存 炮台
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("cannon_atlas.plist", "cannon_atlas.png");

	//Director::getInstance()->getTextureCache()->addImageAsync("cannon_atlas.png", CC_CALLBACK_1(PublicLayer::plistImageAsyncCallback2, this));
	//加载鱼的动画资源
	//this->loadFishAnimation();
	//添加鱼的动画到面板上
	//this->addFishRateAnimation();

	//自动弹出月卡每日领取界面
	int nBuyMonthCard = UserData::getInstance()->getBuyMonthCard();

	if(nBuyMonthCard == 1)
	{
		int nNumReceive = UserData::getInstance()->getNumOfReceivMonthCard();
		int nLastTime = UserData::getInstance()->getGetMonthCardLastTime();
		tm* pTime = GetTime1();
		int nYear = pTime->tm_year;
		int nMon = pTime->tm_mon + 1;
		int nDay = pTime->tm_mday;
		int nData = nYear*10000 + nMon*100 + nDay;

		if(nNumReceive < 30)
		{
			if(nLastTime < nData)
			{
				this->showMonthCardReceiveLayer();
				UserData::getInstance()->setNumOfReceivMonthCard(UserData::getInstance()->getNumOfReceivMonthCard() + 1);
				UserData::getInstance()->setGetMonthCardLastTime(nData);
			}
		}
		if(UserData::getInstance()->getNumOfReceivMonthCard() >= 30)
		{
			UserData::getInstance()->setBuyMonthCard(0);
			UserData::getInstance()->setNumOfReceivMonthCard(0);
			UserData::getInstance()->setGetMonthCardLastTime(0);
		}

		UserData::getInstance()->save();
	}

	//this->getDeviceInfo(1);

	this->schedule(schedule_selector(PublicLayer::updateRewardTime), 1.0);
	this->schedule(schedule_selector(PublicLayer::updateMessageBar), 1.0);
	if(m_bInGame)
		this->schedule(schedule_selector(PublicLayer::updateGuide), 1.0);
	this->schedule(schedule_selector(PublicLayer::updateRechargeResult), 1.0);
	
	return true;
}

//异步加载
void PublicLayer::plistImageAsyncCallback(cocos2d::Texture2D* texture)
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/public/NewUi0.plist", texture);
}
void PublicLayer::plistImageAsyncCallback1(cocos2d::Texture2D* texture)
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/public/NewUi1.plist", texture);
}
void PublicLayer::plistImageAsyncCallback2(cocos2d::Texture2D* texture)
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("cannon_atlas.plist", texture);
}

/********界面切换*******/
//显示免费金币界面
void PublicLayer::showFreeGoldLayer()
{
	setLayerVisible(LAYER_FREEGOLD);
	addShiledLayer();
	//签到
	ui::MyButton * buttonSign = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 16);
	buttonSign->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackSign, this));
	buttonSign->setClickPicture();
	//上线活动
	ui::MyButton * buttonOnline = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 17);
	buttonOnline->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackOnline, this));
	buttonOnline->setClickPicture();
	//关闭
	ui::MyButton * buttonClose = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 18);
	buttonClose->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackCommonClose, this));
	buttonClose->setClickPicture();
}
//显示商店界面
void PublicLayer::showShopLayer()
{
	setLayerVisible(LAYER_SHOP);
	addShiledLayer();
	//关闭
	ui::MyButton * buttonClose = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 26);
	buttonClose->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackCommonClose, this));
	buttonClose->setClickPicture();

	//购买
	ui::MyButton * buttonBuy1 = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 34);
	buttonBuy1->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackBuy, this));
	buttonBuy1->setClickPicture();
	//购买
	ui::MyButton * buttonBuy2 = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 65);
	buttonBuy2->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackBuy, this));
	buttonBuy2->setClickPicture();
	//购买
	ui::MyButton * buttonBuy3 = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 73);
	buttonBuy3->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackBuy, this));
	buttonBuy3->setClickPicture();
	//购买
	ui::MyButton * buttonBuy4 = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 81);
	buttonBuy4->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackBuy, this));
	buttonBuy4->setClickPicture();
	//购买
	ui::MyButton * buttonBuy5 = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 89);
	buttonBuy5->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackBuy, this));
	buttonBuy5->setClickPicture();
	//购买
	ui::MyButton * buttonBuy6 = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 15323);
	buttonBuy6->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackBuy, this));
	buttonBuy6->setClickPicture();
	//购买
	ui::MyButton * buttonBuy7 = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 14810);
	buttonBuy7->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackBuy, this));
	buttonBuy7->setClickPicture();
	

	bool bMMPay = Manager::getInstance()->getMMPay();
	//商品列表
	ui::ScrollView * pScrollView = (ui::ScrollView *)ui::Helper::seekWidgetByTag(m_pUINode, 32);
	Layout* pLayer = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 33);
	/*if(bMMPay)
	{
		int nWidth = 370;
		pScrollView->setInnerContainerSize(Size(960 + nWidth, 200));
		pLayer->runAction(MoveTo::create(0.0f, Vec2(nWidth, -11)));
	}
	else
	{*/
		pScrollView->setInnerContainerSize(Size(1310, 200));
		pLayer->runAction(MoveTo::create(0.0f, Vec2(0, 0)));
	//}


	//首冲双倍的图片
	int bFirst = UserData::getInstance()->getFirstShopping();
	ui::ImageView* pFirst = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 2609);
	if(bFirst == 1)
	{
		pFirst->setVisible(false);
	}

	//额外VIP金币奖励
	int nCurVIP = UserData::getInstance()->getCurVipLevel();
	int nPre = 1;
	int nNext = 0;

	if(nCurVIP == 1)
		nNext = 1;
	else if(nCurVIP == 2)
		nNext = 2;
	else if(nCurVIP >= 3)
	{
		nNext = 3;
	}
	ui::TextAtlas* pText1 = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, 5334);
	pText1->setString(getStringFromInt(nCurVIP));
	ui::TextAtlas* pText2 = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, 5338);
	pText2->setString(getStringFromInt(nPre));
	ui::TextAtlas* pText3 = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, 5339);
	pText3->setString(getStringFromInt(nNext));
}
//显示钻石商店界面
void PublicLayer::showDiamondShopLayer()
{
	setLayerVisible(LAYER_SHOP_DIAMOND);
	addShiledLayer();
	//关闭
	ui::MyButton * buttonClose = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 4724);
	buttonClose->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackCommonClose, this));
	buttonClose->setClickPicture();

	//购买
	ui::MyButton * buttonBuy1 = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 4727);
	buttonBuy1->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackBuy, this));
	buttonBuy1->setClickPicture();
	//购买
	ui::MyButton * buttonBuy2 = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 4735);
	buttonBuy2->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackBuy, this));
	buttonBuy2->setClickPicture();
	//购买
	ui::MyButton * buttonBuy3 = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 4752);
	buttonBuy3->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackBuy, this));
	buttonBuy3->setClickPicture();
	//购买
	ui::MyButton * buttonBuy4 = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 4743);
	buttonBuy4->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackBuy, this));
	buttonBuy4->setClickPicture();
	//购买
	ui::MyButton * buttonBuy5 = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 4760);
	buttonBuy5->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackBuy, this));
	buttonBuy5->setClickPicture();
	//购买
	ui::MyButton * buttonBuy6 = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 15335);
	buttonBuy6->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackBuy, this));
	buttonBuy6->setClickPicture();

	bool bMMPay = Manager::getInstance()->getMMPay();
	//商品列表
	ui::ScrollView * pScrollView = (ui::ScrollView *)ui::Helper::seekWidgetByTag(m_pUINode, 4725);
	Layout* pLayer = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 4726);
	if(bMMPay)
	{
		int nWidth = 370;
		pScrollView->setInnerContainerSize(Size(960 + nWidth, 200));
		pLayer->runAction(MoveTo::create(0.0f, Vec2(nWidth, 0)));
	}
	else
	{
		pScrollView->setInnerContainerSize(Size(1110, 200));
		pLayer->runAction(MoveTo::create(0.0f, Vec2(0, 0)));
	}

	//额外VIP金币奖励
	int nCurVIP = UserData::getInstance()->getCurVipLevel();
	int nPre = 1;
	int nNext = 0;

	if(nCurVIP == 1)
		nNext = 1;
	else if(nCurVIP == 2)
		nNext = 2;
	else if(nCurVIP >= 3)
	{
		nNext = 3;
	}
	ui::TextAtlas* pText1 = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, 5347);
	pText1->setString(getStringFromInt(nCurVIP));
	ui::TextAtlas* pText2 = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, 5348);
	pText2->setString(getStringFromInt(nPre));
	ui::TextAtlas* pText3 = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, 5349);
	pText3->setString(getStringFromInt(nNext));
}
//显示解锁界面
//显示更换炮台界面
void PublicLayer::showSwitchGunLayer()
{
	setLayerVisible(LAYER_SWITCHGUN);
	addShiledLayer();
	ui::ScrollView * pScrollView = (ui::ScrollView *)ui::Helper::seekWidgetByTag(m_pUINode, 161);
	pScrollView->setInnerContainerSize(Size(780, 200));

	//关闭
	ui::MyButton * buttonClose = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 345);
	buttonClose->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackCommonClose, this));
	buttonClose->setClickPicture();

	int nBuyTag = 163;
	int nLockTag = 260;
	int nEquipTag[4] = {1234, 1240, 1243, 1246};
	bool bLock = false;
	int nSelect = UserData::getInstance()->getCannonID();
	if(nSelect > 0)
		nSelect -= 1;
	for (int i = 0; i < 4; i++)
	{
		if(i == 0)
			bLock = UserData::getInstance()->getCannon1If();
		else if(i == 1)
			bLock = UserData::getInstance()->getCannon2If();
		else if(i == 2)
			bLock = UserData::getInstance()->getCannon3If();
		else if(i == 3)
			bLock = UserData::getInstance()->getCannon4If();
		
		//装备图片
		ui::ImageView* imageEquip = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, nEquipTag[i]);
		imageEquip->setVisible(false);

		//锁图片
		ui::ImageView* imageLock = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, nLockTag + i);
		imageLock->setVisible(true);

		//购买
		ui::MyButton * buttonChange = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, nBuyTag + i * 8);
		buttonChange->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackChangeGun, this));
		buttonChange->setClickPicture();

		//装备
		/*ui::MyButton * buttonEquip = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, nTag);
		buttonEquip->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackEquipGun, this));
		buttonEquip->setClickPicture();
		buttonEquip->setVisible(true);*/

		//是否解锁
		if(bLock)
		{
			imageLock->setVisible(false);
		}
		

		if(i == nSelect)
		{
			imageEquip->setVisible(true);
		}

		addIconForGun(buttonChange);	
	}
	//已装备
	
}
//显示签到界面
void PublicLayer::showSignLayer(bool bScale)
{
	setLayerVisible(LAYER_SINGN, bScale);
	addShiledLayer();
	//关闭
	ui::MyButton * buttonClose = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 346);
	buttonClose->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackCommonClose, this));
	buttonClose->setClickPicture();

	//领取按钮
	ui::MyButton * buttonReceive = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 344);
	buttonReceive->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackSignToday, this));
	buttonReceive->setClickPicture();

	//按钮显示处理
	int nSignedDays = m_nSignedNum;
	int nTimesTag[7] = {11911, 11929, 11938, 11947, 11956, 11965, 11974};
	int nSignedTag[7] = {12010, 12011, 12012, 12013, 12014, 12015, 12016};
	int nVip = UserData::getInstance()->getCurVipLevel();
	for (int i = 0; i < 7; i++)
	{
		ui::TextAtlas* pTimes = (ui::TextAtlas *)ui::Helper::seekWidgetByTag(m_pUINode, nTimesTag[i]);
		ui::ImageView* pSigned = (ui::ImageView *)ui::Helper::seekWidgetByTag(m_pUINode, nSignedTag[i]);
		
		if(nVip > 0)
			pTimes->setString("2");
		else
			pTimes->setString("1");

		if(i <= nSignedDays - 1)
		{
			pSigned->setVisible(true);
			if(i == nSignedDays - 1)
			{
				
			}
			else
			{
				
			}
		}
		else
		{
			pSigned->setVisible(false);
			if(i == nSignedDays)
			{
				
			}
			else
			{
				
			}
		}
	}

	//额外VIP金币奖励
	int nCurVIP = UserData::getInstance()->getCurVipLevel();
	int nPre = 1;
	int nNext = 0;

	if(nCurVIP == 1)
		nNext = 2;
	else if(nCurVIP == 2)
		nNext = 5;
	else if(nCurVIP >= 3)
	{
		nPre = 2;
		nNext = 0;
	}
	ui::TextAtlas* pText1 = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, 3472);
	pText1->setString(getStringFromInt(nCurVIP));
	ui::TextAtlas* pText2 = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, 4818);
	pText2->setString(getStringFromInt(nPre));
	ui::TextAtlas* pText3 = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, 4819);
	pText3->setString(getStringFromInt(nNext));
}
//显示领取金币界面
void PublicLayer::showCollectCoinsLayer(bool bScale)
{
	setLayerVisible(LAYER_AWARD, bScale);
	addShiledLayer();
	bool bEnagle = false;
	//领取
	ui::MyButton * buttonReceive = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 564);
	buttonReceive->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackCollectCoins, this));
	buttonReceive->setClickPicture();
	int nCurIndex = getIndexByCurTime();
	if(m_nReceivedCoinIndex < nCurIndex)
	{
		buttonReceive->setVisible(true);
		bEnagle = true;
	}
	else
	{
		buttonReceive->setVisible(false);
	}

	//关闭
	ui::MyButton * buttonClose = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 562);
	buttonClose->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackCommonClose, this));
	buttonClose->setClickPicture();

	int nButtonTag = 565;
	int nNodeTag = 517;

	for (int i = 0; i < 12; i++)
	{
		ui::MyButton * buttonCollect = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, nButtonTag + i);
		//buttonCollect->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackCollectCoins, this));
		//buttonCollect->setClickPicture();
		buttonCollect->setVisible(false);
	
		ImageView* pImage = (ui::ImageView *)ui::Helper::seekWidgetByTag(m_pUINode, 524 + 2*i);

		//添加图片
		if(!pImage->getChildByTag(11))
		{
			std::string str = "ui/public/mrlqjb1.png";
			if(i > 1 && i <= 5)
				str = "ui/public/mrlqjb2.png";
			else if(i > 5 && i <= 7)
				str = "ui/public/mrlqjb3.png";
			else if(i > 7 && i <= 9)
				str = "ui/public/mrlqjb4.png";
			auto pSprite = Sprite::create(str);
			pSprite->setPosition(Vec2(pImage->getContentSize().width / 2.0f, pImage->getContentSize().height / 2.0f));
			pImage->addChild(pSprite, 0, 11);
		}

		if(!bEnagle)
		{
			if(i <= m_nReceivedCoinIndex)
			{
				auto pSprite = pImage->getChildByTag(11);
				if(pSprite)
					this->graySprite(pSprite);
			}
		}
		else
		{
			if(i < nCurIndex)
			{
				auto pSprite = pImage->getChildByTag(11);
				if(pSprite)
					this->graySprite(pSprite);
			}
		}
	}

	//额外VIP金币奖励
	int nCurVIP = UserData::getInstance()->getCurVipLevel();
	int nPre = 1;
	int nNext = 0;

	if(nCurVIP == 1)
		nNext = 2;
	else if(nCurVIP == 2)
		nNext = 5;
	else if(nCurVIP >= 3)
	{
		nPre = 2;
		nNext = 0;
	}
	ui::TextAtlas* pText1 = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, 3473);
	pText1->setString(getStringFromInt(nCurVIP));
	ui::TextAtlas* pText2 = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, 4823);
	pText2->setString(getStringFromInt(nPre));
	ui::TextAtlas* pText3 = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, 4824);
	pText3->setString(getStringFromInt(nNext));
}
//显示话费兑换界面
void PublicLayer::showExchangeLayer()
{
	setLayerVisible(LAYER_EXCHANGE);
	addShiledLayer();
	//关闭
	ui::MyButton* buttonClose = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 2315);
	buttonClose->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackCommonClose, this));
	buttonClose->setClickPicture();

	//兑换
	ui::MyButton* buttonExchange = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 2374);
	buttonExchange->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackExchange, this));
	buttonExchange->setClickPicture();

	//刷新话费数量
	int nFare = UserData::getInstance()->getCurrentTelephoneFare();
	int nPre = nFare / 10;
	int nLast = nFare % 10;
	std::string strName = __String::createWithFormat("%d.%d", nPre, nLast)->getCString();

	ui::Text* pTextFare = (ui::Text *)ui::Helper::seekWidgetByTag(m_pUINode, 2376);
	pTextFare->setString(strName);
}
//显示设置界面
void PublicLayer::showConfigureLayer()
{
	setLayerVisible(LAYER_CONFIGURE);
	addShiledLayer();
	Layout* pConfigure = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 577);

	//关闭
	ui::MyButton * buttonClose = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 579);
	buttonClose->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackCommonClose, this));
	buttonClose->setClickPicture();

	int nButtonTag = 622;
	for (int i = 0; i < 3; i++)
	{
		ui::MyButton * buttonConfigure = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, nButtonTag + i);
		buttonConfigure->setVisible(false);
	}

	bool nMusic = UserData::getInstance()->getMusic();
	bool bSound = UserData::getInstance()->getSound();
	bool bSmooth = UserData::getInstance()->getSmooth();
	for (int i=0; i < 3; i++)
	{
		auto plabel = Label::createWithSystemFont("On", "Arial-BoldMT", 16);
		plabel->setVisible(false);
		auto plabel1 = Label::createWithSystemFont("On", "Arial-BoldMT", 16);
		plabel1->setVisible(false);
		ControlSwitch* control = ControlSwitch::create(
			Sprite::create("ui/public/switch-mask.png"),
			Sprite::create("ui/public/switch-on.png"),
			Sprite::create("ui/public/switch-off.png"),
			Sprite::create("ui/public/switch-thumb.png"),
			plabel,
			plabel1);
		control->setScale(0.75);
        control->setTag(i);
		control->addTargetWithActionForControlEvents(this, cccontrol_selector(PublicLayer::valueChanged), Control::EventType::VALUE_CHANGED);

		if(i == 0)
			control->setOn(nMusic);
		else if(i == 1)
			control->setOn(bSound);
		else 
			control->setOn(bSmooth);

		ui::MyButton * buttonConfigure1 = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, nButtonTag + i);
		control->setPosition(buttonConfigure1->getPosition());
		pConfigure->addChild(control);
	}

}
//显示游戏界面
void PublicLayer::showGameLayer()
{
	hideLayer();
	Layout* pGame = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 691);
	pGame->setVisible(true);
	//pGame->setTouchEnabled(true);

	//五个技能
	ui::MyButton * buttonStopFish = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 694);
	buttonStopFish->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackStopFish, this));
	buttonStopFish->setClickPicture();
	ui::MyButton * buttonSpeedUp = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 695);
	buttonSpeedUp->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackSpeedUp, this));
	buttonSpeedUp->setClickPicture();
	ui::MyButton * buttonRage = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 696);
	buttonRage->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackRage, this));
	buttonRage->setClickPicture();
	ui::MyButton * buttonLockFish = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 697);
	buttonLockFish->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackLockFish, this));
	buttonLockFish->setClickPicture();
	ui::MyButton * buttonTenThousand = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 698);
	buttonTenThousand->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackTenThousand, this));
	buttonTenThousand->setClickPicture();

	//领取金币
	ui::MyButton * buttonReceiveCoin = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 693);
	buttonReceiveCoin->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackReceiveCoins, this));
	buttonReceiveCoin->setClickPicture();
	//返回
	ui::MyButton * buttonReturn = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 699);
	buttonReturn->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackReturn, this));
	buttonReturn->setClickPicture();
	buttonReturn->setVisible(false);

	ui::MyButton * buttonAddCoins = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 702);
	buttonAddCoins->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackAddGold, this));
	buttonAddCoins->setClickPicture();

	ui::MyButton * buttonShop = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 705);
	buttonShop->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackShop, this));
	buttonShop->setClickPicture();

	ui::MyButton * buttonSwitchGun = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 704);
	buttonSwitchGun->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackSwitchGun, this));
	buttonSwitchGun->setClickPicture();

	ui::MyButton * buttonMenu = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 706);
	buttonMenu->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackMenu, this));
	buttonMenu->setClickPicture();

	ui::MyButton * buttonHelp = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 708);
	buttonHelp->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackHelp, this));
	buttonHelp->setClickPicture();
	buttonHelp->setVisible(false);

	ui::MyButton * buttonConfigure = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 707);
	buttonConfigure->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackTenConfigure, this));
	buttonConfigure->setClickPicture();
	buttonConfigure->setVisible(false);

	ui::MyButton * buttonVIP = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 4280);
	buttonVIP->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackShowVIP, this));
	buttonVIP->setClickPicture();

	this->refreshCurrentVIP();

	//更新金币
	int nCoins = UserData::getInstance()->getGoldNum();
	ui::Text* pTextCoins = (ui::Text *)ui::Helper::seekWidgetByTag(m_pUINode, 1525);
	pTextCoins->setString(getStringFromInt(nCoins));

	ui::Text* pPoolGold = (ui::Text *)ui::Helper::seekWidgetByTag(m_pUINode, 5837);
	pPoolGold->setString("0");

    //更新经验
    this->refreshExperiencePoints(UserData::getInstance()->getExp_value());
    
	this->schedule(schedule_selector(PublicLayer::update), 1.5);

	ActionManagerEx::getInstance()->playActionByName("NewUi_1.json", "Animation1");
}
//双按钮确认框
void PublicLayer::showSelectLayer()
{
	//setLayerVisible(LAYER_POPTWO);
	Layout* pTwo = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 800);
	pTwo->setTouchEnabled(false);
	pTwo->setVisible(true);
	pTwo->setZOrder(300);
	addShiled2Layer();

	pTwo->setAnchorPoint(Vec2(0.5, 0.5));
	pTwo->setScale(0.1);
	auto seq = Sequence::createWithTwoActions(ScaleTo::create(0.2, 1.1), ScaleTo::create(0.1, 1.0));
	pTwo->runAction(seq);

	ui::MyButton* buttonTwo = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 802);
	buttonTwo->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackOk, this));
	buttonTwo->setClickPicture();
	ui::MyButton* buttonOne = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 803);
	buttonOne->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackCancel, this));
	buttonOne->setClickPicture();

	//XMLParser* xmlparser = XMLParser::parseWithFile("ui/loading.xml");
	//__String* info = xmlparser->getString("test");

	//ui::Text* pppp = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 801);
	//pppp->setString(info->getCString());


}
//确认按钮框
void PublicLayer::showSureLayer()
{
	Layout* pOne = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 804);
	pOne->setTouchEnabled(false);
	pOne->setVisible(true);
	pOne->setZOrder(300);
	addShiled2Layer();
    
	pOne->setAnchorPoint(Vec2(0.5, 0.5));
	pOne->setScale(0.1);
	auto seq = Sequence::createWithTwoActions(ScaleTo::create(0.2, 1.1), ScaleTo::create(0.1, 1.0));
	pOne->runAction(seq);
    
	ui::MyButton* buttonSure = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 806);
	buttonSure->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackOk, this));
	buttonSure->setClickPicture();
    
	__String* info = m_pXmlParser->getString("buy_success");
	ui::Text* pppp = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 805);
	pppp->setString(info->getCString());
}
//显示捕获话费条
void PublicLayer::showMessageBarLayer()
{
	Layout* pMessageBar = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 935);
	pMessageBar->setTouchEnabled(false);
	pMessageBar->setVisible(true);

	int nID = CCRANDOM_0_1()*4 + 1;
	__String* info = m_pXmlParser->getString(__String::createWithFormat("message_%d", nID)->getCString());
	ui::Text* pText = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 945);
	pText->runAction(MoveTo::create(0, Vec2(600, 23)));
	pText->setString(info->getCString());

	auto seq = Sequence::create(MoveBy::create(12, Vec2(-600 - pText->getContentSize().width, 0)), 
		CallFuncN::create(CC_CALLBACK_1(PublicLayer::callBackHideLayerOut, this)),
		NULL);
	pText->runAction(seq);
}
//显示金币不足的界面
void PublicLayer::showCoinNotEnoughLayer()
{
	Layout* pLayer = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1218);
	pLayer->setVisible(true);
	pLayer->setZOrder(200);
	pLayer->setAnchorPoint(Vec2(0.5, 0.5));
	addShiled1Layer();

	pLayer->setScale(0.1);
	auto seq = Sequence::createWithTwoActions(ScaleTo::create(0.2, 1.1), ScaleTo::create(0.1, 1.0));
	pLayer->runAction(seq);
	//关闭
	ui::MyButton * buttonClose = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 1220);
	buttonClose->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackCommonClose1, this));
	buttonClose->setClickPicture();

	ui::MyButton * buttonLeft = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 1231);
	buttonLeft->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackOk, this));
	buttonLeft->setClickPicture();
	ui::MyButton * buttonRight = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 1232);
	buttonRight->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackCancel, this));
	buttonRight->setClickPicture();
}
//显示任务界面
void PublicLayer::showTaskLayer()
{
	setLayerVisible(LAYER_TASK);
	//addShiledLayer();
	//Layout* pTaskBack = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1471);

	refreshTaskNum();

	//关闭
	ui::MyButton * buttonClose = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 1482);
	buttonClose->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackCommonClose, this));
	buttonClose->setClickPicture();

	//黄底动画
	ui::ImageView* pBack1 = (ui::ImageView *)ui::Helper::seekWidgetByTag(m_pUINode, 1488);
	pBack1->setVisible(false);
	ui::ImageView* pBack2 = (ui::ImageView *)ui::Helper::seekWidgetByTag(m_pUINode, 1489);
	pBack2->setVisible(false);

	//已完成
	ui::ImageView* pFinish1 = (ui::ImageView *)ui::Helper::seekWidgetByTag(m_pUINode, 1980);
	pFinish1->setVisible(false);
	ui::ImageView* pFinish2 = (ui::ImageView *)ui::Helper::seekWidgetByTag(m_pUINode, 1981);
	pFinish2->setVisible(false);	if(m_nFinishTaskIndex == 0)
	{
		pBack1->setVisible(true);
	}
	else if(m_nFinishTaskIndex == 1)
	{
		pBack2->setVisible(true);
		pFinish1->setVisible(true);
	}
	else
	{
		pBack1->setVisible(true);
		pBack2->setVisible(true);
		pFinish1->setVisible(true);
		pFinish2->setVisible(true);	
	}

	int nStage = Manager::getInstance()->getOne_LevelNum();
	int nGold = nStage*500+1000;

	ui::Text* text_coins = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 2414);
	text_coins->setString(getStringFromInt(nGold));
}
//显示左边的任务界面
void PublicLayer::showLeftTaskLayer()
{
	Layout* pTaskLeft = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1503);
	pTaskLeft->setVisible(true);
	
	refreshTaskNum();

	//问号按钮
	ui::MyButton* buttonHelp = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 1504);
	buttonHelp->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackTaskHelp, this));
	buttonHelp->setClickPicture();

	//领取通关奖励
	ui::MyButton* buttonCross = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 1505);
	buttonCross->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackGetReward, this));
	buttonCross->setClickPicture();
	buttonCross->setVisible(false);

	//前往下一关
	ui::MyButton* buttonNextMission = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 1841);
	buttonNextMission->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackNext, this));
	buttonNextMission->setClickPicture();
	buttonNextMission->setVisible(false);

	//小任务栏
	ui::ImageView* pFrame1 = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 1490);
	pFrame1->setVisible(true);
	ui::ImageView* pFrame2 = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 1498);
	pFrame2->setVisible(true);
	ui::ImageView* pLock = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 1502);
	pLock->setVisible(true);

	int nStage = Manager::getInstance()->getOne_LevelNum();
	//左边的数量
	UserDataMission* pData = UserData::getInstance()->getUserDataMissionByID(nStage);
	bool bReceived = pData->getMissionSitate();

	if(m_nFinishTaskIndex == 1)
	{
		pFrame1->setVisible(false);
		pFrame2->runAction(MoveTo::create(0.01, pFrame1->getPosition()));

		pLock->setVisible(false);
	}
	else if(m_nFinishTaskIndex == 2)
	{
		pFrame1->setVisible(false);
		pFrame2->setVisible(false);

		buttonHelp->setVisible(false);

		if(bReceived)
			buttonNextMission->setVisible(true);
		else
			buttonCross->setVisible(true);
	}
}
//显示过关界面
void PublicLayer::showCrossLayer()
{
	addShiledLayer();

	Layout* pCross = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1506);
	pCross->setVisible(true);
	pCross->setScale(1.0);
	pCross->setZOrder(400);

	pCross->setScale(0.1);
	auto seq = Sequence::createWithTwoActions(ScaleTo::create(0.2, 1.1), ScaleTo::create(0.1, 1.0));
	pCross->runAction(seq);
    
    //保存过关关卡
    int nStage = Manager::getInstance()->getOne_LevelNum();
	if(nStage < 10)
	{
		auto pMission = UserData::getInstance()->getUserDataMissionByID(nStage + 1);
		pMission->setUnLock(1);
		//UserData::getInstance()->setUserDataMissionByID(nStage + 1, pMission);
		UserData::getInstance()->save();
	}
    
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng::MobClickCpp::finishLevel(getStringFromInt(nStage).c_str());
#endif
	//继续
	ui::MyButton * buttonGoon = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 1514);
	buttonGoon->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackCommonClose, this));
	buttonGoon->setClickPicture();
	buttonGoon->setVisible(false);

	//下一关
	ui::MyButton * buttonNext = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 1513);
	buttonNext->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackNext, this));
	buttonNext->setClickPicture();
	buttonNext->setVisible(false);

	//光线
	ImageView* pLight = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 1507);
	pLight->setScale(0.2);

	pLight->runAction(ScaleTo::create(1.0, 2.5));

	auto seq1 = Sequence::create(DelayTime::create(1.0), 
		CallFuncN::create(CC_CALLBACK_1(PublicLayer::callBackShowCoins, this)), 
		NULL);
	this->runAction(seq1);
}
//显示帮助界面
void PublicLayer::showHelpLayer()
{
	setLayerVisible(LAYER_HELP);
	addShiledLayer();
	//关闭
	ui::MyButton* buttonClose = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 1546);
	buttonClose->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackCommonClose, this));
	buttonClose->setClickPicture();

	//设置
	/*ui::MyButton* buttonConfigure = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 1543);
	buttonConfigure->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackTenConfigure, this));
	buttonConfigure->setClickPicture();*/

	//特殊玩法
	ui::MyButton* buttonSpecial = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 1544);
	buttonSpecial->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackSpecial, this));
	buttonSpecial->setClickPicture();

	//倍率
	ui::MyButton* buttonFishRate = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 1545);
	buttonFishRate->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackRate, this));
	buttonFishRate->setClickPicture();

	m_bOpenInHelp = true;
}
//显示特殊玩法界面
void PublicLayer::showSpecialPlayLayer()
{
	setLayerVisible(LAYER_SPECIAL);
	addShiledLayer();
	//关闭
	ui::MyButton * buttonClose = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 1550);
	buttonClose->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackCommonClose, this));
	buttonClose->setClickPicture();

	ui::ScrollView * pScrollView = (ui::ScrollView *)ui::Helper::seekWidgetByTag(m_pUINode, 1594);
	pScrollView->setInnerContainerSize(Size(1076, 250));

	//更换图标
	float fScale = 0.7;
	ui::ImageView* pIamge1 = (ui::ImageView *)ui::Helper::seekWidgetByTag(m_pUINode, 1701);
	ui::ImageView* pIamge1_1 = (ui::ImageView *)ui::Helper::seekWidgetByTag(m_pUINode, 1707);
	if(pIamge1_1->isVisible())
	{
		pIamge1_1->setVisible(false);
		auto sp1 = Sprite::createWithSpriteFrameName("task_icon_34.png");
		sp1->setPosition(pIamge1_1->getPosition());
		sp1->setScale(fScale*0.6);
		pIamge1->addChild(sp1);
	}
	ui::ImageView* pIamge2 = (ui::ImageView *)ui::Helper::seekWidgetByTag(m_pUINode, 1719);
	ui::ImageView* pIamge2_1 = (ui::ImageView *)ui::Helper::seekWidgetByTag(m_pUINode, 1726);
	if(pIamge2_1->isVisible())
	{
		pIamge2_1->setVisible(false);
		auto sp2 = Sprite::createWithSpriteFrameName("task_icon_39.png");
		sp2->setPosition(pIamge2_1->getPosition());
		sp2->setScale(fScale);
		pIamge2->addChild(sp2);
	}
	ui::ImageView* pIamge3 = (ui::ImageView *)ui::Helper::seekWidgetByTag(m_pUINode, 1727);
	ui::ImageView* pIamge3_1 = (ui::ImageView *)ui::Helper::seekWidgetByTag(m_pUINode, 1734);
	if(pIamge3_1->isVisible())
	{
		pIamge3_1->setVisible(false);
		auto sp3 = Sprite::createWithSpriteFrameName("task_icon_7.png");
		sp3->setPosition(pIamge3_1->getPosition());
		sp3->setScale(fScale);
		pIamge3->addChild(sp3);
	}
	ui::ImageView* pIamge4 = (ui::ImageView *)ui::Helper::seekWidgetByTag(m_pUINode, 1735);
	ui::ImageView* pIamge4_1 = (ui::ImageView *)ui::Helper::seekWidgetByTag(m_pUINode, 1742);
	if(pIamge4_1->isVisible())
	{
		pIamge4_1->setVisible(false);
		auto sp4 = Sprite::createWithSpriteFrameName("task_icon_5.png");
		sp4->setPosition(pIamge4_1->getPosition());
		sp4->setScale(fScale);
		pIamge4->addChild(sp4);
	}
	ui::ImageView* pIamge5 = (ui::ImageView *)ui::Helper::seekWidgetByTag(m_pUINode, 1743);
	ui::ImageView* pIamge5_1 = (ui::ImageView *)ui::Helper::seekWidgetByTag(m_pUINode, 1750);
	if(pIamge5_1->isVisible())
	{
		pIamge5_1->setVisible(false);
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("task_anim_5_3.plist");
		auto sp5 = Sprite::createWithSpriteFrameName("task_anim_5_3_01.png");
		sp5->setPosition(pIamge5_1->getPosition());
		sp5->setScale(fScale);
		pIamge5->addChild(sp5);
	}
	ui::ImageView* pIamge6 = (ui::ImageView *)ui::Helper::seekWidgetByTag(m_pUINode, 1772);
	ui::ImageView* pIamge6_1 = (ui::ImageView *)ui::Helper::seekWidgetByTag(m_pUINode, 1779);
	if(pIamge6_1->isVisible())
	{
		pIamge6_1->setVisible(false);
		auto sp6 = Sprite::createWithSpriteFrameName("task_icon_7.png");
		sp6->setPosition(pIamge6_1->getPosition());
		sp6->setScale(fScale);
		pIamge6->addChild(sp6);
	}

}
//显示倍率界面
void PublicLayer::showFishRateLayer()
{
	setLayerVisible(LAYER_FISHRATE);
	addShiledLayer();

	//加载鱼的动画资源
	this->loadFishAnimation();
	//添加鱼的动画到面板上
	this->addFishRateAnimation();

	//关闭
	ui::MyButton * buttonClose = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 1763);
	buttonClose->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackCommonClose, this));
	buttonClose->setClickPicture();

	ui::ScrollView * pScrollView = (ui::ScrollView *)ui::Helper::seekWidgetByTag(m_pUINode, 1780);
	pScrollView->setInnerContainerSize(Size(580, 810));
}
//显示多人游戏界面
void PublicLayer::showManyGameLayer()
{
	hideLayer();
	Layout* pGame = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1526);
	pGame->setVisible(true);
	//pGame->setTouchEnabled(true);

	//两个技能
	ui::MyButton * buttonStopFish = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 1977);
	buttonStopFish->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackStopFish, this));
	buttonStopFish->setClickPicture();
	ui::MyButton * buttonLockFish = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 1978);
	buttonLockFish->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackLockFish, this));
	buttonLockFish->setClickPicture();	ui::MyButton * buttonShop = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 1527);
	buttonShop->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackShop, this));
	buttonShop->setClickPicture();
	ui::MyButton * buttonSpeedUp = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 2132);
	buttonSpeedUp->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackSpeedUp, this));
	buttonSpeedUp->setClickPicture();
	ui::MyButton * buttonSwitchGun = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 1537);
	buttonSwitchGun->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackSwitchGun, this));
	buttonSwitchGun->setClickPicture();

	ui::MyButton * buttonMenu = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 1534);
	buttonMenu->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackMenu, this));
	buttonMenu->setClickPicture();

	ui::MyButton* buttonHelp = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 1536);
	buttonHelp->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackHelp, this));
	buttonHelp->setClickPicture();

	ui::MyButton* buttonConfigure = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 707);
	buttonConfigure->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackTenConfigure, this));
	buttonConfigure->setClickPicture();
	buttonConfigure->setVisible(false);

	ui::MyButton* buttonVIP = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 4282);
	buttonVIP->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackShowVIP, this));
	buttonVIP->setClickPicture();

	//四人彩金进度
	ui::MyButton* buttonPool = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 6308);
	buttonPool->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackShowTaskNum, this));
	buttonPool->setClickPicture();

	//箭头
	/*ui::ImageView* pArrow = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 6682);
	pArrow->runAction(RotateBy::create(0.0f, 180.0f));*/

	//返回
	ui::MyButton * buttonReturn = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 1535);
	buttonReturn->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackReturn, this));
	buttonReturn->setClickPicture();
	
	ActionManagerEx::getInstance()->playActionByName("NewUi_1.json" , "Animation2");
}
//显示爆机界面
void PublicLayer::showBlastingMachineLayer()
{
	Layout* pOne = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 804);
	pOne->setTouchEnabled(false);
	pOne->setVisible(true);
	pOne->setZOrder(300);
	addShiled2Layer();

	pOne->setAnchorPoint(Vec2(0.5, 0.5));
	pOne->setScale(0.1);
	auto seq = Sequence::createWithTwoActions(ScaleTo::create(0.2, 1.1), ScaleTo::create(0.1, 1.0));
	pOne->runAction(seq);

	ui::MyButton* buttonSure = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 806);
	buttonSure->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackOk, this));
	buttonSure->setClickPicture();

	__String* info = m_pXmlParser->getString("blasting_machine");
	ui::Text* pppp = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 805);
	pppp->setString(info->getCString());
    if (m_bInGame) {
        m_nmainLevel = 1;
    }
    
}
//显示未下注提示
void PublicLayer::showNotBetLayer()
{
	Layout* pOne = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 804);
	pOne->setTouchEnabled(false);
	pOne->setVisible(true);
	pOne->setZOrder(300);
	addShiled2Layer();

	pOne->setAnchorPoint(Vec2(0.5, 0.5));
	pOne->setScale(0.1);
	auto seq = Sequence::createWithTwoActions(ScaleTo::create(0.2, 1.1), ScaleTo::create(0.1, 1.0));
	pOne->runAction(seq);

	ui::MyButton* buttonSure = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 806);
	buttonSure->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackOk, this));
	buttonSure->setClickPicture();

	__String* info = m_pXmlParser->getString("notbet");
	ui::Text* pppp = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 805);
	pppp->setString(info->getCString());
}
//显示升级界面
void PublicLayer::showUpgradeLayer()
{
	setLayerVisible(LAYER_UPGRADE);
	addShiledLayer();
	//关闭
	ui::MyButton* buttonClose = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 4793);
	buttonClose->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackCommonClose, this));
	buttonClose->setClickPicture();

	int nExp = UserData::getInstance()->getExp_value();
	int nLv = nExp/10000 + 1;

	//赠送物品数量
	//钻石
	int nAwardDiamonds = 50 * nLv;
	ui::Text* pDiamonds = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 4811);
	pDiamonds->setString(getStringFromInt(nAwardDiamonds));
	UserData::getInstance()->increaseDiamonds(nAwardDiamonds);

	//金币
	int nAwardGolds = 300 * nLv + 200;
	ui::Text* pGolds = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 4812);
	pGolds->setString(getStringFromInt(nAwardGolds));
	UserData::getInstance()->increaseGoldNum(nAwardGolds);

	//话费
	int nTelephoneFare = 1;
	int nCurTelephoneFare = UserData::getInstance()->getCurrentTelephoneFare();
	int nPre = nTelephoneFare / 10;
	int nLast = nTelephoneFare % 10;
	std::string strName = __String::createWithFormat("%d.%d", nPre, nLast)->getCString();
	ui::Text* pTelephoneFare = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 4813);
	pTelephoneFare->setString(strName);
	UserData::getInstance()->setCurrentTelephoneFare(nCurTelephoneFare + nTelephoneFare);
	UserData::getInstance()->save();
	
	m_pDelegateGame->refreshGoldAndDiamonds();
}
//显示话费不足界面
void PublicLayer::showFareNotEnoughLayer()
{
	Layout* pOne = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 804);
	pOne->setTouchEnabled(false);
	pOne->setVisible(true);
	pOne->setZOrder(300);
	addShiled2Layer();

	pOne->setAnchorPoint(Vec2(0.5, 0.5));
	pOne->setScale(0.1);
	auto seq = Sequence::createWithTwoActions(ScaleTo::create(0.2, 1.1), ScaleTo::create(0.1, 1.0));
	pOne->runAction(seq);

	ui::MyButton* buttonSure = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 806);
	buttonSure->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackOk, this));
	buttonSure->setClickPicture();

	__String* info = m_pXmlParser->getString("fare");
	ui::Text* pppp = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 805);
	pppp->setString(info->getCString());
}
//显示获得话费界面
void PublicLayer::showGetTelephoneFareLayer(int nFare, int nPlayerID)
{
	Layout* pOne = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 804);
	pOne->setTouchEnabled(false);
	pOne->setVisible(true);
	pOne->setZOrder(300);
	addShiled2Layer();

	pOne->setAnchorPoint(Vec2(0.5, 0.5));
	pOne->setScale(0.1);
	auto seq = Sequence::createWithTwoActions(ScaleTo::create(0.2, 1.1), ScaleTo::create(0.1, 1.0));
	pOne->runAction(seq);

	ui::MyButton* buttonSure = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 806);
	buttonSure->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackOk, this));
	buttonSure->setClickPicture();

	int nPre = nFare / 10;
	int nLast = nFare % 10;

	int nID = UserData::getInstance()->getPlayerID();

	__String* info = NULL;
	if(nID == nPlayerID)
	{
		info = m_pXmlParser->getString("fare_get");
		std::string strName = __String::createWithFormat(info->getCString(), nPre, nLast)->getCString();
		ui::Text* pppp = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 805);
		pppp->setString(strName);
	}
	else
	{
		info = m_pXmlParser->getString("fare_get1");
		std::string strName = __String::createWithFormat(info->getCString(), nPlayerID, nPre, nLast)->getCString();
		ui::Text* pppp = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 805);
		pppp->setString(strName);
	}
}
//显示礼包界面
void PublicLayer::showGiftLayer()
{
	int nBuyGift = UserData::getInstance()->getBuyGift();
	if(nBuyGift >= 2)
		return;
	setLayerVisible(LAYER_GIFT);
	addShiledLayer();
	//购买
	ui::MyButton* buttonBuy = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 2598);
	buttonBuy->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackBuy, this));
	buttonBuy->setClickPicture();
	//关闭
	ui::MyButton* buttonClose = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 2599);
	buttonClose->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackCommonClose, this));
	buttonClose->setClickPicture();

	////礼包显示
	//ui::ImageView* pImageGift_1 = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 2597);
	//ui::ImageView* pImageGift_2 = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 3079);
	//pImageGift_1->setVisible(false);
	//pImageGift_2->setVisible(false);
	//if(nBuyGift == 0)
	//{
	//	pImageGift_1->setVisible(true);
	//}
	//else 
	//{
	//	pImageGift_2->setVisible(true);
	//}
}
//显示月卡界面
void PublicLayer::showMonthCardLayer()
{
	setLayerVisible(LAYER_MONTHCARD);
	addShiledLayer();

	//购买
	ui::MyButton* buttonBuy = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 4103);
	buttonBuy->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackBuy, this));
	buttonBuy->setClickPicture();
	//关闭
	ui::MyButton* buttonClose = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 4102);
	buttonClose->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackCommonClose, this));
	buttonClose->setClickPicture();


	ui::Layout* pWords = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 4647);
	//是否购买了月卡
	int nBuyTime = UserData::getInstance()->getBuyMonthCard();
	//未购买
	if(nBuyTime == 0)
	{
		buttonBuy->setVisible(true);
		pWords->setVisible(false);
	}
	else
	{
		buttonBuy->setVisible(false);
		pWords->setVisible(true);
		ui::TextAtlas* pNum = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, 4642);

		int nReceiveTime = UserData::getInstance()->getNumOfReceivMonthCard();
		//剩余天数
		int nLeftTime = 30 - nReceiveTime;
		pNum->setString(getStringFromInt(nLeftTime));
	}
}
//显示月卡领取界面
void PublicLayer::showMonthCardReceiveLayer()
{
	setLayerVisible(LAYER_MONTHCARD_RECEIVE);
	addShiledLayer();

	//确认领取
	ui::MyButton* buttonClose = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 4662);
	buttonClose->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackCommonClose, this));
	buttonClose->setClickPicture();

	//赠送东西
	UserData::getInstance()->increaseDiamonds(300);
	UserData::getInstance()->increaseGoldNum(50000);
	UserData::getInstance()->increaseSkillNum(SKILL_2, 30);
	UserData::getInstance()->increaseSkillNum(SKILL_3, 10);
	UserData::getInstance()->increaseSkillNum(SKILL_4, 10);
	UserData::getInstance()->save();
}
//公共确认框
void PublicLayer::showPublicSureLayer(const char* pWords, int nNum)
{
	Layout* pOne = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 804);
	pOne->setTouchEnabled(false);
	pOne->setVisible(true);
	pOne->setZOrder(300);
	addShiled2Layer();

	pOne->setAnchorPoint(Vec2(0.5, 0.5));
	pOne->setScale(0.1);
	auto seq = Sequence::createWithTwoActions(ScaleTo::create(0.2, 1.1), ScaleTo::create(0.1, 1.0));
	pOne->runAction(seq);

	ui::MyButton* buttonSure = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 806);
	buttonSure->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackOk, this));
	buttonSure->setClickPicture();

	__String* info = m_pXmlParser->getString(pWords);
	ui::Text* pText = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 805);
	if(nNum < 0)
		pText->setString(info->getCString());
	else
	{
		__String* pNumWords = __String::createWithFormat(info->getCString(), nNum);
		pText->setString(pNumWords->getCString());
	}
}
//显示VIP界面
void PublicLayer::showVIPLayer()
{
	setLayerVisible(LAYER_VIP);
	addShiledLayer();

	ui::ScrollView * pScrollView = (ui::ScrollView *)ui::Helper::seekWidgetByTag(m_pUINode, 161);
	pScrollView->setInnerContainerSize(Size(546, 200));

	//关闭
	ui::MyButton* buttonClose = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 4209);
	buttonClose->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackCommonClose, this));
	buttonClose->setClickPicture();

	//充值
	ui::MyButton* buttonRecharge = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 4278);
	buttonRecharge->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackRecharge, this));
	buttonRecharge->setClickPicture();

	int nTag = 4222;
	for (int i = 0; i < 3; i++)
	{
		ui::MyButton* buttonChange = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, nTag + i*9);
		addIconForGun(buttonChange);
		buttonChange->setTouchEnabled(false);
	}

	//VIP等级
	int nVIPLevel = UserData::getInstance()->getCurVipLevel();
	int nCurRecharge = UserData::getInstance()->getRechargeAmount();
	int nNextAmount = UserData::getInstance()->getNextVIPLevelAmount();
	int nCurAmount = UserData::getInstance()->getCurVIPLevelAmount();
	float fPercentage = float(nCurRecharge - nCurAmount) / float(nNextAmount - nCurAmount) * 100.0f;
	ui::TextAtlas* pVIPText = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, 4269);
	pVIPText->setString(__String::createWithFormat("%d", nVIPLevel)->getCString());
	
	//差额
	ui::TextAtlas* pText1 = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, 4274);
	pText1->setString(getStringFromInt(nNextAmount - nCurRecharge));
	//下一等级
	ui::TextAtlas* pText2 = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, 4276);
	pText2->setString(getStringFromInt(nVIPLevel + 1));

	if(nVIPLevel == 3)
	{
		pText1->setString(getStringFromInt(0));
		pText2->setString(getStringFromInt(3));
	}
}
//显示获取宝箱界面
void PublicLayer::showGetBoxLayer()
{
	setLayerVisible(LAYER_GETBOX);
	addShiledLayer();

	//打开宝箱的按钮
	ui::MyButton* buttonOpen = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 5842);
	buttonOpen->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackOpenBox, this));
	buttonOpen->setClickPicture();
}
//显示客服界面
void PublicLayer::showServiceLayer()
{
	setLayerVisible(LAYER_SERVICE);
	addShiledLayer();

	//关闭
	ui::MyButton* buttonClose = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 7587);
	buttonClose->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackCommonClose, this));
	buttonClose->setClickPicture();
}
//显示金币兑换界面
void PublicLayer::showExchangeGoldLayer()
{
	setLayerVisible(LAYER_EXCHANGEGOLD);
	addShiledLayer();

	//关闭
	ui::MyButton* buttonClose = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 7607);
	buttonClose->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackCommonClose, this));
	buttonClose->setClickPicture();

	//兑换
	ui::MyButton* buttonExchange = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 7604);
	buttonExchange->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackExchangeGold, this));
	buttonExchange->setClickPicture();

	//输入框
	ui::TextField* pInputFrame = (ui::TextField*)ui::Helper::seekWidgetByTag(m_pUINode, 7608);
	pInputFrame->setText("");
}
//显示首次进游戏礼包界面
void PublicLayer::showFirstGiftLayer()
{
	setLayerVisible(LAYER_FIRSTGIFT);
	addShiledLayer();

	//关闭
	ui::MyButton* buttonClose = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 8751);
	buttonClose->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackCommonClose, this));
	buttonClose->setClickPicture();

	//领取
	ui::MyButton* buttonReceive = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 8756);
	buttonReceive->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackBuy, this));
	buttonReceive->setClickPicture();
}
//显示破产礼包界面
void PublicLayer::showUnenoughGiftLayer()
{
	setLayerVisible(LAYER_UNENOUGH);
	addShiledLayer();

	//关闭
	ui::MyButton* buttonClose = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 8800);
	buttonClose->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackCommonClose, this));
	buttonClose->setClickPicture();

	//领取
	ui::MyButton* buttonReceive = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 8805);
	buttonReceive->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackBuy, this));
	buttonReceive->setClickPicture();
}
//显示10000金币礼包界面
void PublicLayer::showGoldGiftLayer()
{
	setLayerVisible(LAYER_GOLDGIFT);
	addShiledLayer();

	//关闭
	ui::MyButton* buttonClose = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 15461);
	buttonClose->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackCommonClose, this));
	buttonClose->setClickPicture();

	//领取
	ui::MyButton* buttonReceive = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 15466);
	buttonReceive->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackReceiveGreenHandGold, this));
	buttonReceive->setClickPicture();
}

/********免费金币*******/
//微信分享按钮
void PublicLayer::callBackShare(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
	}
}
//签到按钮
void PublicLayer::callBackSign(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		MusicData::getInstance()->playSound(13);
		this->showSignLayer();
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng::MobClickCpp::event(getStringFromInt(YM_SIGN).c_str());
#endif
	}
}
//上线活动按钮
void PublicLayer::callBackOnline(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		MusicData::getInstance()->playSound(13);
		this->showCollectCoinsLayer();
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng::MobClickCpp::event(getStringFromInt(YM_ONLINE_GOLD).c_str());
#endif
	}
}

/********游戏商城*******/
//购买按钮
void PublicLayer::callBackBuy(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		MusicData::getInstance()->playSound(13);
		ui::MyButton* pButton = (ui::MyButton*)pObject;
		int fake_code = 0;
		if(pButton->getTag() == 65)
		{
			fake_code = BUY_GOLD_6;
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
			//umeng::MobClickCpp::event(getStringFromInt(YM_BUY_GOLD_6).c_str());
#endif
		}
		else if(pButton->getTag() == 34)
		{
			fake_code = BUY_GOLD_15;
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
			//umeng::MobClickCpp::event(getStringFromInt(YM_BUY_GOLD_15).c_str());
#endif
		}
		else if(pButton->getTag() == 73)
		{
			fake_code = BUY_GOLD_30;
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
			//umeng::MobClickCpp::event(getStringFromInt(YM_BUY_GOLD_30).c_str());
#endif
		}
		else if(pButton->getTag() == 81)
		{
			fake_code = BUY_GOLD_50;
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
			//umeng::MobClickCpp::event(getStringFromInt(YM_BUY_GOLD_50).c_str());
#endif
		}
		else if(pButton->getTag() == 89)
		{
			fake_code = BUY_GOLD_108;
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
			//umeng::MobClickCpp::event(getStringFromInt(YM_BUY_GOLD_108).c_str());
#endif
		}
		else if(pButton->getTag() == 15323)
		{
			fake_code = BUY_GOLD_328;
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
			//umeng::MobClickCpp::event(getStringFromInt(YM_BUY_GOLD_328).c_str());
#endif
		}
		else if(pButton->getTag() == 14810)
		{
			fake_code = BUY_GOLD_01;
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
			//umeng::MobClickCpp::event(getStringFromInt(YM_BUY_GOLD_01).c_str());
#endif
		}
		else if(pButton->getTag() == 4735)
		{
			fake_code = BUY_DIAMOND_6;
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
			//umeng::MobClickCpp::event(getStringFromInt(YM_BUY_DIAMOND_6).c_str());
#endif
		}
		else if(pButton->getTag() == 4727)
		{
			fake_code = BUY_DIAMOND_15;
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
			//umeng::MobClickCpp::event(getStringFromInt(YM_BUY_DIAMOND_15).c_str());
#endif
		}
		else if(pButton->getTag() == 4743)
		{
			log("buy  250000 gold");
			fake_code = BUY_DIAMOND_30;
			
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
			//umeng::MobClickCpp::event(getStringFromInt(YM_BUY_DIAMOND_30).c_str());
#endif
		}
		else if(pButton->getTag() == 4752)
		{
			fake_code = BUY_DIAMOND_50;
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
			//umeng::MobClickCpp::event(getStringFromInt(YM_BUY_DIAMOND_50).c_str());
#endif
		}
		else if(pButton->getTag() == 4760)
		{
			fake_code = BUY_DIAMOND_108;
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
			//umeng::MobClickCpp::event(getStringFromInt(YM_BUY_DIAMOND_108).c_str());
#endif
		}
		else if(pButton->getTag() == 15335)
		{
			fake_code = BUY_DIAMOND_328;
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
			//umeng::MobClickCpp::event(getStringFromInt(YM_BUY_DIAMOND_328).c_str());
#endif
		}
		else if(pButton->getTag() == 8756)
		{
			fake_code = BUY_GIFT_1;
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
			//umeng::MobClickCpp::event(getStringFromInt(YM_GIFT_1).c_str());
#endif
		}
		else if(pButton->getTag() == 2598)
		{
			fake_code = BUY_GIFT_2;
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
			//umeng::MobClickCpp::event(getStringFromInt(YM_GIFT_2).c_str());
#endif
		}
		else if(pButton->getTag() == 8805)
		{
			fake_code = BUY_GIFT_3;
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
			//umeng::MobClickCpp::event(getStringFromInt(YM_GIFT_3).c_str());
#endif
		}
		else if(pButton->getTag() == 4103)
		{
			fake_code = BUY_GIFT_4;
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
			//umeng::MobClickCpp::event(getStringFromInt(YM_GIFT_4).c_str());
#endif
		}
		//HttpInstance::getInstance()->setRewardStatus(REWARD_STATUS_WAIT);
		//requestRecharge2();

		if(!HttpInstance::getInstance()->getCouldPay())
		{
			this->showPublicSureLayer("handling");
		}

		HttpInstance::getInstance()->setFakeCode(fake_code);
		//10元以上的直接切起凡
		if(fake_code == 1 || fake_code == 7 || fake_code == 8 || fake_code == 14 || fake_code == 15 || fake_code == 16)
		{
			//this->recharge();
		}
		else
		{
			log("------qf --- pay---");
			HttpInstance::getInstance()->setPayType(PAY_TYPE_QF);
		}
		if(HttpInstance::getInstance()->getMobileType() >= 1 && 
			HttpInstance::getInstance()->getMobileType() <= 3 && 
			HttpInstance::getInstance()->getCouldPay())
		{
			this->requestRecharge1();
			HttpInstance::getInstance()->setCouldPay(false);
			log("------normal---sim---card---");
		}
		else if(HttpInstance::getInstance()->getMobileType() == 0 && 
			HttpInstance::getInstance()->getCouldPay())
		{
			HttpInstance::getInstance()->setPayType(PAY_TYPE_QF);
			this->requestRecharge1();
			HttpInstance::getInstance()->setCouldPay(false);
			log("------no---sim---card---");
		}

	}
}
//隐藏首冲双倍
void PublicLayer::hideDouble()
{
	//首冲双倍的图片
	int bFirst = UserData::getInstance()->getFirstShopping();
	ui::ImageView* pFirst = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 2609);
	if(bFirst == 1)
	{
		pFirst->setVisible(false);
	}
}

/********签到界面*******/
//购买按钮
void PublicLayer::callBackSignToday(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		if(m_nSignedNum < 7 && !m_bReceived)
		{
			pu_music->playSound(23);
			
			//this->schedule(schedule_selector(PublicLayer::updateCoinEffect), 5.0/60.0f, 8, 0.1f);
			//添加粒子
			ParticleSystemQuad* pBoom = ParticleSystemQuad::create("particle/gold_particle0.plist");
			pBoom->setPosition(Vec2(size.width / 2.0f, size.height / 3.0f * 2.0f));
			this->addChild(pBoom, COIN_zOrder + 10000);

			m_nSignedNum ++;
			int nTempNum = m_nSignedNum;
			m_bReceived = true;
			UserData::getInstance()->setSignInNum(nTempNum);
			//记录时间（日期 月日）
			tm* pTime = GetTime1();
			int nMon = pTime->tm_mon + 1;
			int nDay = pTime->tm_mday;
			int nData = nMon*100 + nDay;

			float nRate = 1.0f;
			int nCurVIP = UserData::getInstance()->getCurVipLevel();
			if(nCurVIP == 1)
				nRate = 1.2f;
			else if(nCurVIP == 2)
				nRate = 1.5f;
			else if(nCurVIP >= 3)
				nRate = 2.0f;

			int nGetCoin = 100;
			if(m_nSignedNum == 1)
				nGetCoin = 100 * nRate;
			else if(m_nSignedNum == 2)
				nGetCoin = 500 * nRate;
			else if(m_nSignedNum == 3)
				nGetCoin = 1000 * nRate;
			else if(m_nSignedNum == 4)
				nGetCoin = 2000 * nRate;
			else if(m_nSignedNum == 5)
				nGetCoin = 3000 * nRate;
			else if(m_nSignedNum == 6)
				nGetCoin = 5000 * nRate;
			else if(m_nSignedNum == 7)
				nGetCoin = 10000 * nRate;

			UserData::getInstance()->setGoldNum(UserData::getInstance()->getGoldNum() + nGetCoin);
			UserData::getInstance()->setSignInLastTime(nData);
			UserData::getInstance()->save();

			refreshMenuGold();

			
			auto seq = Sequence::create(DelayTime::create(0.3f), 
				CallFunc::create(CC_CALLBACK_0(PublicLayer::callBackAutoClose, this)), 
				NULL);
			this->runAction(seq);
		}
		this->showSignLayer(false);
	}
}
//显示回金币效果
void PublicLayer::showCoinEffect()
{
	Size size = Director::getInstance()->getWinSize();
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/golden_coin.plist");
	//获取动画帧数和缓存中的标记
	Vector<SpriteFrame*> array1(8);

	for (int i=1; i <= 8; i++)
	{
		SpriteFrameCache *pcache = SpriteFrameCache::getInstance();
		std::string format = "golden_coin%d.png";
		__String* strName = __String::createWithFormat(format.c_str(), i);
		SpriteFrame* pframe = pcache->getSpriteFrameByName(strName->getCString());
		array1.pushBack(pframe);
	}
	//根据数组创建动画并添加到动画缓存
	Animation *panimation = Animation::createWithSpriteFrames(array1, 0.04f);

	Sprite* sp = Sprite::create();
	sp->setScale(SCALE);
	sp->setPosition(Vec2(size.width / 2.0f, size.height / 2.0f));
	this->addChild(sp, 1000);

	sp->runAction(RepeatForever::create(Animate::create(panimation)));

	auto seq = Sequence::create(MoveBy::create(0.5f, Vec2(0.0f, -50.0f)), 
		MoveTo::create(1.0, Vec2(size.width - 200, size.height - 50)),
		CallFuncN::create(CC_CALLBACK_1(PublicLayer::callBackRemove, this)),
		NULL);

	auto seq1 = Sequence::create(MoveBy::create(0.5f, Vec2(0.0f, 50.0f)), 
		MoveTo::create(1.0, Vec2(size.width / 2.0f, 30)),
		CallFuncN::create(CC_CALLBACK_1(PublicLayer::callBackRemove, this)),
		NULL);

	if(m_bInGame)
		sp->runAction(seq1);
	else
		sp->runAction(seq);
}
//更新回金币效果
void PublicLayer::updateCoinEffect(float dt)
{
	showCoinEffect();
}
/********领取金币界面*******/
//购买按钮
void PublicLayer::callBackCollectCoins(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
        pu_music->playSound(23);
		int nCurIndex = getIndexByCurTime();
		//int nCoinsNum = getCoinsNumByCurIndex(nCurIndex);

		m_nReceivedCoinIndex = nCurIndex;
		if(m_nReceivedCoinIndex >= 11)
		{
			m_nReceivedCoinIndex = 11;
		}

		tm* pTime = GetTime1();
		int nMon = pTime->tm_mon + 1;
		int nDay = pTime->tm_mday;
		int nData = nMon*100 + nDay;
		float nRate = 1.0f;
		int nCurVIP = UserData::getInstance()->getCurVipLevel();
		if(nCurVIP == 1)
			nRate = 1.2f;
		else if(nCurVIP == 2)
			nRate = 1.5f;
		else if(nCurVIP >= 3)
			nRate = 2.0f;
		UserData::getInstance()->setGoldNum(UserData::getInstance()->getGoldNum() + 200 * nRate);
		UserData::getInstance()->setAwardLastTime(nData);
		UserData::getInstance()->setAwardIndex(m_nReceivedCoinIndex);
		UserData::getInstance()->save();

		refreshMenuGold();

		//记录当前领取的进度
		showCollectCoinsLayer(false);

		//this->schedule(schedule_selector(PublicLayer::updateCoinEffect), 5.0/60.0f, 8, 0.1f);	
		//添加粒子
		ParticleSystemQuad* pBoom = ParticleSystemQuad::create("particle/gold_particle0.plist");
		pBoom->setPosition(Vec2(size.width / 2.0f, size.height / 3.0f * 2.0f));
		this->addChild(pBoom, COIN_zOrder + 10000);

		auto seq = Sequence::create(DelayTime::create(0.3f), 
			CallFunc::create(CC_CALLBACK_0(PublicLayer::callBackAutoClose, this)), 
			NULL);
		this->runAction(seq);
	}
}
//自动关闭回调
void PublicLayer::callBackAutoClose()
{
	this->hideLayer();
	if(!m_bInGame)
	{
		if(!Manager::getInstance()->getOpenFreeGoldFromSign())
			this->showFreeGoldLayer();
		else
			Manager::getInstance()->setOpenFreeGoldFromSign(false);
	}
}

/********游戏设置界面*******/
//设置按钮
void PublicLayer::valueChanged(Ref* sender, Control::EventType controlEvent)
{
    MusicData* myMusic = MusicData::getInstance();
	ControlSwitch* pSwitch = (ControlSwitch*)sender;
    
    UserData* user = UserData::getInstance();
	if (pSwitch->isOn())
	{
        
		if(pSwitch->getTag() == 0)//背景音乐
		{
			user->setMusic(1);
			myMusic->onMusic();
		}
		else if(pSwitch->getTag() == 1)//音效
		{
			user->setSound(1);
			myMusic->OnSound();
		}
		else
		{
            user->setSmooth(1);
		}
		//log("-------On-----");
        
        
	}
	else
	{
		if(pSwitch->getTag() == 0)//背景音乐
		{
			myMusic->stopMusic();
			user->setMusic(0);           
		}
		else if(pSwitch->getTag() == 1)//音效
		{
			myMusic->stopSound();
			user->setSound(0);    
		}
		else
		{
            user->setSmooth(0);
		}
		//log("-------Off-----");
        
	}
    user->save();
}

/********游戏主界面*******/
//领取金币
void PublicLayer::callBackReceiveCoins(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		pu_music->playSound(13);
		m_bDirectClose = true;
		this->showCollectCoinsLayer();
	}
}
//定屏
void PublicLayer::callBackStopFish(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		pu_music->playSound(34);
        pu_music->playSound(2);
		m_pDelegateGame->useToolStopFish(pObject);
		//如果有引导就移除
		MyButton* pButton = (MyButton*)pObject;
		if(pButton->getChildByTag(kTagGuideArrow))
		{
			pButton->removeChildByTag(kTagGuideArrow);
		}	
	}
}
//加速
void PublicLayer::callBackSpeedUp(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		pu_music->playSound(34);
		m_pDelegateGame->useToolSpeedUp(pObject);
		//如果有引导就移除
		MyButton* pButton = (MyButton*)pObject;
		if(pButton->getChildByTag(kTagGuideArrow))
		{
			pButton->removeChildByTag(kTagGuideArrow);
		}	
	}
}
//狂暴
void PublicLayer::callBackRage(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		pu_music->playSound(34);
		m_pDelegateGame->useToolRage(pObject);
		//如果有引导就移除
		MyButton* pButton = (MyButton*)pObject;
		if(pButton->getChildByTag(kTagGuideArrow))
		{
			pButton->removeChildByTag(kTagGuideArrow);
		}	
	}
}
//锁定
void PublicLayer::callBackLockFish(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		pu_music->playSound(34);
		m_pDelegateGame->useToolLockFish(pObject);
		//如果有引导就移除
		MyButton* pButton = (MyButton*)pObject;
		if(pButton->getChildByTag(kTagGuideArrow))
		{
			pButton->removeChildByTag(kTagGuideArrow);
		}	
	}
}
//万炮
void PublicLayer::callBackTenThousand(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		pu_music->playSound(34);
		m_pDelegateGame->useToolTenThousand(pObject);
		//如果有引导就移除
		MyButton* pButton = (MyButton*)pObject;
		if(pButton->getChildByTag(kTagGuideArrow))
		{
			pButton->removeChildByTag(kTagGuideArrow);
		}	
	}
}
//返回
void PublicLayer::callBackReturn(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		pu_music->playSound(13);
		this->showSelectLayer();
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng::MobClickCpp::event(getStringFromInt(YM_SING_RETURN).c_str());
#endif
	}
}
//菜单
void PublicLayer::callBackMenu(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		pu_music->playSound(12);
		ui::MyButton* pButton = (ui::MyButton*)pObject;
		if(pButton->getTag() == 1534)
		{
			if(m_bMoving)
			{
				return;
			}
			ui::MyButton* buttonSwitchGun = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 1537);
			ui::MyButton* buttonHelp = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 1536);
			ui::MyButton* buttonReturn = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 1535);
			ui::MyButton* buttonMenu = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 1534);
			if(m_bOpening)
			{
				auto seq = Sequence::create(MoveTo::create(0.2, buttonMenu->getPosition()),
					CallFuncN::create(CC_CALLBACK_1(PublicLayer::callBackHide, this)),
					NULL);
				buttonReturn->runAction(seq);
				buttonSwitchGun->runAction(seq->clone());
				buttonHelp->runAction(seq->clone());

				m_bMoving = true;
			}
			else
			{
				buttonReturn->setVisible(true);
				buttonSwitchGun->setVisible(true);
				buttonHelp->setVisible(true);

				auto seq = Sequence::create(MoveBy::create(0.2, Vec2(0.0f, 70)),
					MoveBy::create(0.1, Vec2(0.0f, -5)),
					CallFuncN::create(CC_CALLBACK_1(PublicLayer::callBackShowFinish, this)),
					NULL);
				auto seq1 = Sequence::create(MoveBy::create(0.2, Vec2(0.0f, 140)),
					MoveBy::create(0.1, Vec2(0.0f, -10)),
					CallFuncN::create(CC_CALLBACK_1(PublicLayer::callBackShowFinish, this)),
					NULL);
				auto seq2 = Sequence::create(MoveBy::create(0.2, Vec2(0.0f, 210)),
					MoveBy::create(0.1, Vec2(0.0f, -15)),
					CallFuncN::create(CC_CALLBACK_1(PublicLayer::callBackShowFinish, this)),
					NULL);
				buttonReturn->runAction(seq);
				buttonHelp->runAction(seq1);
				buttonSwitchGun->runAction(seq2);

				m_bOpening = true;
				m_bMoving = true;
			}
		}
		else
		{
			if(m_bMoving)
			{
				return;
			}
			ui::MyButton* buttonHelp = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 708);
			ui::MyButton* buttonConfigure = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 707);
			ui::MyButton* buttonReturn = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 699);
			ui::MyButton* buttonMenu = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 706);
			if(m_bOpening)
			{
				auto seq = Sequence::create(MoveTo::create(0.2, buttonMenu->getPosition()),
					CallFuncN::create(CC_CALLBACK_1(PublicLayer::callBackHide, this)),
					NULL);
				buttonConfigure->runAction(seq);
				buttonHelp->runAction(seq->clone());
				buttonReturn->runAction(seq->clone());

				m_bMoving = true;
			}
			else
			{
				buttonConfigure->setVisible(true);
				buttonHelp->setVisible(true);
				buttonReturn->setVisible(true);

				auto seq = Sequence::create(MoveBy::create(0.2, Vec2(0.0f, 70)),
					MoveBy::create(0.1, Vec2(0.0f, -5)),
					CallFuncN::create(CC_CALLBACK_1(PublicLayer::callBackShowFinish, this)),
					NULL);
				auto seq1 = Sequence::create(MoveBy::create(0.2, Vec2(0.0f, 140)),
					MoveBy::create(0.1, Vec2(0.0f, -10)),
					CallFuncN::create(CC_CALLBACK_1(PublicLayer::callBackShowFinish, this)),
					NULL);
				auto seq2 = Sequence::create(MoveBy::create(0.2, Vec2(0.0f, 210)),
					MoveBy::create(0.1, Vec2(0.0f, -15)),
					CallFuncN::create(CC_CALLBACK_1(PublicLayer::callBackShowFinish, this)),
					NULL);
				buttonConfigure->runAction(seq2);
				buttonHelp->runAction(seq1);
				buttonReturn->runAction(seq);

				m_bOpening = true;
				m_bMoving = true;
			}
		}
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng::MobClickCpp::event(getStringFromInt(YM_SING_MENU).c_str());
#endif
	}
}
//帮助
void PublicLayer::callBackHelp(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		pu_music->playSound(13);
		this->showHelpLayer();
	}
}
//设置
void PublicLayer::callBackTenConfigure(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		pu_music->playSound(13);
		this->showConfigureLayer();
	}
}
//加金币
void PublicLayer::callBackAddGold(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		pu_music->playSound(13);
		this->showShopLayer();
	}
}
//充值
void PublicLayer::callBackShop(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		pu_music->playSound(13);
		this->showShopLayer();
	}
}
//换炮
void PublicLayer::callBackSwitchGun(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
	    pu_music->playSound(13);
		this->showSwitchGunLayer();
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng::MobClickCpp::event(getStringFromInt(YM_SWITCH_GUN).c_str());
#endif
	}
}
//VIP
void PublicLayer::callBackShowVIP(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		pu_music->playSound(13);
		this->showVIPLayer();
	}
}
//四人彩金的进度显示
void PublicLayer::callBackShowTaskNum(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		if(m_bMoving)return;
		Layout* pPool = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 5845);
		//箭头
		ui::ImageView* pArrow = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 6682);
		Vec2 curPos = pPool->getPosition();
		if(curPos.y > 240.0f)
		{
			float fMove = 120.0f;
			if(curPos.y > 400)
			{
				fMove = -120.0f;
				pArrow->runAction(RotateTo::create(0.0f, 180.0f));
			}
			else
			{
				pArrow->runAction(RotateTo::create(0.0f, 0.0f));
			}
			auto seq = Sequence::create(MoveBy::create(0.2f, Vec2(0.0f, fMove)), 
				CallFuncN::create(CC_CALLBACK_1(PublicLayer::callBackShowFinish, this)), 
				NULL);
			pPool->runAction(seq);
		}
		else
		{
			float fMove = 120.0f;
			if(curPos.y > 80)
			{
				fMove = -120.0f;
				pArrow->runAction(RotateTo::create(0.0f, 0.0f));
			}
			else
			{
				pArrow->runAction(RotateTo::create(0.0f, 180.0f));
			}
			auto seq = Sequence::create(MoveBy::create(0.2f, Vec2(0.0f, fMove)), 
				CallFuncN::create(CC_CALLBACK_1(PublicLayer::callBackShowFinish, this)), 
				NULL);
			pPool->runAction(seq);
		}
		m_bMoving = true;
	}
}
//刷新技能剩余次数
void PublicLayer::refreshSkillNum()
{
	int nNum1 = UserData::getInstance()->getSkillNum1();
	int nNum2 = UserData::getInstance()->getSkillNum2();
	int nNum3 = UserData::getInstance()->getSkillNum3();
	int nNum4 = UserData::getInstance()->getSkillNum4();
	int nNum5 = UserData::getInstance()->getSkillNum5();

	if(Manager::getInstance()->getRoomNum() == 1)
	{
		//钻石图片
		ui::ImageView* pDiamondImage1 = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 3836);
		ui::ImageView* pDiamondImage2 = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 3837);
		ui::ImageView* pDiamondImage3 = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 3838);
		ui::ImageView* pDiamondImage4 = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 3839);
		ui::ImageView* pDiamondImage5 = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 3840);

		pDiamondImage1->setVisible(false);
		pDiamondImage2->setVisible(false);
		pDiamondImage3->setVisible(false);
		pDiamondImage4->setVisible(false);
		pDiamondImage5->setVisible(false);

		ui::Text* pSkillNum1 = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 3831);
		ui::Text* pSkillNum2 = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 3832);
		ui::Text* pSkillNum3 = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 3833);
		ui::Text* pSkillNum4 = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 3834);
		ui::Text* pSkillNum5 = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 3835);

		pSkillNum1->setVisible(false);
		pSkillNum2->setVisible(false);
		pSkillNum3->setVisible(false);
		pSkillNum4->setVisible(false);
		pSkillNum5->setVisible(false);
	
		ui::Text* pNewSkillNum1 = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 5362);
		pNewSkillNum1->setVisible(true);
		pNewSkillNum1->setString("x" + getStringFromInt(nNum1));
		if(nNum1 == 0)
		{
			pDiamondImage1->setVisible(true);
			pSkillNum1->setVisible(true);
			pNewSkillNum1->setVisible(false);
		}
		ui::Text* pNewSkillNum2 = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 5363);
		pNewSkillNum2->setVisible(true);
		pNewSkillNum2->setString("x" + getStringFromInt(nNum2));
		if(nNum2 == 0)
		{
			pDiamondImage2->setVisible(true);
			pSkillNum2->setVisible(true);
			pNewSkillNum2->setVisible(false);
		}
		ui::Text* pNewSkillNum3 = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 5364);
		pNewSkillNum3->setVisible(true);
		pNewSkillNum3->setString("x" + getStringFromInt(nNum3));
		if(nNum3 == 0)
		{
			pDiamondImage3->setVisible(true);
			pSkillNum3->setVisible(true);
			pNewSkillNum3->setVisible(false);
		}
		ui::Text* pNewSkillNum4 = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 5365);
		pNewSkillNum4->setVisible(true);
		pNewSkillNum4->setString("x" + getStringFromInt(nNum4));
		if(nNum4 == 0)
		{
			pDiamondImage4->setVisible(true);
			pSkillNum4->setVisible(true);
			pNewSkillNum4->setVisible(false);
		}
		ui::Text* pNewSkillNum5 = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 5366);
		pNewSkillNum5->setVisible(true);
		pNewSkillNum5->setString("x" + getStringFromInt(nNum5));
		if(nNum5 == 0)
		{
			pDiamondImage5->setVisible(true);
			pSkillNum5->setVisible(true);
			pNewSkillNum5->setVisible(false);
		}
	}


	if(Manager::getInstance()->getRoomNum() == 2)
	{
		//多人场刷新
		//钻石图片
		ui::ImageView* pDiamondImage6 = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 3844);
		ui::ImageView* pDiamondImage7 = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 3845);
		ui::ImageView* pDiamondImage8 = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 3846);
		pDiamondImage6->setVisible(false);
		pDiamondImage7->setVisible(false);
		pDiamondImage8->setVisible(false);

		ui::Text* pSkillNum1 = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 5371);
		ui::Text* pSkillNum2 = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 5372);
		ui::Text* pSkillNum3 = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 5373);
		
		pSkillNum1->setVisible(false);
		pSkillNum2->setVisible(false);
		pSkillNum3->setVisible(false);

		ui::Text* pNewSkillNum1 = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 5374);
		pNewSkillNum1->setVisible(true);
		pNewSkillNum1->setString("x" + getStringFromInt(nNum1));
		if(nNum1 == 0)
		{
			pDiamondImage6->setVisible(true);
			pSkillNum1->setVisible(true);
			pNewSkillNum1->setVisible(false);
		}
		ui::Text* pNewSkillNum2 = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 5375);
		pNewSkillNum2->setVisible(true);
		pNewSkillNum2->setString("x" + getStringFromInt(nNum4));
		if(nNum4 == 0)
		{
			pDiamondImage7->setVisible(true);
			pSkillNum2->setVisible(true);
			pNewSkillNum2->setVisible(false);
		}
		ui::Text* pNewSkillNum3 = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 5376);
		pNewSkillNum3->setVisible(true);
		pNewSkillNum3->setString("x" + getStringFromInt(nNum2));
		if(nNum2 == 0)
		{
			pDiamondImage8->setVisible(true);
			pSkillNum3->setVisible(true);
			pNewSkillNum3->setVisible(false);
		}
	}

}
//刷新多人场任务情况
void PublicLayer::refreshManyTask()
{
	int nTaskFishID1 = Manager::getInstance()->getTaskFishID1();
	int nTaskFishID2 = Manager::getInstance()->getTaskFishID2();
	int nTaskFishID3 = Manager::getInstance()->getTaskFishID3();

	Layout* pPannel = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 5859);
	ui::Text* pTaskNum1 = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 5850);
	ui::Text* pTaskNum2 = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 5851);
	ui::Text* pTaskNum3 = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 5852);

	pPannel->removeChildByTag(0);
	pPannel->removeChildByTag(1);
	pPannel->removeChildByTag(2);

	auto pFish1 = createFishSprite(nTaskFishID1);
	pFish1->setPosition(Vec2(pTaskNum1->getPositionX() - 50.0f, pTaskNum1->getPositionY()));
	pPannel->addChild(pFish1, 0, 0);
	pFish1->setScale(0.14f);
	pFish1->setRotation(-180);
	if(nTaskFishID1 == 36 || nTaskFishID1 == 38 || nTaskFishID1 == 40)
		pFish1->setRotation(-90);

	auto pFish2 = createFishSprite(nTaskFishID2);
	pFish2->setPosition(Vec2(pTaskNum2->getPositionX() - 50.0f, pTaskNum2->getPositionY()));
	pPannel->addChild(pFish2, 0, 1);
	pFish2->setScale(0.14f);
	pFish2->setRotation(-180);
	if(nTaskFishID2 == 36 || nTaskFishID2 == 38 || nTaskFishID2 == 40)
		pFish2->setRotation(-90);

	auto pFish3 = createFishSprite(nTaskFishID3);
	pFish3->setPosition(Vec2(pTaskNum3->getPositionX() - 50.0f, pTaskNum3->getPositionY()));
	pPannel->addChild(pFish3, 0, 2);
	pFish3->setScale(0.14f);
	pFish3->setRotation(-180);
	if(nTaskFishID3 == 36 || nTaskFishID3 == 38 || nTaskFishID3 == 40)
		pFish3->setRotation(-90);

	//返回百分比
	std::string strPer = "";
	int nLevel = Manager::getInstance()->getTwo_LevelNum();
	if(nLevel == 1)
		strPer = "30%";
	else if(nLevel == 2)
		strPer = "50%";
	else if(nLevel == 3)
		strPer = "75%";
	else if(nLevel == 4)
		strPer = "100%";

	ui::Text* pPercent = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 5855);
	pPercent->setString(strPer);
}
//创建显示的鱼
Sprite* PublicLayer::createFishSprite(int nID)
{
	//任务鱼的显示
	string fishName = d_fishName[nID][0];
	int nFrameNum = getIntFromS(d_fishName[nID][1].c_str());
	auto ani = Animation::create();
	for (int j = 0; j < nFrameNum; j++)
	{
		string strPath = fishName + __String::createWithFormat("%02d.png", j + 1)->getCString();
		auto pFrame = SpriteFrameCache::getInstance()->spriteFrameByName(strPath);
		ani->addSpriteFrame(pFrame);
	}
	ani->setDelayPerUnit(0.1);
	ani->setLoops(-1);
	auto sp = Sprite::create();
	sp->setScale(m_aFishSale[nID]);
	sp->setRotation(-90);
	auto pAni1 = Animate::create(ani);
	sp->runAction(pAni1);

	return sp;
}

/********更换炮台界面*******/
//更换炮台回调
void PublicLayer::callBackChangeGun(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
        pu_music->playSound(13);
		ui::MyButton* buttonChange = (ui::MyButton*)pObject;
		int nTag = buttonChange->getTag();
		int nID = (nTag - 163)/8;
		log("----callBackChangeGun---(%d)---", nID);

		int nNeedCoins = 0;
		int nCurGold = UserData::getInstance()->getGoldNum();
		bool bLock = UserData::getInstance()->getCannon1If();
		if(nID == 0)
		{
			nNeedCoins = 3000;
			bLock = UserData::getInstance()->getCannon1If();
		}
		else if(nID == 1)
		{
			nNeedCoins = 30000;
			bLock = UserData::getInstance()->getCannon2If();
		}
		else if(nID == 2)
		{
			nNeedCoins = 100000;
			bLock = UserData::getInstance()->getCannon3If();
		}
		else if(nID == 3)
		{
			nNeedCoins = 300000;
			bLock = UserData::getInstance()->getCannon4If();
		}
		else if(nID == 4)
		{
			nNeedCoins = 100000;
		}

		//解锁了就装备
		if(bLock)
		{
			
			int cannonNum = Manager::getInstance()->getM_BulletNum();
			switch (nID) {
			case 0:
				m_pDelegateGame->chageManualCannon(0, cannonNum);
				break;
			case 1:
				m_pDelegateGame->chageManualCannon(2, cannonNum);
				break;
			case 2:
				m_pDelegateGame->chageManualCannon(3, cannonNum);
				break;
			case 3:
				m_pDelegateGame->chageManualCannon(4, cannonNum);
				break;
			case 4:
				m_pDelegateGame->chageManualCannon(4, cannonNum);
				break;

			default:
				break;
			}
			if(nID > 0)nID++;
			UserData::getInstance()->setCannonID(nID);
			UserData::getInstance()->save();

			this->showSwitchGunLayer();
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng::MobClickCpp::event(getStringFromInt(YM_UNLOCK_GUN_1 + nID).c_str());
#endif
		}
		else
		{
			//钱够了
			if(nCurGold >= nNeedCoins)
			{
				//减少钱
				UserData::getInstance()->setGoldNum(UserData::getInstance()->getGoldNum() - nNeedCoins);
				//装备图片
				ui::ImageView* imageEquip = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 260 + nID);
				imageEquip->setVisible(true);
				//购买
				//ui::MyButton * buttonBuy = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 163 + nID * 8);
				//buttonBuy->setTouchEnabled(false);

				if(nID == 0)
					UserData::getInstance()->setCannon1If(true);
				else if(nID == 1)
					UserData::getInstance()->setCannon2If(true);
				else if(nID == 2)
					UserData::getInstance()->setCannon3If(true);
				else if(nID == 3)
					UserData::getInstance()->setCannon4If(true);
				else if(nID == 4)
					UserData::getInstance()->setCannon5If(true);

				UserData::getInstance()->save();
				Manager::getInstance()->setIsCoinChange(true);
				this->showSureLayer();

				Manager::getInstance()->setRefreshGun(true);
			}
			else
			{
				showCoinNotEnoughLayer();
				m_bOpenOnUnenough = true;
			}
		}

		
	}
}
//装备炮台
void PublicLayer::callBackEquipGun(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
        pu_music->playSound(13);
		ui::MyButton* buttonEquip1 = (ui::MyButton*)pObject;
		int nTag = buttonEquip1->getTag();

		int nBuyTag = 163;
		int nEquipImageTag = 260;
		int nEquipTag = 1238;
		bool bLock = true;
		int nSelect = 0;
		for (int i = 0; i < 4; i++)
		{
			if(i == 0)
				bLock = UserData::getInstance()->getCannon1If();
			else if(i == 1)
				bLock = UserData::getInstance()->getCannon2If();
			else if(i == 2)
				bLock = UserData::getInstance()->getCannon3If();
			else if(i == 3)
				bLock = UserData::getInstance()->getCannon4If();
			else if(i == 4)
				bLock = UserData::getInstance()->getCannon5If();
			int nTempTag = nEquipTag + i*3;
			if(i == 0)
				nTempTag = 1235;
			//装备图片
			ui::ImageView* imageEquip = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, nEquipImageTag + i);
			imageEquip->setVisible(true);

			//购买
			ui::MyButton * buttonChange = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, nBuyTag + i * 8);
			buttonChange->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackChangeGun, this));
			buttonChange->setClickPicture();

			//装备
			ui::MyButton * buttonEquip = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, nTempTag);
			buttonEquip->addTouchEventListener(CC_CALLBACK_2(PublicLayer::callBackEquipGun, this));
			buttonEquip->setClickPicture();
			buttonEquip->setVisible(true);

			//是否解锁
			if(!bLock)
			{
				imageEquip->setVisible(false);
			}
			else
			{
				buttonChange->setTouchEnabled(false);
			}

			if(nTempTag == nTag)
			{
				buttonEquip->setVisible(false);
				nSelect = i;
			}
		}
        int cannonNum = Manager::getInstance()->getM_BulletNum();
        switch (nSelect) {
            case 0:
                m_pDelegateGame->chageManualCannon(0, cannonNum);
                break;
            case 1:
                m_pDelegateGame->chageManualCannon(2, cannonNum);
                break;
            case 2:
                m_pDelegateGame->chageManualCannon(3, cannonNum);
                break;
            case 3:
                m_pDelegateGame->chageManualCannon(4, cannonNum);
                break;
            case 4:
                m_pDelegateGame->chageManualCannon(4, cannonNum);
                break;

            default:
                break;
        }

		if(nSelect > 0)
			nSelect = nSelect + 1;
		UserData::getInstance()->setCannonID(nSelect);
		UserData::getInstance()->save();
	}
}
//添加炮的ICON
void PublicLayer::addIconForGun(MyButton* pButton)
{
	//if(m_bInGame)
	{
		if(!pButton->getChildByTag(kTagGunIcon))
		{
			int nID = 0;
			if(pButton->getTag() == 163)
				nID = 0;
			else if(pButton->getTag() == 171 || pButton->getTag() == 4222)
				nID = 1;
			else if(pButton->getTag() == 179 || pButton->getTag() == 4231)
				nID = 2;
			else if(pButton->getTag() == 187 || pButton->getTag() == 4240)
				nID = 3;

			std::string str1 = "battery_red.png";
			std::string str2 = "cannon_red2.png";
			std::string str3 = "display_red.png";
			if(nID == 1)
			{
				str1 = "battery_bronze.png";
				str2 = "cannon_bronze2.png";
				str3 = "display_bronze.png";
			}
			if(nID == 2)
			{
				str1 = "battery_silver.png";
				str2 = "cannon_silver2.png";
				str3 = "display_silver.png";
			}
			if(nID == 3)
			{
				str1 = "battery_golden.png";
				str2 = "cannon_golden2.png";
				str3 = "display_golden.png";
			}
			
			auto sp1 = Sprite::createWithSpriteFrameName(str1);
			sp1->setScale(SCALE);
			sp1->setPosition(Vec2(pButton->getContentSize().width / 2.0, pButton->getContentSize().height / 2.0 + 30));
			pButton->addChild(sp1, 0, kTagGunIcon);

			auto sp2 = Sprite::createWithSpriteFrameName(str2);
			sp2->setPosition(Vec2(sp1->getContentSize().width / 2.0, sp1->getContentSize().height / 2.0));
			sp1->addChild(sp2);

			auto sp3 = Sprite::createWithSpriteFrameName(str3);
			sp3->setPosition(Vec2(sp1->getContentSize().width / 2.0, sp1->getContentSize().height / 2.0));
			sp1->addChild(sp3);
		}
	}
}
//刷新换炮界面
void PublicLayer::callBackRefreshGun(Node* pNode)
{
	this->showSwitchGunLayer();
}

/********客服界面*******/
//兑换回调
void PublicLayer::callBackExchangeGold(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		this->hideLayer();
		ui::TextField* pInputFrame = (ui::TextField*)ui::Helper::seekWidgetByTag(m_pUINode, 7608);
		//HttpInstance::getInstance()->requestIdentifyingCode(pInputFrame->getStringValue());
		//HttpInstance::getInstance()->setPublicLayer(this);
		std::string str = pInputFrame->getStringValue();
		str += "&imei=";
		str += HttpInstance::getInstance()->getDeviceMac();
		str += "&channel=";
		str += HttpInstance::getInstance()->getChannelID();
		HttpInstance::getInstance()->getStore()->RequestOrder(str, 4, this, callfuncND_selector(PublicLayer::callbackGetOrder));

		log("input word == %s", str.c_str());

#if (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
	HttpInstance::getInstance()->requestIdentifyingCode(str);
#endif
		
	}
}

/********弹出框选择界面*******/
//确定
void PublicLayer::callBackOk(Ref* pObject, ui::Widget::TouchEventType ty)
{
    
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
        pu_music->playSound(14);
        
        
		ui::MyButton* buttonOk = (ui::MyButton*)pObject;
		int nTag = buttonOk->getTag();
		
        auto seq = Sequence::create(ScaleTo::create(0.2, 0),
                                    CallFuncN::create(CC_CALLBACK_1(PublicLayer::callBackHideButton, this)),
                                    NULL);
		if(nTag == 802)
		{
			Manager::getInstance()->setEnterGame(false);
			Scene* scene1 = Scene::create();
			scene1->addChild(LoadingLayer::create());
			Director::getInstance()->replaceScene(scene1);
		}
		else if(nTag == 806)
		{
			Layout* pOne = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 804);
			Layout* pShiled = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1253);
			pShiled->setVisible(false);
			pShiled->setTouchEnabled(false);
			pOne->runAction(seq);

			//提示VIP等级不足后弹出VIP界面
			if(Manager::getInstance()->getNeedOpenVIP())
			{
				this->showVIPLayer();
				Manager::getInstance()->setNeedOpenVIP(false);
			}
			

			if(Manager::getInstance()->getRefreshGun())
			{
				Manager::getInstance()->setRefreshGun(false);
				auto seq = Sequence::create(DelayTime::create(0.2f), CallFuncN::create(CC_CALLBACK_1(PublicLayer::callBackRefreshGun, this)), NULL);
				this->runAction(seq);
			}
		}
		else if(nTag == 1231)
	    {
			Layout* pLayer = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1218);
			Layout* pShiled = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1252);
			pShiled->setVisible(false);
			pShiled->setTouchEnabled(false);

			pLayer->runAction(seq);
			this->showShopLayer();
		}
	}
}
//取消
void PublicLayer::callBackCancel(Ref* pObject, ui::Widget::TouchEventType ty)
{
    
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
        pu_music->playSound(14);
        
		ui::MyButton* buttonCancel = (ui::MyButton*)pObject;

		auto seq = Sequence::create(ScaleTo::create(0.2, 0),
			CallFuncN::create(CC_CALLBACK_1(PublicLayer::callBackHideButton, this)),
			NULL);

		if(buttonCancel->getTag() == 803)
		{
			Layout* pTwo = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 800);
			//pTwo->setVisible(false);
			Layout* pShiled = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1253);
			pShiled->setVisible(false);
			pShiled->setTouchEnabled(false);

			pTwo->runAction(seq);
		}
		else if(buttonCancel->getTag() == 1232)
		{
			Layout* pLayer = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1218);
			//pLayer->setVisible(false);
			Layout* pShiled = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1252);
			pShiled->setVisible(false);
			pShiled->setTouchEnabled(false);

			pLayer->runAction(seq);
			this->showFreeGoldLayer();
		}


	}
}

/********任务界面*******/
//打开任务界面
void PublicLayer::callBackTaskHelp(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
        pu_music->playSound(13);
		this->showTaskLayer();
	}
}
//完成一个任务
void PublicLayer::finishedOneTask()
{
	Layout* pOne = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1490);
	Layout* pTwo = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1498);
	Vec2 pos = pOne->getPosition();

	auto seq = Sequence::create(MoveTo::create(1.0f, Vec2(size.width / 2.0f, size.height / 2.0f)), 
		CallFunc::create(CC_CALLBACK_0(PublicLayer::callBackShowBigDialog, this)),
		DelayTime::create(1.0f),
		CallFunc::create(CC_CALLBACK_0(PublicLayer::callBackHideDialog, this)),
		CallFunc::create(CC_CALLBACK_0(PublicLayer::callBackShowLeftFirstTask, this)),
		MoveTo::create(1.0f, pos), 
		CallFunc::create(CC_CALLBACK_0(PublicLayer::callBackHideLeftFirstTask, this)),
		NULL);

	pOne->runAction(seq);

	auto seq1 = Sequence::create(DelayTime::create(1.0f),
		MoveTo::create(1.0f, pos), 
		CallFunc::create(CC_CALLBACK_0(PublicLayer::callBackRemoveTaskLock, this)),
		NULL);
	pTwo->runAction(seq1);
}
//完成两个任务
void PublicLayer::finishedTwoTask()
{
	Layout* pTwo = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1498);
	Vec2 pos = pTwo->getPosition();
	auto seq = Sequence::create(MoveTo::create(1.0f, Vec2(size.width / 2.0f, size.height / 2.0f)), 
		CallFunc::create(CC_CALLBACK_0(PublicLayer::callBackShowBigDialog, this)),
		DelayTime::create(1.0f),
		CallFunc::create(CC_CALLBACK_0(PublicLayer::callBackHideDialog, this)),
		CallFunc::create(CC_CALLBACK_0(PublicLayer::callBackShowLeftSecondTask, this)),
		MoveTo::create(1.0f, pos), 
		CallFunc::create(CC_CALLBACK_0(PublicLayer::callBackHideLeftSecondTask, this)),
		NULL);

	pTwo->runAction(seq);

}
//显示中间的任务
void PublicLayer::callBackShowBigDialog()
{
	Layout* pOne = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1490);
	pOne->setVisible(false);

	this->showTaskLayer();
}
//隐藏中间的任务
void PublicLayer::callBackHideDialog()
{
	this->hideLayer();
}
//隐藏左边的任务一
void PublicLayer::callBackHideLeftFirstTask()
{
	Layout* pOne = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1490);
	pOne->setVisible(false);
}
//隐藏左边的任务二
void PublicLayer::callBackHideLeftSecondTask()
{
	Layout* pTwo = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1498);
	pTwo->setVisible(false);

	this->showLeftTaskLayer();
}
//显示左边的任务一
void PublicLayer::callBackShowLeftFirstTask()
{
	Layout* pOne = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1490);
	pOne->setVisible(true);
}
//显示左边的任务二
void PublicLayer::callBackShowLeftSecondTask()
{
	Layout* pTwo = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1498);
	pTwo->setVisible(true);
}
//移除锁
void PublicLayer::callBackRemoveTaskLock()
{
	ui::ImageView* pLock = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 1502);
	pLock->runAction(FadeOut::create(0.5f));
	//pLock->setVisible(false);
}

/********过关界面*******/
//打开过关界面
void PublicLayer::callBackGetReward(Ref* pObject, ui::Widget::TouchEventType ty)
{
    
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
        //添加声音
        pu_music->playSound(69);
        
		this->showCrossLayer();
		//领取通关奖励
		ui::MyButton* buttonCross = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 1505);
		buttonCross->setVisible(false);

		//前往下一关
		ui::MyButton* buttonNextMission = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 1841);
		buttonNextMission->setVisible(true);

		int nStage = Manager::getInstance()->getOne_LevelNum();
		//左边的数量
		UserDataMission* pData = UserData::getInstance()->getUserDataMissionByID(nStage);
		pData->setMissionSitate(1);

        int nGold = nStage*500+1000;
        
		UserData::getInstance()->setGoldNum(UserData::getInstance()->getGoldNum() + nGold);
		UserData::getInstance()->save();

		refreshMenuGold();
	}
}
//继续游戏
void PublicLayer::callBackGoonGame(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
        
        pu_music->playSound(13);
	}
}
//下一关
void PublicLayer::callBackNext(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
        pu_music->playSound(13);
		int nCur = Manager::getInstance()->getOne_LevelNum() + 1;
		if(nCur >= 10) nCur = 10;
		Manager::getInstance()->setOne_LevelNum(nCur);
		//auto pLoading = LoadingLayer::create();
		//this->addChild(pLoading, 500);
		Scene* scene1 = Scene::create();
		scene1->addChild(LoadingLayer::create());
		Director::getInstance()->replaceScene(scene1);
	}
}
//显示按钮
void PublicLayer::callBackShowCoins(Node* pNode)
{
	addCoinsLayer();
}
//添加金币图层
void PublicLayer::addCoinsLayer()
{
	m_nCrossCoinIndex = 0;

	auto pLayer = Layer::create();

	auto pCatch = SpriteFrameCache::getInstance();
	pCatch->addSpriteFramesWithFile("ui/public/batch_fold_gold.plist");

	for (int i = 0; i < 30; i++)
	{
		auto pCoin = Sprite::createWithSpriteFrameName("super_line_coin.png");
		pCoin->setAnchorPoint(Vec2(0.5, 0));
		pCoin->setPosition(Vec2(854*CCRANDOM_0_1(), -150));
		pLayer->addChild(pCoin, 1);
		pCoin->setScale(CCRANDOM_0_1() * 0.3 + 0.3);
		pCoin->runAction(MoveBy::create(3.0, Vec2(0.0, 130)));
	}
	Layout* pShiled = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 799);
	pShiled->addChild(pLayer, 0, kTagCoinsLayer);

	auto seq = Sequence::create(DelayTime::create(3.0),
		CallFuncN::create(CC_CALLBACK_1(PublicLayer::callBackRemoveCoins, this)),
		DelayTime::create(1.0),
		CallFuncN::create(CC_CALLBACK_1(PublicLayer::callBackRemove, this)),
		NULL);
	pLayer->runAction(seq);

	this->schedule(schedule_selector(PublicLayer::dropCoins), 1/60.0);
}
void PublicLayer::dropCoins(float dt)
{
	Layout* pShiled = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 799);
	Layer* pLayer = (Layer*)pShiled->getChildByTag(kTagCoinsLayer);
	//获取动画帧数和缓存中的标记
	Vector<SpriteFrame*> array1(4);
	SpriteFrameCache *pcache = SpriteFrameCache::getInstance();
	for (int i = 1; i <= 4; i++)
	{
		std::string format = "fold_gold_%02d.png";
		__String* strName = __String::createWithFormat(format.c_str(), i);
		SpriteFrame* pframe = pcache->getSpriteFrameByName(strName->getCString());
		array1.pushBack(pframe);
	}
	//根据数组创建动画并添加到动画缓存
	Animation *panimation = Animation::createWithSpriteFrames(array1, 0.1);

	Sprite* sp = Sprite::create();
	sp->setScale(0.6);
	//sp->setRotation(-90);
	sp->setPosition(Vec2(CCRANDOM_0_1()*854, 500));
	pLayer->addChild(sp);

	sp->runAction(MoveBy::create(0.2, Vec2(0.0, -600)));

	sp->runAction(RepeatForever::create(Animate::create(panimation)));

    int nStage = Manager::getInstance()->getOne_LevelNum();
	int nGold = nStage*500+1000;
    
	m_nCrossCoinIndex += 35;
    if (m_nCrossCoinIndex >= nGold) 
	{
        m_nCrossCoinIndex = nGold;
    }

	ui::Text* text_coins = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 1512);
	text_coins->setString(getStringFromInt(m_nCrossCoinIndex));
}

/********帮助界面*******/
//鱼的特殊玩法
void PublicLayer::callBackSpecial(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
        pu_music->playSound(13);
		this->showSpecialPlayLayer();
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng::MobClickCpp::event(getStringFromInt(YM_SPECIAL_PLAY).c_str());
#endif
	}
}
//鱼的倍率
void PublicLayer::callBackRate(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
        pu_music->playSound(13);
		this->showFishRateLayer();
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng::MobClickCpp::event(getStringFromInt(YM_FISH_RATE).c_str());
#endif
	}
}
//加载鱼的动画资源
void PublicLayer::loadFishAnimation()
{
	SpriteFrameCache* cache = CCSpriteFrameCache::getInstance();

	//缓存 炮台
	cache->addSpriteFramesWithFile("cannon_atlas.plist", "cannon_atlas.png");

	cache->addSpriteFramesWithFile("fish_frame_1.plist", "fish_frame_1.png");
	cache->addSpriteFramesWithFile("fish_frame_28.plist", "fish_frame_28.png");
	cache->addSpriteFramesWithFile("fish_frame_29.plist", "fish_frame_29.png");
	cache->addSpriteFramesWithFile("fish_frame_2.plist", "fish_frame_2.png");
	cache->addSpriteFramesWithFile("fish_frame_3.plist", "fish_frame_3.png");
	cache->addSpriteFramesWithFile("fish_frame_4.plist", "fish_frame_4.png");
	cache->addSpriteFramesWithFile("fish_frame_18.plist", "fish_frame_18.png");
	cache->addSpriteFramesWithFile("fish_frame_31.plist", "fish_frame_31.png");
	cache->addSpriteFramesWithFile("fish_frame_166.plist", "fish_frame_166.png");
	cache->addSpriteFramesWithFile("fish_frame_6.plist", "fish_frame_6.png");

	cache->addSpriteFramesWithFile("fish_frame_224.plist", "fish_frame_224.png");
	cache->addSpriteFramesWithFile("fish_frame_54.plist", "fish_frame_54.png");
	cache->addSpriteFramesWithFile("fish_frame_23.plist", "fish_frame_23.png");
	cache->addSpriteFramesWithFile("fish_frame_26.plist", "fish_frame_26.png");
	cache->addSpriteFramesWithFile("fish_frame_241.plist", "fish_frame_241.png");
	cache->addSpriteFramesWithFile("fish_frame_252.plist", "fish_frame_252.png");
	cache->addSpriteFramesWithFile("fish_frame_49.plist", "fish_frame_49.png");
	cache->addSpriteFramesWithFile("fish_frame_244.plist", "fish_frame_244.png");
	cache->addSpriteFramesWithFile("fish_frame_262.plist", "fish_frame_262.png");
	cache->addSpriteFramesWithFile("fish_frame_97.plist", "fish_frame_97.png");
	cache->addSpriteFramesWithFile("fish_frame_31.plist", "fish_frame_31.png");
	cache->addSpriteFramesWithFile("fish_frame_33.plist", "fish_frame_33.png");
	cache->addSpriteFramesWithFile("fish_frame_61.plist", "fish_frame_61.png");
	cache->addSpriteFramesWithFile("fish_frame_7.plist", "fish_frame_7.png");
	cache->addSpriteFramesWithFile("fish_frame_34.plist", "fish_frame_34.png");
	cache->addSpriteFramesWithFile("fish_frame_79.plist", "fish_frame_79.png");
	cache->addSpriteFramesWithFile("fish_frame_8.plist", "fish_frame_8.png");
	cache->addSpriteFramesWithFile("fish_frame_39.plist", "fish_frame_39.png");
	cache->addSpriteFramesWithFile("fish_frame_62.plist", "fish_frame_62.png");
	cache->addSpriteFramesWithFile("fish_frame_9.plist", "fish_frame_9.png");
	cache->addSpriteFramesWithFile("fish_frame_36.plist", "fish_frame_36.png");
	cache->addSpriteFramesWithFile("fish_frame_103.plist", "fish_frame_103.png");
	cache->addSpriteFramesWithFile("fish_frame_10.plist", "fish_frame_10.png");
	cache->addSpriteFramesWithFile("fish_frame_37.plist", "fish_frame_37.png");
	cache->addSpriteFramesWithFile("fish_frame_11.plist", "fish_frame_11.png");
	cache->addSpriteFramesWithFile("fish_frame_12.plist", "fish_frame_12.png");
	cache->addSpriteFramesWithFile("fish_frame_106.plist", "fish_frame_106.png");
	cache->addSpriteFramesWithFile("fish_frame_13.plist", "fish_frame_13.png");
	cache->addSpriteFramesWithFile("fish_frame_66.plist", "fish_frame_66.png");
	cache->addSpriteFramesWithFile("fish_frame_14.plist", "fish_frame_14.png");
	cache->addSpriteFramesWithFile("fish_frame_41.plist", "fish_frame_41.png");
	cache->addSpriteFramesWithFile("fish_frame_17.plist", "fish_frame_17.png");
	cache->addSpriteFramesWithFile("fish_frame_15.plist", "fish_frame_15.png");
	cache->addSpriteFramesWithFile("fish_frame_42.plist", "fish_frame_42.png");
	cache->addSpriteFramesWithFile("fish_frame_69.plist", "fish_frame_69.png");
	cache->addSpriteFramesWithFile("fish_frame_16.plist", "fish_frame_16.png");
	cache->addSpriteFramesWithFile("fish_frame_43.plist", "fish_frame_43.png");
	cache->addSpriteFramesWithFile("fish_frame_70.plist", "fish_frame_70.png");
	cache->addSpriteFramesWithFile("fish_frame_19.plist", "fish_frame_19.png");
	cache->addSpriteFramesWithFile("fish_frame_20.plist", "fish_frame_20.png");
	cache->addSpriteFramesWithFile("fish_frame_210.plist", "fish_frame_210.png");
	cache->addSpriteFramesWithFile("fish_frame_22.plist", "fish_frame_22.png");
	cache->addSpriteFramesWithFile("fish_frame_51.plist", "fish_frame_51.png");
	cache->addSpriteFramesWithFile("fish_frame_353.plist", "fish_frame_353.png");
	cache->addSpriteFramesWithFile("task_anim_5_3.plist", "task_anim_5_3.png");
}
//添加鱼的倍率界面
void PublicLayer::addFishRateAnimation()
{
	auto pPanel = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 1781);
	//滑动面板
	for (int i = 0; i < 41; i++)
	{
		string fishName = d_fishName[i][0];
		int nFrameNum = getIntFromS(d_fishName[i][1].c_str());

		auto ani = Animation::create();
		for (int j = 0; j < nFrameNum; j++)
		{
			string strPath = fishName + __String::createWithFormat("%02d.png", j + 1)->getCString();
			auto pFrame = SpriteFrameCache::getInstance()->spriteFrameByName(strPath);
			ani->addSpriteFrame(pFrame);
		}

		ani->setDelayPerUnit(0.1);
		ani->setLoops(-1);

		auto pRate = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, 1800+i);

		auto sp = Sprite::create();
		//sp->setPosition(Vec2(110*(i%5) + 60, i/5 * 120 + 60));
		sp->setPosition(pRate->getPosition() + Vec2(0, 40));
		sp->setScale(m_aFishSale[i]);
		sp->setRotation(-90);
		sp->runAction(Animate::create(ani));

		pPanel->addChild(sp);
	}
}

/********话费兑换界面*******/
//兑换按钮
void PublicLayer::callBackExchange(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		pu_music->playSound(13);
		this->showFareNotEnoughLayer();
	}
}
/********礼包界面*******/
bool PublicLayer::getIsShowGiftLayer()
{
	Layout* pGift = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 2596);

	return pGift->getScale() <= 0 ? false : true;
}
bool PublicLayer::getIsShowShopLayer()
{
	Layout* pShop = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 19);

	return pShop->getScale() <= 0 ? false : true;
}
bool PublicLayer::getIsShowFreeGoldLayer()
{
	Layout* pShop = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 8793);

	if(pShop->isVisible() && pShop->getScale() > 0)
	{
		return true;
	}
	return false;
}

/********VIP界面*******/
//充值按钮
void PublicLayer::callBackRecharge(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		pu_music->playSound(13);
		this->showShopLayer();
		Manager::getInstance()->setOpenFromVIP(true);
	}
}

/********获得宝箱界面*******/
//打开宝箱的按钮
void PublicLayer::callBackOpenBox(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		pu_music->playSound(13);
		//添加粒子
		ParticleSystemQuad* pBoom = ParticleSystemQuad::create("particle/gold_particle3.plist");
		pBoom->setPosition(Vec2(size.width / 2.0f, size.height / 3.0f * 2.0f));
		this->addChild(pBoom, 10000);
		MusicData::getInstance()->playSound(23);

		if(Manager::getInstance()->getRoomNum() == 1)
		{
			int nCurVIP = UserData::getInstance()->getCurVipLevel();
			float fPercentage = 0.0f;
			if(nCurVIP == 1)
				fPercentage = 0.1f;
			else if(nCurVIP == 2)
				fPercentage = 0.2f;
			else if(nCurVIP == 3)
				fPercentage = 0.3f;

			int nCurGold = 0;
			if(CCRANDOM_0_1() < fPercentage)
			{
				nCurGold = Manager::getInstance()->getPoolGoldNum() * 0.0015f;
			}
			else
			{
				nCurGold = 1000;
			}
			//在这里处理打开宝箱的逻辑
			UserData::getInstance()->increaseGoldNum(nCurGold);
			Manager::getInstance()->setPoolGoldNum(0);
			this->hideLayer();
			//获取东西
			this->showPublicSureLayer("open_box", nCurGold);
		}
		else if(Manager::getInstance()->getRoomNum() == 2)
		{
			int nNum = Manager::getInstance()->getManyPoolGoldNum();
			Manager::getInstance()->setManyPoolGoldNum(0);
			UserData::getInstance()->increaseGoldNum(nNum);
			//Manager::getInstance()->setTaskOver(true);
			this->hideLayer();
			this->showPublicSureLayer("open_box", nNum);
		}
		Manager::getInstance()->setIsCoinChange(true);
	}
}

/*******免费领取10000金币界面*******/
//领取按钮
void PublicLayer::callBackReceiveGreenHandGold(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		UserData::getInstance()->setFirstEnterGame(1);
		UserData::getInstance()->increaseGoldNum(10000);
		UserData::getInstance()->save();
		ParticleSystemQuad* pBoom = ParticleSystemQuad::create("particle/gold_particle0.plist");
		pBoom->setPosition(Vec2(size.width / 2.0f, size.height / 3.0f * 2.0f));
		this->addChild(pBoom, COIN_zOrder + 10000);
		m_pDelegateGame->refreshGoldAndDiamonds();

		this->hideLayer();
	}
}


//其他函数
void PublicLayer::callBackRemove(Node* pNode)
{
	pNode->removeFromParent();
}
void PublicLayer::callBackRemoveCoins(Node* pNode)
{
	Layout* pShiled = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 799);
	pShiled->removeChildByTag(kTagCoinsLayer);
	int nStage = Manager::getInstance()->getOne_LevelNum();
	int nGold = nStage*500+1000;
	ui::Text* text_coins = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 1512);
	text_coins->setString(getStringFromInt(nGold));
	this->unschedule(schedule_selector(PublicLayer::dropCoins));
	//继续
	ui::MyButton* buttonGoon = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 1514);
	buttonGoon->setVisible(true);
	//下一关
	ui::MyButton* buttonNext = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 1513);
	buttonNext->setVisible(true);
}
//公共关闭按钮
void PublicLayer::callBackCommonClose(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		MusicData::getInstance()->playSound(13);
		ui::MyButton* pButton = (ui::MyButton*)pObject;
		if(pButton->getTag() == 346)
		{
			if(!Manager::getInstance()->getOpenFreeGoldFromSign())
				this->showFreeGoldLayer();
			else
				Manager::getInstance()->setOpenFreeGoldFromSign(false);
		}
		else if(pButton->getTag() == 562)
		{
			if((this->getOpenInGame() && !this->getOpenOnUnenough()) || m_bDirectClose)
			{
				m_bDirectClose = false;
				this->hideLayer();
			}
			else
			{
				this->showFreeGoldLayer();
			}
		}
		else if(pButton->getTag() == 18)
		{
			if(m_bOpenOnUnenough)
			{
				this->showSwitchGunLayer();
				m_bOpenOnUnenough = false;
			}
			else
			{
				this->hideLayer();
			}
		}
		else if(pButton->getTag() == 579)
		{
			if(m_bOpenInHelp)
			{
				this->showHelpLayer();
			}
			else
			{
				this->hideLayer();
			}
		}
		else if(pButton->getTag() == 1763 || pButton->getTag() == 1550)
		{
			this->showHelpLayer();
		}
		else if(pButton->getTag() == 26)
		{
			if (Manager::getInstance()->getOpenFromVIP())
			{
				this->showVIPLayer();
				Manager::getInstance()->setOpenFromVIP(false);
			}
			else
			{
				this->hideLayer();
			}
		}
		else
		{
			this->hideLayer();
		}
	}
}
void PublicLayer::callBackCommonClose1(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		MusicData::getInstance()->playSound(13);
		MyButton* pButton = (MyButton*)pObject;
		if(pButton->getTag() == 1220)
		{
			Layout* pLayer = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1218);
			//pLayer->setVisible(false);
			auto seq = Sequence::create(ScaleTo::create(0.2, 0),
				CallFuncN::create(CC_CALLBACK_1(PublicLayer::callBackHideButton, this)),
				NULL);
			pLayer->runAction(seq);
		}
		//公共屏蔽层
		Layout* pShiled = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1252);
		pShiled->setVisible(false);
		pShiled->setTouchEnabled(false);
	}
}
void PublicLayer::callBackCommonClose2(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		MusicData::getInstance()->playSound(13);
		Layout* pShiled = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1253);
		pShiled->setVisible(false);
		pShiled->setTouchEnabled(false);
	}
}
//全部隐藏掉
void PublicLayer::hideLayer()
{
	auto seq = Sequence::create(ScaleTo::create(0.2, 0),
		CallFuncN::create(CC_CALLBACK_1(PublicLayer::callBackHideButton, this)),
		NULL);

	Layout* pFreeGold = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 10);
	pFreeGold->setTouchEnabled(false);
	if(pFreeGold->isVisible())
	{
		pFreeGold->runAction(seq);
	}

	Layout* pExchange = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 2313);
	pExchange->setTouchEnabled(false);
	if(pExchange->isVisible())
	{
		pExchange->runAction(seq);
	}

	Layout* pShop = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 19);
	pShop->setTouchEnabled(false);
	if(pShop->isVisible())
	{
		pShop->runAction(seq);
	}
	
	Layout* pDiamondShop = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 4719);
	pDiamondShop->setTouchEnabled(false);
	if(pDiamondShop->isVisible())
	{
		pDiamondShop->runAction(seq);
	}

	Layout* pUnlock = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 98);
	pUnlock->setTouchEnabled(false);
	if(pUnlock->isVisible())
	{
		pUnlock->runAction(seq);
		m_bOpenOnUnenough = false;
	}

	Layout* pSign = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 276);
	pSign->setTouchEnabled(false);
	if(pSign->isVisible())
	{
		pSign->runAction(seq);
	}

	Layout* pCollectCoins = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 508);
	pCollectCoins->setTouchEnabled(false);
	if(pCollectCoins->isVisible())
	{
		pCollectCoins->runAction(seq);
	}

	Layout* pConfigure = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 577);
	pConfigure->setTouchEnabled(false);
	if(pConfigure->isVisible())
	{
		pConfigure->runAction(seq);
	}

	Layout* pTask = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1470);
	pTask->setTouchEnabled(false);
	if(pTask->isVisible())
	{
		pTask->runAction(seq);
	}

	Layout* pCross = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1506);
	pCross->setTouchEnabled(false);
	if(pCross->isVisible())
	{
		pCross->runAction(seq);
	}

	Layout* pHelp = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1539);
	pHelp->setTouchEnabled(false);
	if(pHelp->isVisible())
	{
		pHelp->runAction(seq);
		m_bOpenInHelp = false;
	}

	Layout* pSpecial = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1547);
	pSpecial->setTouchEnabled(false);
	if(pSpecial->isVisible())
	{
		pSpecial->runAction(seq);
	}

	Layout* pFishRate = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1751);
	pFishRate->setTouchEnabled(false);
	if(pFishRate->isVisible())
	{
		pFishRate->runAction(seq);
	}

	Layout* pUpgrade = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 4789);
	pUpgrade->setTouchEnabled(false);
	if(pUpgrade->isVisible())
	{
		pUpgrade->runAction(seq);
	}

	Layout* pGift = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 2596);
	pGift->setTouchEnabled(false);
	if(pGift->isVisible())
	{
		pGift->runAction(seq);
	}

	Layout* pMonthCard = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 4100);
	pMonthCard->setTouchEnabled(false);
	if(pMonthCard->isVisible())
	{
		pMonthCard->runAction(seq);
	}

	Layout* pMonthCardReceive = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 4657);
	pMonthCardReceive->setTouchEnabled(false);
	if(pMonthCardReceive->isVisible())
	{
		pMonthCardReceive->runAction(seq);
	}

	Layout* pVIP = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 4206);
	pVIP->setTouchEnabled(false);
	if(pVIP->isVisible())
	{
		pVIP->runAction(seq);
	}

	Layout* pGetBox = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 5840);
	pGetBox->setTouchEnabled(false);
	if(pGetBox->isVisible())
	{
		pGetBox->runAction(seq);
	}

	Layout* pService = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 7579);
	pService->setTouchEnabled(false);
	if(pService->isVisible())
	{
		pService->runAction(seq);
	}

	Layout* pExchangeGold = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 7599);
	pExchangeGold->setTouchEnabled(false);
	if(pExchangeGold->isVisible())
	{
		pExchangeGold->runAction(seq);
	}

	Layout* pFirstGift = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 8744);
	pFirstGift->setTouchEnabled(false);
	if(pFirstGift->isVisible())
	{
		pFirstGift->runAction(seq);
	}

	Layout* pUnenough = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 8793);
	pUnenough->setTouchEnabled(false);
	if(pUnenough->isVisible())
	{
		pUnenough->runAction(seq);
	}

	Layout* pGoldGift = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 15453);
	pGoldGift->setTouchEnabled(false);
	if(pGoldGift->isVisible())
	{
		pGoldGift->runAction(seq);
	}

	Layout* pShiled = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 799);
	pShiled->setVisible(false);
	pShiled->setTouchEnabled(false);
	pShiled->setZOrder(0);

}
//设置界面可见
void PublicLayer::setLayerVisible(int nID, bool bScale)
{
	Layout* pFreeGold = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 10);
	pFreeGold->setVisible(false);
	pFreeGold->setTouchEnabled(false);
	pFreeGold->setZOrder(2);

	Layout* pExchange = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 2313);
	pExchange->setVisible(false);
	pExchange->setTouchEnabled(false);
	pExchange->setZOrder(2);

	Layout* pShop = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 19);
	pShop->setVisible(false);
	pShop->setTouchEnabled(false);
	pShop->setZOrder(2);

	Layout* pDiamondShop = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 4719);
	pDiamondShop->setVisible(false);
	pDiamondShop->setTouchEnabled(false);
	pDiamondShop->setZOrder(2);

	Layout* pUnlock = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 98);
	pUnlock->setVisible(false);
	pUnlock->setTouchEnabled(false);
	pUnlock->setZOrder(2);

	Layout* pSign = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 276);
	pSign->setVisible(false);
	pSign->setTouchEnabled(false);
	pSign->setZOrder(2);

	Layout* pCollectCoins = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 508);
	pCollectCoins->setVisible(false);
	pCollectCoins->setTouchEnabled(false);
	pCollectCoins->setZOrder(2);
	pCollectCoins->setAnchorPoint(Vec2(0.5, 0.5));

	Layout* pConfigure = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 577);
	pConfigure->setVisible(false);
	pConfigure->setTouchEnabled(false);
	pConfigure->setZOrder(2);
	pConfigure->setAnchorPoint(Vec2(0.5, 0.5));

	Layout* pTask = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1470);
	pTask->setVisible(false);
	pTask->setTouchEnabled(false);
	pTask->setZOrder(2);

	Layout* pHelp = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1539);
	pHelp->setVisible(false);
	pHelp->setTouchEnabled(false);
	pHelp->setZOrder(2);

	Layout* pSpecial = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1547);
	pSpecial->setVisible(false);
	pSpecial->setTouchEnabled(false);
	pSpecial->setZOrder(2);

	Layout* pFishRate = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1751);
	pFishRate->setVisible(false);
	pFishRate->setTouchEnabled(false);
	pFishRate->setZOrder(2);

	Layout* pUpgrade = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 4789);
	pUpgrade->setVisible(false);
	pUpgrade->setTouchEnabled(false);
	pUpgrade->setZOrder(2);

	Layout* pGift = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 2596);
	pGift->setVisible(false);
	pGift->setTouchEnabled(false);
	pGift->setZOrder(2);

	Layout* pMonthCard = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 4100);
	pMonthCard->setVisible(false);
	pMonthCard->setTouchEnabled(false);
	pMonthCard->setZOrder(2);

	Layout* pMonthCardReceive = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 4657);
	pMonthCardReceive->setVisible(false);
	pMonthCardReceive->setTouchEnabled(false);
	pMonthCardReceive->setZOrder(2);

	Layout* pVIP = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 4206);
	pVIP->setVisible(false);
	pVIP->setTouchEnabled(false);
	pVIP->setZOrder(2);

	Layout* pGetBox = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 5840);
	pGetBox->setVisible(false);
	pGetBox->setTouchEnabled(false);
	pGetBox->setZOrder(2);

	Layout* pService = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 7579);
	pService->setVisible(false);
	pService->setTouchEnabled(false);
	pService->setZOrder(2);

	Layout* pExchangeGold = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 7599);
	pExchangeGold->setVisible(false);
	pExchangeGold->setTouchEnabled(false);
	pExchangeGold->setZOrder(2);

	Layout* pFirstGift = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 8744);
	pFirstGift->setVisible(false);
	pFirstGift->setTouchEnabled(false);
	pFirstGift->setZOrder(2);

	Layout* pUnenough = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 8793);
	pUnenough->setVisible(false);
	pUnenough->setTouchEnabled(false);
	pUnenough->setZOrder(2);

	Layout* pGoldGift = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 15453);
	pGoldGift->setVisible(false);
	pGoldGift->setTouchEnabled(false);
	pGoldGift->setZOrder(2);

	auto seq = Sequence::createWithTwoActions(ScaleTo::create(0.2, 1.1), ScaleTo::create(0.1, 1.0));
	if(nID == LAYER_FREEGOLD)
	{
		pFreeGold->setVisible(true);
		pFreeGold->setZOrder(100);
		if(bScale)
		{
			pFreeGold->setScale(0.0);
			pFreeGold->runAction(seq);
		}
	}
	else if(nID == LAYER_EXCHANGE)
	{
		pExchange->setVisible(true);
		pExchange->setZOrder(100);
		if(bScale)
		{
			pExchange->setScale(0.0);
			pExchange->runAction(seq);
		}
	}
	else if(nID == LAYER_SHOP)
	{
		pShop->setVisible(true);
		pShop->setZOrder(100);
		if(bScale)
		{
			pShop->setScale(0.0);
			pShop->runAction(seq);
		}
	}
	else if(nID == LAYER_SHOP_DIAMOND)
	{
		pDiamondShop->setVisible(true);
		pDiamondShop->setZOrder(100);
		if(bScale)
		{
			pDiamondShop->setScale(0.0);
			pDiamondShop->runAction(seq);
		}
	}
	else if(nID == LAYER_SWITCHGUN)
	{
		pUnlock->setVisible(true);
		pUnlock->setZOrder(100);
		if(bScale)
		{
			pUnlock->setScale(0.0);
			pUnlock->runAction(seq);
		}
	}
	else if(nID == LAYER_SINGN)
	{
		pSign->setVisible(true);
		pSign->setZOrder(100);
		if(bScale)
		{
			pSign->setScale(0.0);
			pSign->runAction(seq);
		}
	}
	else if(nID == LAYER_AWARD)
	{
		pCollectCoins->setVisible(true);
		pCollectCoins->setZOrder(100);
		if(bScale)
		{
			pCollectCoins->setScale(0.0);
			pCollectCoins->runAction(seq);
		}
	}
	else if(nID == LAYER_CONFIGURE)
	{
		pConfigure->setVisible(true);
		pConfigure->setZOrder(100);
		if(bScale)
		{
			pConfigure->setScale(0.0);
			pConfigure->runAction(seq);
		}
	}
	else if(nID == LAYER_TASK)
	{
		pTask->setVisible(true);
		pTask->setZOrder(100);
		if(bScale)
		{
			pTask->setScale(0.0);
			pTask->runAction(seq);
		}
	}
	else if(nID == LAYER_SPECIAL)
	{
		pSpecial->setVisible(true);
		pSpecial->setZOrder(100);
		if(bScale)
		{
			pSpecial->setScale(0.0);
			pSpecial->runAction(seq);
		}
	}
	else if(nID == LAYER_FISHRATE)
	{
		pFishRate->setVisible(true);
		pFishRate->setZOrder(100);
		if(bScale)
		{
			pFishRate->setScale(0.0);
			pFishRate->runAction(seq);
		}
	}
	else if(nID == LAYER_HELP)
	{
		pHelp->setVisible(true);
		pHelp->setZOrder(100);
		if(bScale)
		{
			pHelp->setScale(0.0);
			pHelp->runAction(seq);
		}
	}
	else if(nID == LAYER_UPGRADE)
	{
		pUpgrade->setVisible(true);
		pUpgrade->setZOrder(100);
		if(bScale)
		{
			pUpgrade->setScale(0.0);
			pUpgrade->runAction(seq);
		}
	}
	else if(nID == LAYER_GIFT)
	{
		pGift->setVisible(true);
		pGift->setZOrder(100);
		if(bScale)
		{
			pGift->setScale(0.0);
			pGift->runAction(seq);
		}
	}
	else if(nID == LAYER_MONTHCARD)
	{
		pMonthCard->setVisible(true);
		pMonthCard->setZOrder(100);
		if(bScale)
		{
			pMonthCard->setScale(0.0);
			pMonthCard->runAction(seq);
		}
	}
	else if(nID == LAYER_MONTHCARD_RECEIVE)
	{
		pMonthCardReceive->setVisible(true);
		pMonthCardReceive->setZOrder(100);
		if(bScale)
		{
			pMonthCardReceive->setScale(0.0);
			pMonthCardReceive->runAction(seq);
		}
	}
	else if(nID == LAYER_VIP)
	{
		pVIP->setVisible(true);
		pVIP->setZOrder(100);
		if(bScale)
		{
			pVIP->setScale(0.0);
			pVIP->runAction(seq);
		}
	}
	else if(nID == LAYER_GETBOX)
	{
		pGetBox->setVisible(true);
		pGetBox->setZOrder(100);
		if(bScale)
		{
			pGetBox->setScale(0.0);
			pGetBox->runAction(seq);
		}
	}
	else if(nID == LAYER_SERVICE)
	{
		pService->setVisible(true);
		pService->setZOrder(100);
		if(bScale)
		{
			pService->setScale(0.0);
			pService->runAction(seq);
		}
	}
	else if(nID == LAYER_EXCHANGEGOLD)
	{
		pExchangeGold->setVisible(true);
		pExchangeGold->setZOrder(100);
		if(bScale)
		{
			pExchangeGold->setScale(0.0);
			pExchangeGold->runAction(seq);
		}
	}
	else if(nID == LAYER_FIRSTGIFT)
	{
		pFirstGift->setVisible(true);
		pFirstGift->setZOrder(100);
		if(bScale)
		{
			pFirstGift->setScale(0.0);
			pFirstGift->runAction(seq);
		}
	}
	else if(nID == LAYER_UNENOUGH)
	{
		pUnenough->setVisible(true);
		pUnenough->setZOrder(100);
		if(bScale)
		{
			pUnenough->setScale(0.0);
			pUnenough->runAction(seq);
		}
	}
	else if(nID == LAYER_GOLDGIFT)
	{
		pGoldGift->setVisible(true);
		pGoldGift->setZOrder(100);
		if(bScale)
		{
			pGoldGift->setScale(0.0);
			pGoldGift->runAction(seq);
		}
	}
}
//出场完毕回调
void PublicLayer::callBackShowFinish(Node* pNode)
{
	m_bMoving = false;
}
//隐藏菜单按钮回调
void PublicLayer::callBackHide(Node* pNode)
{
	ui::MyButton* pButton = (ui::MyButton*)pNode;

	pButton->setVisible(false);

	m_bOpening = false;
	m_bMoving = false;
}
//添加屏蔽层
void PublicLayer::addShiledLayer()
{
	Layout* pShiled = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 799);
	pShiled->setVisible(true);
	pShiled->setTouchEnabled(true);
	//pShiled->setZOrder(99);
}
void PublicLayer::addShiled1Layer()
{
	Layout* pShiled = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1252);
	pShiled->setVisible(true);
	pShiled->setTouchEnabled(true);
	//pShiled->setZOrder(199);
}
void PublicLayer::addShiled2Layer()
{
	Layout* pShiled = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 1253);
	pShiled->setVisible(true);
	pShiled->setTouchEnabled(true);
	//pShiled->setZOrder(299);
}
//隐藏按钮回调visible(false)
void PublicLayer::callBackHideButton(Node* pNode)
{
	auto pButton = (MyButton*)pNode;

	pButton->setVisible(false);
}
//隐藏图层回调visible(false)
void PublicLayer::callBackHideLayerOut(Node* pNode)
{
	Layout* pLayerOut = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 935);
	pLayerOut->setVisible(false);
}
//添加按钮特效
void PublicLayer::addButtonEffect(Node* pNode)
{

	auto pCatch = AnimationCache::getInstance();
	if(!pCatch->animationByName("skill_ani"))
	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/skill/skill_ani.plist");
		//获取动画帧数和缓存中的标记
		Vector<SpriteFrame*> array1(12);

		for (int i=1; i <= 12; i++)
		{
			SpriteFrameCache *pcache = SpriteFrameCache::getInstance();
			std::string format = "skill_anim_%02d.png";
			__String* strName = __String::createWithFormat(format.c_str(), i);
			SpriteFrame* pframe = pcache->getSpriteFrameByName(strName->getCString());
			array1.pushBack(pframe);
		}

		Animation *panimation = Animation::createWithSpriteFrames(array1, 0.1);
		AnimationCache::getInstance()->addAnimation(panimation, "skill_ani");
	}
	
	auto pAni = pCatch->animationByName("skill_ani");

	auto pSprite = Sprite::create();
	pSprite->setPosition(Vec2(pNode->getContentSize().width / 2 + 5, pNode->getContentSize().height / 2 + 5));
	pNode->addChild(pSprite);

	auto seq = Sequence::create(Animate::create(pAni), 
		CallFuncN::create(CC_CALLBACK_1(PublicLayer::callBackRemove, this)),
		NULL);
	pSprite->runAction(seq);
	return;
}
void PublicLayer::update(float dt)
{
	if(m_nEffectIndex%7 == 1)
	{
		ui::MyButton * buttonStopFish = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 694);
		this->addButtonEffect(buttonStopFish);
	}
	else if(m_nEffectIndex%7 == 2)
	{
		ui::MyButton * buttonSpeedUp = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 695);
		this->addButtonEffect(buttonSpeedUp);
	}
	else if(m_nEffectIndex%7 == 3)
	{
		ui::MyButton * buttonRage = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 696);
		this->addButtonEffect(buttonRage);
	}
	else if(m_nEffectIndex%7 == 4)
	{
		ui::MyButton * buttonLockFish = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 697);
		this->addButtonEffect(buttonLockFish);
	}
	else if(m_nEffectIndex%7 == 5)
	{
		ui::MyButton * buttonTenThousand = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 698);
		this->addButtonEffect(buttonTenThousand);
	}
	
	m_nEffectIndex ++;
}
void PublicLayer::updateMessageBar(float dt)
{
	if(m_nMessageIndex > CCRANDOM_0_1()*30 + 30)
	{
		this->showMessageBarLayer();
		m_nMessageIndex = 0;
	}

	m_nMessageIndex ++;
}

void PublicLayer::graySprite(Node * sprite)  
{  
	if(sprite)
	{
		GLProgram * p = new GLProgram();
		p->initWithFilenames("ui/shader/gray.vsh", "ui/shader/gray.fsh");
		p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
		p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
		p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
		p->link();
		p->updateUniforms();
		sprite->setShaderProgram(p);
	}  
}
//更新领奖时间
void PublicLayer::updateRewardTime(float dt)
{
	//领取金币的更新
	int nCurIndex = getIndexByCurTime();
	if((m_nReceivedCoinIndex < nCurIndex) || (m_nReceivedCoinIndex == 11 && nCurIndex == 0))
	{
		ui::MyButton * buttonReceive = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 564);
		if(!buttonReceive->isVisible())
		{
			buttonReceive->setVisible(true);
		}
		__String* info = m_pXmlParser->getString("get_enable");
		ui::Text* pTime1 = (ui::Text *)ui::Helper::seekWidgetByTag(m_pUINode, 932);
		pTime1->setString(info->getCString());

		ActionManagerEx::getInstance()->playActionByName("NewUi_1.json" , "Animation0");
	}
	else
	{
		ui::MyButton* buttonReceive = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 564);
		if(buttonReceive->isVisible())
		{
			buttonReceive->setVisible(false);
		}
		ui::Text* pTime = (ui::Text *)ui::Helper::seekWidgetByTag(m_pUINode, 1207);
		tm* stm = GetTime1();

		int nNextHour = getTimeByCurIndex(nCurIndex + 1);
		int nHour = (nNextHour - stm->tm_hour - 1)%24;
		int nMin = 59 - stm->tm_min;	
		int nSec = 60 - stm->tm_sec;
		
		std::string str = "%02d:%02d:%02d";
		__String* pFormat = __String::createWithFormat(str.c_str(), nHour, nMin, nSec);
		pTime->setString(pFormat->getCString());

		ui::Text* pTime1 = (ui::Text *)ui::Helper::seekWidgetByTag(m_pUINode, 932);
		pTime1->setString(pFormat->getCString());
	}

	//单人场的刷新
	if(Manager::getInstance()->getRoomNum() == 1)
	{
		bool m_bInterNet = Manager::getInstance()->getInternet();
		Manager::getInstance()->setPoolGoldNum(Manager::getInstance()->getPoolGoldNum() + rand() % 50000 + 20000);
		unsigned int nNum = Manager::getInstance()->getPoolGoldNum();
		//全民彩金的刷新
		//奖金池金币数
		ui::Text* pPoolGold = (ui::Text *)ui::Helper::seekWidgetByTag(m_pUINode, 5837);
		if(m_bInterNet)
			pPoolGold->setString(getStringFromInt(nNum));
		else
			pPoolGold->setString(m_pXmlParser->getString("net_no")->getCString());
		tm* stm = GetTime1();
		int nMin = 59 - stm->tm_min;	
		int nSec = 60 - stm->tm_sec;
		std::string str = "%02d:%02d";
		__String* pFormat = __String::createWithFormat(str.c_str(), nMin, nSec);
		//时间
		ui::Text* pPoolTime = (ui::Text *)ui::Helper::seekWidgetByTag(m_pUINode, 5839);
		if(m_bInterNet)
			pPoolTime->setString(pFormat->getCString());
		else
			pPoolTime->setString("");

		if(nMin == 0 && nSec == 1 && m_bInterNet)
		{
			Manager::getInstance()->setReleaseBoxFish(true);
		}
	}
	else if(Manager::getInstance()->getRoomNum() == 2)
	{
		if(!m_pDelegateGame)return;
		unsigned int nNum = Manager::getInstance()->getManyPoolGoldNum();
		//奖金池金币数
		ui::Text* pPoolGold = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 5848);
		int nNum1 = m_pDelegateGame->getCurrentFinishNum(0);
		int nNum2 = m_pDelegateGame->getCurrentFinishNum(1);
		int nNum3 = m_pDelegateGame->getCurrentFinishNum(2);
		//刷新当前完成的数量
		ui::Text* pCurNum1 = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 6304);
		pCurNum1->setString(getStringFromInt(nNum1));
		ui::Text* pCurNum2 = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 6305);
		pCurNum2->setString(getStringFromInt(nNum2));
		ui::Text* pCurNum3 = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 6306);
		pCurNum3->setString(getStringFromInt(nNum3));

		pPoolGold->setString(getStringFromInt(nNum));
		
		//刷新剩余时间
		tm* stm = GetTime1();
		int nMin = (59 - stm->tm_min)%20;	
		int nSec = 60 - stm->tm_sec;
		std::string str = "%02d:%02d";
		__String* pFormat = __String::createWithFormat(str.c_str(), nMin, nSec);
		//时间
		ui::Text* pPoolTime = (ui::Text*)ui::Helper::seekWidgetByTag(m_pUINode, 5854);
		pPoolTime->setString(pFormat->getCString());
		if(Manager::getInstance()->getTaskOver())
		{
			pPoolGold->setString(m_pXmlParser->getString("reset_task")->getCString());
		}

		if(nMin == 0 && nSec == 1)
		{
			Manager::getInstance()->setTaskOver(false);
			m_pDelegateGame->resetManyTask();
			this->refreshManyTask();
		}
	}
}
//通过当前时间获取相应的下标
int PublicLayer::getIndexByCurTime()
{
	int nIndex = 0;
	tm* pTime = GetTime1();
	int nHour = pTime->tm_hour;

	if(nHour < 11 && nHour >= 9)
		nIndex = 0;
	else if(nHour < 12 && nHour >= 11)
		nIndex = 1;
	else if(nHour < 14 && nHour >= 12)
		nIndex = 2;
	else if(nHour < 16 && nHour >= 14)
		nIndex = 3;
	else if(nHour < 18 && nHour >= 16)
		nIndex = 4;
	else if(nHour < 19 && nHour >= 18)
		nIndex = 5;
	else if(nHour < 20 && nHour >= 19)
		nIndex = 6;
	else if(nHour < 22 && nHour >= 20)
		nIndex = 7;
	else if(nHour < 23 && nHour >= 22)
		nIndex = 8;
	else if(nHour < 1 || nHour >= 23)
		nIndex = 9;
	else if(nHour < 3 && nHour >= 1)
		nIndex = 10;
	else if(nHour < 9 && nHour >= 3)
		nIndex = 11;


	return nIndex;
}

//通过当前时间下标获取能够领取的金币
int PublicLayer::getCoinsNumByCurIndex(int nIndex)
{
	int nCoinNum = 100;

	if(nIndex == 0)
		nCoinNum = 100;
	else if(nIndex == 1)
		nCoinNum = 100;
	else if(nIndex == 2)
		nCoinNum = 100;
	else if(nIndex == 3)
		nCoinNum = 100;
	else if(nIndex == 4)
		nCoinNum = 100;
	else if(nIndex == 5)
		nCoinNum = 100;
	else if(nIndex == 6)
		nCoinNum = 100;
	else if(nIndex == 7)
		nCoinNum = 100;
	else if(nIndex == 8)
		nCoinNum = 100;
	else if(nIndex == 9)
		nCoinNum = 100;
	else if(nIndex == 10)
		nCoinNum = 100;
	else if(nIndex == 11)
		nCoinNum = 100;
	
	return nCoinNum;
}

//通过当前时间下标获取具体时间
int PublicLayer::getTimeByCurIndex(int nIndex)
{
	int nHour = 0;

	if(nIndex == 0)
		nHour = 9;
	else if(nIndex == 1)
		nHour = 11;
	else if(nIndex == 2)
		nHour = 12;
	else if(nIndex == 3)
		nHour = 14;
	else if(nIndex == 4)
		nHour = 16;
	else if(nIndex == 5)
		nHour = 18;
	else if(nIndex == 6)
		nHour = 19;
	else if(nIndex == 7)
		nHour = 20;
	else if(nIndex == 8)
		nHour = 22;
	else if(nIndex == 9)
		nHour = 23;
	else if(nIndex == 10)
		nHour = 25;
	else if(nIndex == 11)
		nHour = 27;
	else
		nHour = 9;

	return nHour;
}
//获取当前关的任务完成情况
int PublicLayer::getTaskIndex(int nStage)
{
	int nID = 0;
	if(nStage < 1)
	{
		return 0;
	}

	UserDataMission* pData = UserData::getInstance()->getUserDataMissionByID(nStage);

	int nFishAim1 = pData->getFishNum();
	int nFishCur1 = pData->getFinishNum();
	int nFishAim2 = pData->getFishNum1();
	int nFishCur2 = pData->getFinishNum1();

	if(nFishCur1 >= nFishAim1)
	{
		nID = 1;
	}

	if(nFishCur2 >= nFishAim2)
	{
		nID = 2;
	}

	return nID;
}

//刷新任务显示
void PublicLayer::refreshTaskNum()
{
	//ooo
	int nStage = Manager::getInstance()->getOne_LevelNum();
	//左边的数量
	UserDataMission* pData = UserData::getInstance()->getUserDataMissionByID(nStage);

	int nFishAim1 = pData->getFishNum();
	int nFishCur1 = pData->getFinishNum();
	int nFishID1 = pData->getFishID();
	int nFishAim2 = pData->getFishNum1();
	int nFishCur2 = pData->getFinishNum1();
	int nFishID2 = pData->getFishID1();

	if(nFishCur1 >= nFishAim1) nFishCur1 = nFishAim1;
	if(nFishCur2 >= nFishAim2) nFishCur2 = nFishAim2;
	std::string text = getStringFromInt(nFishCur1) + "/" + getStringFromInt(nFishAim1);
	ui::Text* pText1 = (ui::Text *)ui::Helper::seekWidgetByTag(m_pUINode, 1492);
	pText1->setString(text.c_str());

	text = getStringFromInt(nFishCur1) + "/" + getStringFromInt(nFishAim1);
	ui::Text* pText11 = (ui::Text *)ui::Helper::seekWidgetByTag(m_pUINode, 1476);
	pText11->setString(text.c_str());

	text = getStringFromInt(nFishCur2) + "/" + getStringFromInt(nFishAim2);
	ui::Text* pText2 = (ui::Text *)ui::Helper::seekWidgetByTag(m_pUINode, 1500);
	pText2->setString(text.c_str());

	text = getStringFromInt(nFishCur2) + "/" + getStringFromInt(nFishAim2);
	ui::Text* pText12 = (ui::Text *)ui::Helper::seekWidgetByTag(m_pUINode, 1477);
	pText12->setString(text.c_str());


	//任务鱼的显示
	string fishName = d_fishName[nFishID1][0];
	int nFrameNum = getIntFromS(d_fishName[nFishID1][1].c_str());
	auto ani = Animation::create();
	for (int j = 0; j < nFrameNum; j++)
	{
		string strPath = fishName + __String::createWithFormat("%02d.png", j + 1)->getCString();
		auto pFrame = SpriteFrameCache::getInstance()->spriteFrameByName(strPath);
		ani->addSpriteFrame(pFrame);
	}
	ani->setDelayPerUnit(0.1);
	ani->setLoops(-1);
	auto sp = Sprite::create();
	sp->setScale(m_aFishSale[nFishID1]);
	sp->setRotation(-90);
	auto pAni1 = Animate::create(ani);
	sp->runAction(pAni1);

	fishName = d_fishName[nFishID2][0];
	nFrameNum = getIntFromS(d_fishName[nFishID2][1].c_str());
	auto ani1 = Animation::create();
	for (int j = 0; j < nFrameNum; j++)
	{
		string strPath = fishName + __String::createWithFormat("%02d.png", j + 1)->getCString();
		auto pFrame = SpriteFrameCache::getInstance()->spriteFrameByName(strPath);
		ani1->addSpriteFrame(pFrame);
	}
	ani1->setDelayPerUnit(0.1);
	ani1->setLoops(-1);
	auto sp1 = Sprite::create();
	sp1->setScale(m_aFishSale[nFishID2]);
	sp1->setRotation(-90);
	auto pAni2 = Animate::create(ani1);
	sp1->runAction(pAni2);


	//左边任务一
	ui::Layout* pPanel1 = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 1490);
	ui::ImageView* pTaskFish1 = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 1493);
	if(pTaskFish1->isVisible())
	{
		sp->setPosition(pTaskFish1->getPosition());
		pPanel1->addChild(sp);
		pTaskFish1->setVisible(false);
	}
	//左边任务二
	ui::Layout* pPanel2 = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 1498);
	ui::ImageView* pTaskFish2 = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 1501);
	if(pTaskFish2->isVisible())
	{
		sp1->setPosition(pTaskFish2->getPosition());
		pPanel2->addChild(sp1);
		pTaskFish2->setVisible(false);
	}

	//中间任务一
	auto sp3 = Sprite::create();
	sp3->setScale(m_aFishSale[nFishID2]);
	sp3->setRotation(-90);
	sp3->runAction(pAni1->clone());

	ui::Layout* pPanel3 = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 1472);
	ui::ImageView* pTaskFish3 = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 1478);
	if(pTaskFish3->isVisible())
	{
		sp3->setPosition(pTaskFish3->getPosition());
		pPanel3->addChild(sp3);
		pTaskFish3->setVisible(false);
	}

	//中间任务二
	auto sp4 = Sprite::create();
	sp4->setScale(m_aFishSale[nFishID2]);
	sp4->setRotation(-90);
	sp4->runAction(pAni2->clone());
	ui::Layout* pPanel4 = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 1473);
	ui::ImageView* pTaskFish4 = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 1479);
	if(pTaskFish4->isVisible())
	{
		sp4->setPosition(pTaskFish4->getPosition());
		pPanel4->addChild(sp4);
		pTaskFish4->setVisible(false);
	}
}
//刷新menu的金币
void PublicLayer::refreshMenuGold()
{
	if(!this->getInGame())
	{
		MenuLayer* pLayer = (MenuLayer*)this->getParent();

		//更新金币
		int nCoins = UserData::getInstance()->getGoldNum();
		ui::TextAtlas* pTextCoins = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(pLayer->m_pUINode, 442);
		pTextCoins->setString(getStringFromInt(nCoins));
		//刷新钻石
		int nDiamonds = UserData::getInstance()->getDiamonds();
		ui::TextAtlas* pTextDiamonds = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(pLayer->m_pUINode, 869);
		pTextDiamonds->setString(getStringFromInt(nDiamonds));

		int nBuyGift = UserData::getInstance()->getBuyGift();
		if(nBuyGift >= 2)
		{
			ui::MyButton* buttonGratiaPackage = (ui::MyButton *)ui::Helper::seekWidgetByTag(pLayer->m_pUINode, 857);
			buttonGratiaPackage->setVisible(false);
		}
	}
	else
	{
		Manager::getInstance()->setIsCoinChange(true);
	}
}
//刷新当前VIP值
void PublicLayer::refreshCurrentVIP()
{
	//当前VIP显示
	ImageView* pVip0 = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 7056);
	ImageView* pVip1 = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 7057);
	ImageView* pVip2 = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 7058);
	ImageView* pVip3 = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 7059);
	pVip0->setVisible(false);
	pVip1->setVisible(false);
	pVip2->setVisible(false);
	pVip3->setVisible(false);
	int nCurVIP = UserData::getInstance()->getCurVipLevel();
	if(nCurVIP == 0)
		pVip0->setVisible(true);
	else if(nCurVIP == 1)
		pVip1->setVisible(true);
	else if(nCurVIP == 2)
		pVip2->setVisible(true);
	else if(nCurVIP == 3)
		pVip3->setVisible(true);
}

//刷新游戏中的经验值
void PublicLayer::refreshExperiencePoints(int nExpOld)
{
	ui::LoadingBar* pBar = (ui::LoadingBar*)Helper::seekWidgetByTag(m_pUINode, 700);
    
	int nExp = UserData::getInstance()->getExp_value();
	if(nExp > 1000000)
	{
		UserData::getInstance()->setExp_value(1000000);
	}
	
	int nLv = nExp/10000 + 1;
	if(nLv > 100)
	{
		nLv = 100;
	}
	int nOldLv = nExpOld/10000 + 1;
	float fPer = nExp%10000/10000.0f;
	pBar->setPercent(fPer*100);
    
	ui::TextAtlas* pText = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, 709);
	pText->setString(getStringFromInt(nLv));

	if(nOldLv < nLv)
	{
		this->showUpgradeLayer();
	}
}
//更新多人场的技能条
void PublicLayer::refreshSkillMneuPosition(int nPosID)
{
	//技能条的位置
	Layout* pSkill = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 1976);
	if(nPosID == 0 || nPosID == 1)
	{
		pSkill->runAction(MoveTo::create(0.0f, Vec2(427.0f, 40.0f)));
	}
	else
	{
		pSkill->runAction(MoveTo::create(0.0f, Vec2(427.0f, 430.0f)));
	}

	//奖金池的位置
	Layout* pPool = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 5845);
	Layout* pWords = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 5858);
	Layout* pTime = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 6303);
	Layout* pNum = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 5859);
	Layout* pAsk = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 6302);
	MyButton* pBack = (ui::MyButton*)ui::Helper::seekWidgetByTag(m_pUINode, 6308);
	ui::ImageView* pArrow = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 6682);
	if(nPosID == 2 || nPosID == 3)
	{
		pPool->runAction(MoveTo::create(0.0f, Vec2(427.0f, 87.0f)));
		pWords->runAction(MoveTo::create(0.0f, Vec2(0.0f, -61.0f)));
		pTime->runAction(MoveTo::create(0.0f, Vec2(0, -20.0f)));
		pNum->runAction(MoveTo::create(0.0f, Vec2(0, 15.0f)));
		pAsk->runAction(MoveTo::create(0.0f, Vec2(0, 51.0f)));
		pBack->runAction(RotateTo::create(0.0f, 180.0f));
		pBack->runAction(MoveTo::create(0.0f, Vec2(0, 0.0f)));
		pArrow->runAction(RotateTo::create(0.0f, 180.0f));
	}
	else
	{
		pPool->runAction(MoveTo::create(0.0f, Vec2(427.0f, 393.0f)));
		pWords->runAction(MoveTo::create(0.0f, Vec2(0.0f, 61.0f)));
		pTime->runAction(MoveTo::create(0.0f, Vec2(0, 20.0f)));
		pNum->runAction(MoveTo::create(0.0f, Vec2(0, -15.0f)));
		pAsk->runAction(MoveTo::create(0.0f, Vec2(0, -51.0f)));
		pBack->runAction(RotateTo::create(0.0f, 0.0f));
		pBack->runAction(MoveTo::create(0.0f, Vec2(0, 0.0f)));
		pArrow->runAction(RotateTo::create(0.0f, 180.0f));
	}
}

/********游戏引导*******/
//随机添加技能引导
void PublicLayer::addRandomSkillGuide()
{
	int nTag = 694;
	int nRand = rand() % 5;
	if(nRand == 0)
		nTag = 694;
	else if(nRand == 1)
		nTag = 695;
	else if(nRand == 2)
		nTag = 696;
	else if(nRand == 3)
		nTag = 697;
	else if(nRand == 4)
		nTag = 698;

	ui::MyButton* button = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, nTag);

	//添加箭头
	auto pArrow = Sprite::create("lock_arrow.png");
	pArrow->setAnchorPoint(Vec2(0.5, 1.0));
	pArrow->setPosition(Vec2(button->getContentSize().width / 2, -button->getContentSize().height + 20));
	button->addChild(pArrow, 0, kTagGuideArrow);
	pArrow->setScale(SCALE);
	pArrow->setRotation(180);

	//文字图片
	std::string strPath = __String::createWithFormat("ui/guide/words_%d.png", nRand + 1)->getCString();
	//strPath = "n_dis_3.png";
	auto pWords = Sprite::create(strPath.c_str());
	pWords->setAnchorPoint(Vec2(0.5, 1.0));
	pWords->setPosition(Vec2(pArrow->getContentSize().width / 2.0f * pArrow->getScale(), 80.0f));
	pArrow->addChild(pWords);
	pWords->setRotation(180);

	auto seq = Sequence::create(DelayTime::create(8.0), CallFuncN::create(CC_CALLBACK_1(PublicLayer::callBackRemove, this)), NULL);
	pArrow->runAction(seq);
	
	auto seq1 = Sequence::createWithTwoActions(MoveBy::create(0.5, Vec2(0, 15.0f)), MoveBy::create(0.5, Vec2(0, -15.0f)));
	pArrow->runAction(RepeatForever::create(seq1));
}
//更新引导
void PublicLayer::updateGuide(float dt)
{
	if(m_nGuideIndex > CCRANDOM_0_1()*30 + 10)
	{
		this->addRandomSkillGuide();
		m_nGuideIndex = 0;
	}

	m_nGuideIndex ++;
}/***********付费接口*************/
void PublicLayer::callbackConfigure(Node* target, void* data)
{
	log("---callbackConfigure----");
	__Dictionary* dic = (__Dictionary*)data;
	//用做统计
	int rmb = 0;
	
	if(!dic->objectForKey("fake_code"))
	{
		CCLog("buyMoneyFromJavaCallBack fake_code is null");
	}

	int paytype = dic->valueForKey("fake_code")->intValue();
	const char* tid = dic->valueForKey("tradeId")->getCString();
	int result = dic->valueForKey("result")->intValue();
	int bTest = dic->valueForKey("is_test")->intValue();
	int nNum = 0;
	int nDiamondNum = 0;
    
    if (result == 2) { // 客户端支付成功
        HttpInstance::getInstance()->setRewardStatus(REWARD_STATUS_WAIT);
        return;
    }
    else if(result == 1000) //当返回结果为1000，说明付费失败（客户端返回失败）
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		//发放完物品状态恢复
		HttpInstance::getInstance()->setRewardStatus(REWARD_STATUS_NONE);
		m_nCurrentRequestTimes = 0;
		if(HttpInstance::getInstance()->getPayType() == PAY_TYPE_THRAN)
		{
			log("lian zhong pay failed change to qifan pay");
			//联众直接恢复状态
			if(paytype == 7)
			{
				HttpInstance::getInstance()->setCouldPay(true);
				HttpInstance::getInstance()->setPayType(PAY_TYPE_THRAN);
				HttpInstance::getInstance()->setRewardStatus(REWARD_STATUS_NONE);
				m_nCurrentRequestTimes = 0;
			}
			else
			{
				HttpInstance::getInstance()->setPayType(PAY_TYPE_QF);
				this->requestRecharge1();
			}
		}
		else
		{
			log("qf pay failed end!!!");
			HttpInstance::getInstance()->setCouldPay(true);
			HttpInstance::getInstance()->setPayType(PAY_TYPE_THRAN);
			HttpInstance::getInstance()->setRewardStatus(REWARD_STATUS_NONE);
			m_nCurrentRequestTimes = 0;
		}
#endif
		return;
	}
    else if(result != 1)
	{
		log("----result-----(%d)---------", result);
		return;
	}

	//向服务器发送成功结果
	requestRecharge3();

	HttpInstance::getInstance()->setCouldPay(true);
	HttpInstance::getInstance()->setPayType(PAY_TYPE_THRAN);
	HttpInstance::getInstance()->setRewardStatus(REWARD_STATUS_NONE);
	m_nCurrentRequestTimes = 0;


	int bFirst = UserData::getInstance()->getFirstShopping();
	int nDouble = (bFirst == 0) ? 2 : 1;
	//VIP加成
	int nVip = UserData::getInstance()->getCurVipLevel();
	float fAdd = 1.0f;
	if(nVip == 0)
		fAdd = 1.0f;
	else if(nVip == 1)
		fAdd = 1.1f;
	else if(nVip == 2)
		fAdd = 1.2f;
	else if(nVip == 3)
		fAdd = 1.3f;
	switch (paytype)
	{
	case BUY_GOLD_6:
		nNum = 60000;
		rmb = 6;
		nNum *= nDouble;
		nNum = (float)nNum * fAdd;
		UserData::getInstance()->setFirstShopping(1);
		break;
	case BUY_GOLD_15:
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		nNum = 180000;
		rmb = 18;
#else
        nNum = 150000;
        rmb = 15;
#endif
		nNum *= nDouble;
		nNum = (float)nNum * fAdd;
		UserData::getInstance()->setFirstShopping(1);
		break;
	case BUY_GOLD_30:
		nNum = 300000;
		rmb = 30;
		nNum *= nDouble;
		nNum = (float)nNum * fAdd;
		UserData::getInstance()->setFirstShopping(1);
		break;
	case BUY_GOLD_50:
		nNum = 500000;
		rmb = 50;
		nNum *= nDouble;
		nNum = (float)nNum * fAdd;
		UserData::getInstance()->setFirstShopping(1);
		break;
	case BUY_GOLD_108:
		nNum = 1080000;
		rmb = 108;
		nNum *= nDouble;
		nNum = (float)nNum * fAdd;
		UserData::getInstance()->setFirstShopping(1);
		break;
	case BUY_GOLD_328:
		nNum = 3280000;
		rmb = 328;
		nNum *= nDouble;
		nNum = (float)nNum * fAdd;
		UserData::getInstance()->setFirstShopping(1);
		break;
	case BUY_GOLD_01:
		nNum = 120;
		rmb = 0;
		nNum *= nDouble;
		nNum = (float)nNum * fAdd;
		UserData::getInstance()->setFirstShopping(1);
		break;
	case BUY_DIAMOND_6:
		nDiamondNum = 600;
		rmb = 6;
		nDiamondNum = (float)nDiamondNum * fAdd;
		UserData::getInstance()->setFirstShopping(1);
		break;
	case BUY_DIAMOND_15:
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        nDiamondNum = 1800;
        rmb = 18;
#else
        nDiamondNum = 1500;
        rmb = 15;
#endif
		nDiamondNum = (float)nDiamondNum * fAdd;
		UserData::getInstance()->setFirstShopping(1);
		break;
	case BUY_DIAMOND_30:
		nDiamondNum = 3000;
		rmb = 30;
		nDiamondNum = (float)nDiamondNum * fAdd;
		UserData::getInstance()->setFirstShopping(1);
		break;
	case BUY_DIAMOND_50:
		nDiamondNum = 5000;
		rmb = 50;
		nDiamondNum = (float)nDiamondNum * fAdd;
		UserData::getInstance()->setFirstShopping(1);
		break;
	case BUY_DIAMOND_108:
		nDiamondNum = 10800;
		rmb = 108;
		nDiamondNum = (float)nDiamondNum * fAdd;
		UserData::getInstance()->setFirstShopping(1);
		break;
	case BUY_DIAMOND_328:
		nDiamondNum = 32800;
		rmb = 328;
		nDiamondNum = (float)nDiamondNum * fAdd;
		UserData::getInstance()->setFirstShopping(1);
		break;
	case BUY_GIFT_1:
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        nNum = 120000;
        rmb = 12;
#else
        nNum = 100000;
        rmb = 10;
#endif
		UserData::getInstance()->increaseSkillNum(0, 2);
		UserData::getInstance()->increaseSkillNum(1, 2);
		UserData::getInstance()->increaseSkillNum(2, 2);
		UserData::getInstance()->increaseSkillNum(3, 2);
		UserData::getInstance()->increaseSkillNum(4, 2);

		this->refreshSkillNum();
		break;
	case BUY_GIFT_2:
		nNum = 150000;
		rmb = 10;
		break;
	case BUY_GIFT_3:
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        nNum = 120000;
        rmb = 12;
#else
        nNum = 100000;
        rmb = 10;
#endif
		break;
	case BUY_GIFT_4:
		nNum = 0;
		rmb = 30;

		//记录时间（日期 月日）
		tm* pTime = GetTime1();
		int nYear = pTime->tm_year;
		int nMon = pTime->tm_mon + 1;
		int nDay = pTime->tm_mday;
		int nData = nYear*10000 + nMon*100 + nDay;

		UserData::getInstance()->setBuyMonthCard(1);
		UserData::getInstance()->setGetMonthCardLastTime(nData);
		UserData::getInstance()->save();

		this->showMonthCardLayer();
		break;
	}
	log("-------------2-------------");
	log("-i am c++    result from java  --");
	
		
	UserData::getInstance()->setRechargeAmount(UserData::getInstance()->getRechargeAmount() + rmb);
	UserData::getInstance()->setOverGoldPeak(0);
	//首冲之后移除双倍提示
	hideDouble();
	log("-------------3-------------");
	
	UserData::getInstance()->setGoldNum(UserData::getInstance()->getGoldNum() + nNum);
	UserData::getInstance()->increaseDiamonds(nDiamondNum);
	
	this->refreshMenuGold();
	this->refreshCurrentVIP();
	log("-------------4-------------");
	//检测当前充值金额，解锁响应的炮
	if(UserData::getInstance()->getCurVipLevel() == 1)
	{
		UserData::getInstance()->setCannon2If(1);
	}
	else if(UserData::getInstance()->getCurVipLevel() == 2)
	{
		UserData::getInstance()->setCannon2If(1);
		UserData::getInstance()->setCannon3If(1);
	}
	else if(UserData::getInstance()->getCurVipLevel() == 3)
	{
		UserData::getInstance()->setCannon2If(1);
		UserData::getInstance()->setCannon3If(1);
		UserData::getInstance()->setCannon4If(1);
	}
	log("-------------5-------------");
	//自动更换到最高倍率的炮
	int nNumCannon = Manager::getInstance()->getM_BulletNum();
	if(UserData::getInstance()->getCannon4If() && this->getInGame())
	{
		m_pDelegateGame->chageManualCannon(4, nNumCannon);
		UserData::getInstance()->setCannonID(4);
	}
	else if(UserData::getInstance()->getCannon3If() && this->getInGame()) 
	{
		m_pDelegateGame->chageManualCannon(3, nNumCannon);
		UserData::getInstance()->setCannonID(3);
	}
	else if(UserData::getInstance()->getCannon2If() && this->getInGame()) 
	{
		m_pDelegateGame->chageManualCannon(2, nNumCannon);
		UserData::getInstance()->setCannonID(2);
	}
	log("-------------6-------------");
	if(paytype == BUY_GIFT_1 || paytype == BUY_GIFT_2 || paytype == BUY_GIFT_3)
	{
		this->hideLayer();
	}
	
	//购买钻石
	std::string strPath = __String::createWithFormat("particle/gold_particle2.plist")->getCString();
	if(paytype >= BUY_DIAMOND_6 && paytype <= BUY_DIAMOND_328)
	{
		strPath = __String::createWithFormat("particle/diamond_2.plist")->getCString();
	}
	log("-------------7-------------");
	//添加粒子
	ParticleSystemQuad* pBoom = ParticleSystemQuad::create(strPath.c_str());
	pBoom->setPosition(Vec2(size.width / 2.0f, size.height / 3.0f * 2.0f));
	this->addChild(pBoom, COIN_zOrder + 10000);
	
	MusicData::getInstance()->playSound(23);
	log("-------------8-------------");
	UserData::getInstance()->save();
	//友盟统计
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng::MobClickCpp::pay(rmb, 2, nNum);
	//umeng::MobClickCpp::event(getStringFromInt(paytype + 63).c_str());
#endif
	log("-------------9-------------");
}

/***********网络检查回调*************/
void PublicLayer::callbackDeviceInfoFromJava(Node* target, void* data)
{
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
//检查网络
void PublicLayer::getDeviceInfo(int fake_code)
{
	HttpInstance::getInstance()->getStore()->getDeviceInfo("getDeviceInfo", fake_code, this, callfuncND_selector(PublicLayer::callbackDeviceInfoFromJava));
}

//退出游戏
void PublicLayer::exitGame()
{
	HttpInstance::getInstance()->getStore()->exit("exitGame", 1, this, callfuncND_selector(PublicLayer::callbackExit));
}

void PublicLayer::callbackExit(CCNode* target, void* data)
{

}

//刷新付费结果
void PublicLayer::updateRechargeResult(float dt)
{
    auto status = HttpInstance::getInstance()->getRewardStatus();
    // 无支付或者正在校单
    if(status == REWARD_STATUS_NONE || status == REWARD_STATUS_VALIDATING)
    {
        return;
    }
	
	log("getRewardStatus == (%d)", HttpInstance::getInstance()->getRewardStatus());
    // 支付已经完成
	if(HttpInstance::getInstance()->getRewardStatus() == REWARD_STATUS_COME)
	{
        // 可以再次请求支付
        HttpInstance::getInstance()->setCouldPay(true);
        
		//参数
		__Dictionary* params = __Dictionary::create();
		//第二个参数是key 第一个参数是value
		params->setObject(__String::createWithFormat("%d", HttpInstance::getInstance()->getFakeCode()), "fake_code");
		params->setObject(__String::createWithFormat("1"), "result");
		params->setObject(__String::createWithFormat("100"), "tradeId");
		params->setObject(__String::createWithFormat("0"), "is_test");

		this->callbackConfigure(this, params);
		return;
	}
    
    if (m_nCurrentRequestTimes >= 10) {
        // 超出10次对单，放弃对单
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        std::string message(m_pXmlParser->getString("validate_order_timeout")->getCString());
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("show_toast", &message);
#endif
        //发放完物品状态恢复
        HttpInstance::getInstance()->setRewardStatus(REWARD_STATUS_NONE);
        HttpInstance::getInstance()->setCouldPay(true);
        m_nCurrentRequestTimes = 0;
        m_fValidatingDelayTime = 0;
        if(HttpInstance::getInstance()->getPayType() == PAY_TYPE_THRAN)
        {
            HttpInstance::getInstance()->setPayType(PAY_TYPE_QF);
            this->requestRecharge1();
        }
        else
        {
            HttpInstance::getInstance()->setPayType(PAY_TYPE_THRAN);
        }
    }
    else
    {
        m_fValidatingDelayTime += dt;
        //
        if (m_fValidatingDelayTime >= m_nCurrentRequestTimes*2) {
            // 延迟n秒请求对单, n为请求次数
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
            HttpInstance::getInstance()->setRewardStatus(REWARD_STATUS_VALIDATING);// 等待服务器结果
            requestRecharge2();
            log("m_nCurrentRequestTimes ++");
            m_nCurrentRequestTimes ++;
#else
            int fake_code = HttpInstance::getInstance()->getFakeCode();
            if(fake_code == 1 || fake_code == 7 || fake_code == 8 || fake_code == 14 || fake_code == 15 || fake_code == 16)
            {
                
            }
            else
            {
                HttpInstance::getInstance()->setRewardStatus(REWARD_STATUS_VALIDATING);// 等待服务器结果
                requestRecharge2();
                log("m_nCurrentRequestTimes ++");
                m_nCurrentRequestTimes ++;
            }
#endif
        }
    }
}
//付费
void PublicLayer::recharge()
{
	log("PublicLayer::recharge()");
	HttpInstance::getInstance()->getStore()->Purchase("BUY_CRYSTAL_2", HttpInstance::getInstance()->getFakeCode(), this, callfuncND_selector(PublicLayer::callbackConfigure));
}

//请求付费
void PublicLayer::requestRecharge1()
{
    m_nCurrentRequestTimes = 0;
    m_fValidatingDelayTime = 0;
	int paytype = HttpInstance::getInstance()->getFakeCode();
	int rmb = 6;
	std::string orderNmae = "";
	switch (paytype)
	{
		case BUY_GOLD_6:
			rmb = 6;
			orderNmae = "60000jb";
			break;
		case BUY_GOLD_15:
			rmb = 15;
			orderNmae = "150000jb";
			break;
		case BUY_GOLD_30:
			rmb = 30;
			orderNmae = "300000jb";
			break;
		case BUY_GOLD_50:
			rmb = 50;
			orderNmae = "500000jb";
			break;
		case BUY_GOLD_108:
			rmb = 108;
			orderNmae = "1080000jb";
			break;
		case BUY_GOLD_328:
			rmb = 328;
			orderNmae = "3280000jb";
			break;
		case BUY_GOLD_01:
			rmb = 0;
			orderNmae = "120jb";
			break;
		case BUY_DIAMOND_6:
			rmb = 6;
			orderNmae = "60000zs";
			break;
		case BUY_DIAMOND_15:
			rmb = 6;
			orderNmae = "60000zs";
			break;
		case BUY_DIAMOND_30:
			rmb = 30;
			orderNmae = "300000zs";
			break;
		case BUY_DIAMOND_50:
			rmb = 50;
			orderNmae = "500000zs";
			break;
		case BUY_DIAMOND_108:
			rmb = 108;
			orderNmae = "1080000zs";
			break;
		case BUY_DIAMOND_328:
			rmb = 328;
			orderNmae = "3280000zs";
			break;
		case BUY_GIFT_1:
			rmb = 10;
			orderNmae = "lb";
			break;
		case BUY_GIFT_2:
			rmb = 10;
			orderNmae = "lb";
			break;
		case BUY_GIFT_3:
			rmb = 10;
			orderNmae = "lb";
			break;
		case BUY_GIFT_4:
			rmb = 30;
			orderNmae = "lb";
			break;
	}
	rapidjson::Document writedoc;
	writedoc.SetObject();
	rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
	
	// json object 格式添加 “名称/值” 对
	
	std::string pImei = HttpInstance::getInstance()->getDeviceMac().c_str();
	int nLen = strlen(pImei.c_str());
	char pData[100] = "";
	for(int i = 0; i < nLen; i++)
	{
		pData[i] = pImei[i];
	}
	pData[nLen] = '\0';
	writedoc.AddMember("imei", pData, allocator);
    writedoc.AddMember("userName", "yelong", allocator);
	writedoc.AddMember("goods", paytype, allocator);
	writedoc.AddMember("price", rmb, allocator);
	writedoc.AddMember("payType", HttpInstance::getInstance()->getMobileType(), allocator);
	writedoc.AddMember("clientChannel", HttpInstance::getInstance()->getChannelID().c_str(), allocator);
	writedoc.AddMember("clientVersion", HttpInstance::getInstance()->getCurVersion().c_str(), allocator);
	

	StringBuffer buffer;
	rapidjson::Writer<StringBuffer> writer(buffer);
	writedoc.Accept(writer);
	std::string str0 = buffer.GetString();
	
	log("mac == (%s)", HttpInstance::getInstance()->getDeviceMac().c_str());
	log("check order 111 str == (%s)", str0.c_str());

	//HttpInstance::getInstance()->requestRecharge1(str0);

	/*if((paytype == 1 || paytype == 7 || paytype == 8 || paytype == 14 || paytype == 15 || paytype == 16) && 
		HttpInstance::getInstance()->getPayType() == PAY_TYPE_THRAN)
	{
		this->recharge();
	}
	else
	{*/
		HttpInstance::getInstance()->getStore()->RequestOrder(str0, 1, this, callfuncND_selector(PublicLayer::callbackGetOrder));
	//}
}
//请求付费
void PublicLayer::requestRecharge2()
{
	rapidjson::Document writedoc;
	writedoc.SetObject();
	rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
	
	// json object 格式添加 “名称/值” 对
	std::string pImei = HttpInstance::getInstance()->getDeviceMac().c_str();
	size_t nLen = strlen(pImei.c_str());
	char pData[100] = "";
	for(int i = 0; i < nLen; i++)
	{
		pData[i] = pImei[i];
	}
	pData[nLen] = '\0';
    writedoc.AddMember("imei", pData, allocator);


	std::string pOrder= HttpInstance::getInstance()->getCurOrderString().c_str();
	nLen = strlen(pOrder.c_str());
	char pData1[100] = "";
	for(int i = 0; i < nLen; i++)
	{
		pData1[i] = pOrder[i];
	}
	pData1[nLen] = '\0';
	writedoc.AddMember("order", pData1, allocator);
    
    // appstore需要receipt参数，以校验支付凭证
    if (CURRENT_CHANNEL == IOS) {
        auto receipt = HttpInstance::getInstance()->getTransactionReceipt();
        writedoc.AddMember("receipt", receipt.c_str(), allocator);
    }
	//writedoc.AddMember("imei", "123456789", allocator);
	//writedoc.AddMember("order", "12345678910", allocator);
    
	StringBuffer buffer;
	rapidjson::Writer<StringBuffer> writer(buffer);
	writedoc.Accept(writer);
	std::string str0 = buffer.GetString();
	
	log("check order 222 str == (%s)", str0.c_str());
	//HttpInstance::getInstance()->requestRecharge2(str0);
	HttpInstance::getInstance()->getStore()->RequestOrder(str0, 2, this, callfuncND_selector(PublicLayer::callbackGetOrder));
}
//请求付费
void PublicLayer::requestRecharge3()
{
	int paytype = HttpInstance::getInstance()->getFakeCode();
	int rmb = 6;
	std::string orderNmae = "";
	switch (paytype)
	{
		case BUY_GOLD_6:
			rmb = 6;
			orderNmae = "60000jb";
			break;
		case BUY_GOLD_15:
			rmb = 15;
			orderNmae = "150000jb";
			break;
		case BUY_GOLD_30:
			rmb = 30;
			orderNmae = "300000jb";
			break;
		case BUY_GOLD_50:
			rmb = 50;
			orderNmae = "500000jb";
			break;
		case BUY_GOLD_108:
			rmb = 108;
			orderNmae = "1080000jb";
			break;
		case BUY_GOLD_328:
			rmb = 328;
			orderNmae = "3280000jb";
			break;
		case BUY_GOLD_01:
			rmb = 0;
			orderNmae = "120jb";
			break;
		case BUY_DIAMOND_6:
			rmb = 6;
			orderNmae = "60000zs";
			break;
		case BUY_DIAMOND_15:
			rmb = 6;
			orderNmae = "60000zs";
			break;
		case BUY_DIAMOND_30:
			rmb = 30;
			orderNmae = "300000zs";
			break;
		case BUY_DIAMOND_50:
			rmb = 50;
			orderNmae = "500000zs";
			break;
		case BUY_DIAMOND_108:
			rmb = 108;
			orderNmae = "1080000zs";
			break;
		case BUY_DIAMOND_328:
			rmb = 328;
			orderNmae = "3280000zs";
			break;
		case BUY_GIFT_1:
			rmb = 10;
			orderNmae = "lb";
			break;
		case BUY_GIFT_2:
			rmb = 10;
			orderNmae = "lb";
			break;
		case BUY_GIFT_3:
			rmb = 10;
			orderNmae = "lb";
			break;
		case BUY_GIFT_4:
			rmb = 30;
			orderNmae = "lb";
			break;
	}
	rapidjson::Document writedoc;
	writedoc.SetObject();
	rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
	
	// json object 格式添加 “名称/值” 对
	
	std::string pOrder= HttpInstance::getInstance()->getCurOrderString().c_str();
	int nLen = strlen(pOrder.c_str());
	char pData1[100] = "";
	for(int i = 0; i < nLen; i++)
	{
		pData1[i] = pOrder[i];
	}
	pData1[nLen] = '\0';
	writedoc.AddMember("order", pData1, allocator);
	writedoc.AddMember("price", rmb, allocator);
	writedoc.AddMember("goods", orderNmae.c_str(), allocator);


	StringBuffer buffer;
	rapidjson::Writer<StringBuffer> writer(buffer);
	writedoc.Accept(writer);
	std::string str0 = buffer.GetString();

	HttpInstance::getInstance()->getStore()->RequestOrder(str0, 3, this, callfuncND_selector(PublicLayer::callbackGetOrder));
}

/***********获取付费订单的回调*************/
void PublicLayer::callbackGetOrder(Node* target, void* data)
{
	__Dictionary* dic = (__Dictionary*)data;

	int nFakeCode = dic->valueForKey("fake_code")->intValue();
	std::string str = dic->valueForKey("result_str")->getCString();
	int nLen = strlen(str.c_str());
	char pData[100] = "";
	for(int i = 0; i < nLen; i++)
	{
		pData[i] = str[i];
	}
	pData[nLen] = '\0';
	//获取订单的返回
	if(nFakeCode == 1)
	{
		rapidjson::Document readdoc;
		readdoc.Parse<0>(pData);

		int nResult = readdoc["result"].GetInt();
		if(nResult == 1)
		{
			std::string strOrder = readdoc["order"].GetString();
			int nChannel = readdoc["payChannel"].GetInt();
			//HttpInstance::getInstance()->setRewardStatus(REWARD_STATUS_WAIT);
			HttpInstance::getInstance()->setCurOrderString(strOrder);
			this->recharge();
		}
	}
	//核对订单的返回
	else if(nFakeCode == 2)
	{
		rapidjson::Document readdoc;
		readdoc.Parse<0>(pData);

		int nResult = readdoc["result"].GetInt();
		if(nResult == 1)
		{
            if (readdoc.HasMember("status"))
            {
                int nStatus = readdoc["status"].GetInt();
                if(nStatus == 1)
                {
                    log("---------m_nRewardStatus = REWARD_STATUS_COME---------");
                    HttpInstance::getInstance()->setRewardStatus(REWARD_STATUS_COME);
                    return;
                }
                log("---nStatus-- ==== -----(%d)--", nStatus);
            }
		}
        log("---------m_nRewardStatus = REWARD_STATUS_WAIT---------");
        m_fValidatingDelayTime = 0;
        HttpInstance::getInstance()->setRewardStatus(REWARD_STATUS_WAIT);
	}
	else if(nFakeCode == 4)
	{
		rapidjson::Document readdoc;
		readdoc.Parse<0>(pData);

		int nResult = readdoc["result"].GetInt();
		if(nResult == 1)
		{
			int fake_code = readdoc["goods"].GetInt();
			//参数
			__Dictionary* params = __Dictionary::create();
			//第二个参数是key 第一个参数是value
			params->setObject(__String::createWithFormat("%d", fake_code), "fake_code");
			params->setObject(__String::createWithFormat("1"), "result");
			params->setObject(__String::createWithFormat("100"), "tradeId");
			params->setObject(__String::createWithFormat("0"), "is_test");

			this->callbackConfigure(this, params);
		}
		else
		{
			this->showPublicSureLayer("key_error");
		}
	}
	
}
