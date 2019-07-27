#include "SmallGameLayer.h"
#include "MenuLayer.h"
#include "MusicData.h"

SmallGameLayer::SmallGameLayer()
{

}
SmallGameLayer::~SmallGameLayer()
{

}
bool SmallGameLayer::init()
{
	Layer::init();

	size = Director::getInstance()->getWinSize();
	m_nCurrentRate = 10;
	m_nCurrentRandomID = 1;
	m_bRotating = false;
	MusicData::getInstance()->preloadSmallGameMusic();

	LayerColor* pLayerColor = LayerColor::create(Color4B(0, 0, 0, 255));
	pLayerColor->setPosition(Vec2::ZERO);
	this->addChild(pLayerColor);

	auto pBack = Sprite::create("ui/smallgame1/jc_bg.jpg");
	pBack->setPosition(Vec2(size.width / 2, size.height / 2));
	pBack->setScale(0.75);
	this->addChild(pBack);

	//UI的使用
	//m_pUINode = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui/smallgame1/NewUi_1.ExportJson");
	m_pUINode = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui/smallgame1/NewUi_1.json");
	m_pUINode->setPosition(Vec2(0, 0));
	this->addChild(m_pUINode, 1); 
	m_pUINode->setScaleX(size.width/854.0);
	
	//开始
	ui::MyButton* buttonBegin = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 16);
	buttonBegin->addTouchEventListener(CC_CALLBACK_2(SmallGameLayer::callBackBegin, this));
	buttonBegin->setClickPicture();
	//退出
	ui::MyButton* buttonExit = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 145);
	buttonExit->addTouchEventListener(CC_CALLBACK_2(SmallGameLayer::callBackExit, this));
	buttonExit->setClickPicture();
	//继续
	ui::MyButton* buttonGoon = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 146);
	buttonGoon->addTouchEventListener(CC_CALLBACK_2(SmallGameLayer::callBackGoon, this));
	buttonGoon->setClickPicture();
	//退出
	ui::MyButton* buttonExit1 = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 303);
	buttonExit1->addTouchEventListener(CC_CALLBACK_2(SmallGameLayer::callBackExit, this));
	buttonExit1->setClickPicture();

	//下注
	int nTag = 3;
	for (int i = 0; i < 8; i++)
	{
		ui::Button* buttonBet = (ui::Button *)ui::Helper::seekWidgetByTag(m_pUINode, nTag + i);
		buttonBet->addTouchEventListener(CC_CALLBACK_2(SmallGameLayer::callBackBet, this));
	}

	//切换倍率
	nTag = 24;
	for (int i = 0; i < 3; i++)
	{
		ui::Button* callBackSwitchRate = (ui::Button *)ui::Helper::seekWidgetByTag(m_pUINode, nTag + i);
		callBackSwitchRate->addTouchEventListener(CC_CALLBACK_2(SmallGameLayer::callBackSwitchRate, this));
		if(i == 0)
		{
			callBackSwitchRate->setBrightStyle(ui::BrightStyle::HIGHLIGHT);
		}
	}

	//初始化显示
	this->showBetLayer();

	//更新金币
	this->refreshGold();

	//弹出框
	m_pPublicLayer = PublicLayer::create();
	m_pPublicLayer->setPosition(Vec2(size.width / 2 - 427, 0));
	this->addChild(m_pPublicLayer, 10);

	this->scheduleUpdate();
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/47.mp3", true);
	MusicData::getInstance()->playSound(46);

	string strPath[8] = {"fish_frame_166.plist", "fish_frame_6.plist", "fish_frame_7.plist", "fish_frame_8.plist",
		"fish_frame_252.plist", "fish_frame_22.plist", "fish_frame_23.plist", "fish_frame_54.plist"};
	//预加载资源
	for (int i = 0; i < 8; i++)
	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(strPath[i]);
	}

	//添加鱼
	this->addFish();

	return true;
}

//显示押注界面
void SmallGameLayer::showBetLayer()
{
	//初始化显示
	auto pPanel1 = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 2);
	pPanel1->setVisible(true);
	auto pPanel2 = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 11);
	pPanel2->setVisible(true);
	auto pJC = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 94);
	pJC->setRotation(0);
	pJC->setVisible(false);
	auto pJCBack = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 15);
	pJCBack->setVisible(false);
	auto buttonBegin = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 16);
	buttonBegin->setVisible(true);
	auto pPanel3 = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 42);
	pPanel3->setVisible(false);
	auto pPanel4 = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 77);
	pPanel4->setVisible(false);
	auto pResult = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 144);
	pResult->setVisible(false);
	auto pFish = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 168);
	pFish->setVisible(false);
	//隐藏押注数组
	int nTag = 136;
	for (int i = 0; i < 8; i++)
	{
		ui::TextAtlas* textNum = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, nTag + i);
		textNum->setVisible(false);
		textNum->setString("0");
	}
}
//显示旋转界面
void SmallGameLayer::showRotateLayer()
{
	MusicData::getInstance()->playSound(71);
	//初始化显示
	auto pPanel1 = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 2);
	pPanel1->setVisible(false);
	auto pPanel2 = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 11);
	pPanel2->setVisible(true);
	auto pJC = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 94);
	pJC->setVisible(true);
	auto pJCBack = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 15);
	pJCBack->setVisible(true);
	auto buttonBegin = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 16);
	buttonBegin->setVisible(false);
	auto pPanel3 = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 42);
	pPanel3->setVisible(true);
	auto pPanel4 = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 77);
	pPanel4->setVisible(true);
	auto pResult = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 144);
	pResult->setVisible(false);
	auto pFish = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 168);
	pFish->setVisible(true);

	for (int i = 0; i < 16; i++)
	{
		auto pYellow = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, i + 78);
		pYellow->setVisible(false);
	}

	int nRoundArray[16] = {4,3,1,2,7,0,3,1,6,0,1,2,5,3,0,2};
	//设置押注的显示
	int nTag = 230;
	for (int i = 0; i < 16; i++)
	{
		int nRate = getBetRateByID(i);
		auto pCoin = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, nTag + i);
		if(nRate > 0)
		{
			pCoin->setVisible(true);
		}
		else
		{
			pCoin->setVisible(false);
		}
	}
}
//显示结果界面
void SmallGameLayer::showResultLayer()
{
	//初始化显示
	auto pPanel1 = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 2);
	pPanel1->setVisible(false);
	auto pPanel2 = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 11);
	pPanel2->setVisible(true);
	auto pJC = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 94);
	pJC->setVisible(true);
	auto pJCBack = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 15);
	pJCBack->setVisible(true);
	auto buttonBegin = (ui::MyButton *)ui::Helper::seekWidgetByTag(m_pUINode, 16);
	buttonBegin->setVisible(false);
	auto pPanel3 = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 42);
	pPanel3->setVisible(true);
	auto pPanel4 = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 77);
	pPanel4->setVisible(false);
	auto pResult = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 144);
	pResult->setVisible(true);
	pResult->removeChildByTag(100);
	//输和赢
	auto pWin = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 165);
	pWin->setVisible(false);
	auto pLose = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 164);
	pLose->setVisible(false);

	//黄色光开始旋转
	auto pYellowBack1 = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 153);
	auto pYellowBack2 = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 159);
	pYellowBack1->stopAllActions();
	pYellowBack2->stopAllActions();
	pYellowBack1->runAction(RepeatForever::create(RotateBy::create(20.0, 360)));
	pYellowBack2->runAction(RepeatForever::create(RotateBy::create(20.0, -360)));

	int nTotalRate = this->getTotalBetRate();
	int nCurRate = this->getBetRateByID(m_nCurrentRandomID);
	int nSelfRate = m_nCurrentRandomID%4 == 0 ? 20 : 5;
	int nResult = nCurRate*nSelfRate - nTotalRate;
	UserData::getInstance()->setGoldNum(UserData::getInstance()->getGoldNum() + nCurRate*nSelfRate);
	UserData::getInstance()->save();
	this->refreshGold();
	auto pTextNum = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, 22);
	pTextNum->setString(getStringFromInt(nResult));
	if(nResult >= 0)
	{
		MusicData::getInstance()->playSound(69);
		log("win    win    win    win    win    win");
		pWin->setVisible(true);
		auto pTextResult = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, 167);
		pTextResult->setString(getStringFromInt(nResult));
	}
	else
	{
		MusicData::getInstance()->playSound(45);
		log("lose    lose    lose    lose    lose    lose");
		pLose->setVisible(true);
		auto pTextResult = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, 166);
		pTextResult->setString(getStringFromInt(nResult));
	}
}

//开始
void SmallGameLayer::callBackBegin(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		bool bBet = this->getIsBet();
		if(!bBet)
		{
			log("hasnt bet please bet the fish");
			//MessageBox("please bet", "");
			m_pPublicLayer->showNotBetLayer();
		}
		else
		{
			this->showRotateLayer();
			auto pJC = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 94);
			pJC->setVisible(true);

			m_nCurrentRandomID = this->randPosition();
			auto pRot = RotateBy::create(10.0, 2160 + this->getRotationByID(m_nCurrentRandomID));
			pJC->runAction(EaseInOut::create(pRot, 4.5));

			auto seq = Sequence::create(DelayTime::create(10.0f), 
				CallFuncN::create(CC_CALLBACK_1(SmallGameLayer::callBackShowResultLayer, this)), 
				NULL);
			this->runAction(seq);

			m_bRotating = true;
		}

	}
}
//下注
void SmallGameLayer::callBackBet(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		MusicData::getInstance()->playSound(44);
		auto pButton = (Button*)pObject;

		ui::TextAtlas* textNum = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, pButton->getTag() + 133);
		int nPreNum = getIntFromS(textNum->getString().c_str());
		int nNeedGold = m_nCurrentRate;
		int nCurGold = UserData::getInstance()->getGoldNum();
		//金币足够
		if(nCurGold >= nNeedGold)
		{
			nPreNum += nNeedGold;
			textNum->setString(getStringFromInt(nPreNum));
			textNum->setVisible(true);
			UserData::getInstance()->setGoldNum(nCurGold - nNeedGold);

			this->refreshGold();
		}
		//金币不足
		else
		{
			m_pPublicLayer->showCoinNotEnoughLayer();
		}
	}
}
//切换倍率
void SmallGameLayer::callBackSwitchRate(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		MusicData::getInstance()->playSound(44);
		auto pButton = (Button*)pObject;
		int nTag = pButton->getTag();
		if(nTag == 24)
			m_nCurrentRate = 10;
		else if(nTag == 25)
			m_nCurrentRate = 100;
		else if(nTag == 26)
			m_nCurrentRate = 1000;

		
		setBrightByID(nTag - 24);
	}
}

//根据下标获取角度
float SmallGameLayer::getRotationByID(int nID)
{
	float fRot = 0;
	switch (nID)
	{
	case 0:
		fRot = 0;
		break;
	case 1:
		fRot = 20;
		break;
	case 2:
		fRot = 36;
		break;
	case 3:
		fRot = 53;
		break;
	case 4:
		fRot = 90;
		break;
	case 5:
		fRot = 127;
		break;
	case 6:
		fRot = 144;
		break;
	case 7:
		fRot = 160;
		break;
	case 8:
		fRot = 180;
		break;
	case 9:
		fRot = 200;
		break;
	case 10:
		fRot = 216;
		break;
	case 11:
		fRot = 233;
		break;
	case 12:
		fRot = 270;
		break;
	case 13:
		fRot = 307;
		break;
	case 14:
		fRot = 324;
		break;
	case 15:
		fRot = 340;
		break;
	default:
		break;
	}

	return fRot;
}
//通过当前转角获取ID
int SmallGameLayer::getIDByCurRotation(float fRot)
{
	int nRot = (int(fRot))%360;

	for (int i = 0; i < 16; i++)
	{
		//当前角度
		int nCurRot = getRotationByID(i);
		//前一个ID的角度
		int preRot = getRotationByID(i - 1);
		if(i == 0)
			preRot = -20;
		//后一个ID的角度
		int nextRot = getRotationByID(i + 1);
		if(i == 15)
			nextRot = 360;
		//获取当前i的角度范围
		preRot = preRot + (nCurRot - preRot)/2;
		nextRot = nCurRot + (nextRot - nCurRot)/2;
		if(nRot >= preRot && nRot < nextRot)
		{
			return i;
		}
	}
	return -1;
}
//通过ID添加播放动画
void SmallGameLayer::addAnimationByID(int nID)
{
	string strName[8] = {"166_%02d.png", "6_%02d.png", "7_%02d.png", "8_%02d.png",
		"252_%02d.png", "22_%02d.png", "23_%02d.png", "54_%02d.png"};
	int frameNum[8] = {8,6,10,6,8,9,8,10};

	int nRoundArray[16] = {4,3,1,2,7,0,3,1,6,0,1,2,5,3,0,2};
	auto pPanel4 = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 144);
	auto pYellow = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 153);
	//旋转界面的鱼
	
	__String* sreName = __String::createWithFormat(strName[nRoundArray[nID]].c_str(),1);
	int nFrame = frameNum[nRoundArray[nID]];
	Vector<SpriteFrame*> array1(nFrame);

	for (int i=1; i <= nFrame; i++)
	{
		SpriteFrameCache *pcache = SpriteFrameCache::getInstance();
		__String* strName1 = __String::createWithFormat(strName[nRoundArray[nID]].c_str(), i);
		SpriteFrame* pframe = pcache->getSpriteFrameByName(strName1->getCString());
		array1.pushBack(pframe);
	}
	Animation *panimation = Animation::createWithSpriteFrames(array1, 0.1f);

	Sprite* sp = Sprite::create();
	if(nID%4 == 0)
		sp->setScale(0.6);
	else
		sp->setScale(1.0);
	sp->setPosition(pYellow->getPosition());
	sp->setRotation(180);
	pPanel4->addChild(sp, 10, 100);

	sp->runAction(RepeatForever::create(Animate::create(panimation)));
	
}
//根据ID来设置高亮按钮
void SmallGameLayer::setBrightByID(int nID)
{
	int nTag = 24;
	for (int i = 0; i < 3; i++)
	{
		ui::Button* callBackSwitchRate = (ui::Button *)ui::Helper::seekWidgetByTag(m_pUINode, nTag + i);
		callBackSwitchRate->addTouchEventListener(CC_CALLBACK_2(SmallGameLayer::callBackSwitchRate, this));
		if(i == nID)
		{
			callBackSwitchRate->setBrightStyle(ui::BrightStyle::HIGHLIGHT);
		}
		else
		{
			callBackSwitchRate->setBrightStyle(ui::BrightStyle::NORMAL);
		}
	}
}
//刷新金币
void SmallGameLayer::refreshGold()
{
	//更新金币
	int nGoldNum = UserData::getInstance()->getGoldNum();
	auto pTextGold = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, 23);
	pTextGold->setString(getStringFromInt(nGoldNum));

	auto pTextNum = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, 22);
	pTextNum->setString(getStringFromInt(0));
}
//随机位置
int SmallGameLayer::randPosition()
{
	int nRand = rand()%10 + 1;
	int nID = 0;
	if(nRand < 3)
	{
		nID = rand()%4 * 4;
	}
	else
	{
		int array[12] = {1,2,3,5,6,7,9,10,11,13,14,15};
		nID = array[rand()%12];
	}

	return nID;
}
//通过ID获取押注的倍率
int SmallGameLayer::getBetRateByID(int nID)
{
	int nRoundArray[16] = {4,3,1,2,7,0,3,1,6,0,1,2,5,3,0,2};

	ui::TextAtlas* textNum = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, nRoundArray[nID] + 136);

	return getIntFromS(textNum->getString().c_str());
}
//获取一共得押注
int SmallGameLayer::getTotalBetRate()
{
	int nRate = 0;
	for (int i = 0; i < 8; i++)
	{
		ui::TextAtlas* textNum = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, i + 136);
		nRate += getIntFromS(textNum->getString().c_str());
	}
	return nRate;
}
//显示结果界面
void SmallGameLayer::callBackShowResultLayer(Node* pNode)
{
	m_bRotating = false;
	this->showResultLayer();
	this->addAnimationByID(m_nCurrentRandomID);
}
//退出
void SmallGameLayer::callBackExit(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		MusicData::getInstance()->playSound(13);
		if(Manager::getInstance()->getInSmallGame())
		{
			Manager::getInstance()->setInSmallGame(false);
			MusicData::getInstance()->unloadSmallGameMusic();
			auto pScene = Scene::create();
			pScene->addChild(MenuLayer::create());
			Director::getInstance()->replaceScene(pScene);
		}
	}
}
//继续
void SmallGameLayer::callBackGoon(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		MusicData::getInstance()->playSound(13);
		this->showBetLayer();
	}
}
//获取是否押注了金币
bool SmallGameLayer::getIsBet()
{
	//隐藏押注数组
	int nTag = 136;
	for (int i = 0; i < 8; i++)
	{
		ui::TextAtlas* textNum = (ui::TextAtlas*)ui::Helper::seekWidgetByTag(m_pUINode, nTag + i);
		int nData = getIntFromS(textNum->getString().c_str());
		if(nData != 0)
		{
			return true;
		}
	}

	return false;
}
//添加鱼
void SmallGameLayer::addFish()
{
	SpriteFrameCache* pCache = CCSpriteFrameCache::getInstance();
	
	string strPath[8] = {"fish_frame_166.plist", "fish_frame_6.plist", "fish_frame_7.plist", "fish_frame_8.plist",
						 "fish_frame_252.plist", "fish_frame_22.plist", "fish_frame_23.plist", "fish_frame_54.plist"};
	string strPng[8] = {"fish_frame_166.png", "fish_frame_6.png", "fish_frame_7.png", "fish_frame_8.png",
						"fish_frame_252.png", "fish_frame_22.png", "fish_frame_23.png", "fish_frame_54.png"};
	string strName[8] = {"166_%02d.png", "6_%02d.png", "7_%02d.png", "8_%02d.png",
						 "252_%02d.png", "22_%02d.png", "23_%02d.png", "54_%02d.png"};
	//下注界面的鱼
	int nTag = 3;
	for (int i = 0; i < 8; i++)
	{
		pCache->addSpriteFramesWithFile(strPath[i], strPng[i]);
		ui::Button* buttonBet = (ui::Button *)ui::Helper::seekWidgetByTag(m_pUINode, nTag + i);
		__String* sreName = __String::createWithFormat(strName[i].c_str(),1);
		auto sp = Sprite::createWithSpriteFrameName(sreName->getCString());
		sp->setPosition(Vec2(buttonBet->getContentSize().width / 2.0f, buttonBet->getContentSize().height / 2.0f));
		sp->setRotation(180.0f);
		buttonBet->addChild(sp);

		if(i < 4)
		{
			sp->setScale(0.8);
		}
		else
		{
			sp->setScale(0.4);
		}
	}


	int nRoundArray[16] = {4,3,1,2,7,0,3,1,6,0,1,2,5,3,0,2};
	auto pPanel3 = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 168);
	//旋转界面的鱼
	nTag = 44;
	for (int i = 0; i < 16; i++)
	{
		auto pRound = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, nTag + i);
		
		__String* sreName = __String::createWithFormat(strName[nRoundArray[i]].c_str(),1);
		auto sp = Sprite::createWithSpriteFrameName(sreName->getCString());
		sp->setPosition(pRound->getPosition());
		sp->setRotation(180.0f);
		pPanel3->addChild(sp, 2, i + 100);

		if(nRoundArray[i] < 4)
		{
			sp->setScale(0.6);
		}
		else
		{
			sp->setScale(0.4);
		}
	}
}
void SmallGameLayer::update(float dt)
{
	if(m_bRotating)
	{
		auto pJC = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, 94);
		int nCurID = getIDByCurRotation(pJC->getRotation());
		auto pPanel3 = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 168);
		//黄色底
		for (int i = 0; i < 16; i++)
		{
			auto pYellow = (ui::ImageView*)ui::Helper::seekWidgetByTag(m_pUINode, i + 78);
			if(i == nCurID)
				pYellow->setVisible(true);
			else
				pYellow->setVisible(false);
		}
		//缩放鱼
		for (int i = 0; i < 16; i++)
		{
			auto pFish = pPanel3->getChildByTag(i + 100);
			float oldScale = pFish->getScale();
			if(i%4 == 0)
			{
				if(i == nCurID)
				{
					if(pFish->getScale() <= 0.5)
						MusicData::getInstance()->playSound(53);
					pFish->setScale(0.4*1.5);
				}
				else
					pFish->setScale(0.4);
			}
			else
			{
				if(i == nCurID)
				{
					if(pFish->getScale() <= 0.7)
						MusicData::getInstance()->playSound(53);
					pFish->setScale(0.6*1.5);
				}
				else
					pFish->setScale(0.6);
			}
		}
	}
}