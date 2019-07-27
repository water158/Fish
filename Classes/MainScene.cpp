//
//  MainScene.cpp
//  Helloworld
//
//  Created by etgame on 15-1-28.
//
//

#include "MainScene.h"
#include "Bullet.h"
#include "Fire.h"
#include "ManualCannon.h"
#include "Data.h"
#include "FishPath.h"
#include "NetSprite.h"
#include "Coin.h"
#include "PixCell.h"
#include "CoinPrompt.h"
#include "MyActionNew.h"

#include "FishSpline.h"
#include "FishBezier.h"
#include "FishBoom.h"
#include "BoomData.h"
#include "ProductType.h"

#include "Manager.h"
#include "EnergyCannonCard.h"
#include "ScopeFish.h"
#include "PublicLayer.h"
#include "FishGroup.h"
#include "Fire.h"
#include "FishMedol.h"
#include "MyMenu.h"
#include "SanYuanSiXi.h"
#include "UserData.h"
#include "UserDataMission.h"
#include "MusicData.h"

#include "MenuLayer.h"
#include "NextLevel.h"
#include "CountdownLayer.h"
#include "NoticeBoss.h"
#include "BossCoinPrompt.h"
#include "TelephoneFareFish.h"
//#include "MobClickCpp.h"
#include "ShiledLayer.h"

using namespace std;
//using namespace umeng;

#define CountDown_tag 111
#define LOCK_tag 112
#define FIRE1_tag 113
#define FIRE2_tag 114
#define FIRE3_tag 115
//int m_totalCoin;
const float sanyuansixi[2][3] = {{1100, 400, -100}, {-1100, 400, 850}};
static int chage_pro = 0; //改变的概率
static int last_coin = 2000;  //记录上一次金币数

Scene* MainScene::scene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vec2(0.0f, 0.0f));
    scene->getPhysicsWorld()->setDebugDrawMask(false);
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    MainScene* layer = MainScene::create();
    
    layer->setPhyWorld(scene->getPhysicsWorld());
    scene->addChild(layer);
    return scene;
}

bool MainScene::init()
{
    if (!CCLayer::init()) 
	{
        return false;
    }

	Manager::getInstance()->cleanBuffer();

    instance = Manager::getInstance();
    instance->setM_layer(this);
    size = CCDirector::getInstance()->getWinSize();
    //读取总金币数
    //instance->setM_totalCoin(instance->getM_totalCoin());
    
    //用户ID
	if(UserData::getInstance()->getPlayerID() == 0)
	{
		UserData::getInstance()->setPlayerID(rand() % 900000 + 100000);
		UserData::getInstance()->save();
	}
   
    Manager::getInstance()->setBulletNum(0);
    //帧缓存
    this->loadCache();


    //任务单例
    u_data = UserData::getInstance();

	//将默认的炮台改成最后一个
	int nTempID = 0;
	if(UserData::getInstance()->getCannon2If() == 1)
		nTempID = 2;
	if(UserData::getInstance()->getCannon3If() == 1)
		nTempID = 3;
	if(UserData::getInstance()->getCannon4If() == 1)
		nTempID = 4;

	UserData::getInstance()->setCannonID(nTempID);
	
    if (instance->getRoomNum() == 1) 
	{
		Manager::getInstance()->setIsStopBoom(false);
		//单人场
        levelNum = instance->getOne_LevelNum();
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
		//umeng::MobClickCpp::event(getStringFromInt(YM_ENTER_SINGLE).c_str());
#endif

        char level_str[10];
        sprintf(level_str, "bg_%d.png", levelNum);
        Sprite* bg = CCSprite::create(level_str);
        bg->setPosition(Point(size.width/2, size.height/2));
        this->addChild(bg, -1, kTagBg_1);
        m_nCurrentBgID = levelNum;
        
        missionData = u_data->getUserDataMissionByID(levelNum);
         
        //弹出框
        m_pPublicLayer = PublicLayer::create();
        m_pPublicLayer->setPosition(Vec2(size.width / 2 - 427, 0));
        this->addChild(m_pPublicLayer, PUBLIC_zOrder);
        
        m_pPublicLayer->setDelegateGame(this);
        m_pPublicLayer->showLeftTaskLayer();
        
        m_pPublicLayer->showTaskLayer();
        m_pPublicLayer->showGameLayer();
        
        m_pPublicLayer->m_pUINode->setPosition(Vec2::ZERO);
		m_pPublicLayer->m_pUINode->setScaleX(size.width/854.0);
        
        m_pPublicLayer->updateRewardTime(0.0f);
        int nID = UserData::getInstance()->getCannonID();
        powerOfBullet = UserData::getInstance()->getCannonEnergyByID(nID);
        
		if(instance->getM_bulletPower() < 20)
			instance->setM_bulletPower(20);   

        //添加炮台
        manualCannon = ManualCannon::createCannon();
        manualCannon->initConnon(instance->getM_bulletPower(), u_data->getCannonID(), instance->getM_BulletNum(), m_bCDTime);
        this->addChild(manualCannon, MANUALCANNON_zOrder);
        manualCannon->manualNum = 1;
        manualCannon->setPosition(Vec2(size.width/2, 45));
		manualCannon->setM_chosePositionNum(1);
        manualCannon->changeCannonPos();
		manualCannon->setDelegateGame(this);

        
        //添加 造型鱼
        FishMedol* medol = FishMedol::createFishMedol();
        medol->initFishMedol();
        this->addChild(medol, 0, kTagFishMedol);
		medol->setDelegateGame(this);
        isBoom = true;
		m_bNormalFish = false;
		m_bSpeedUp = true;
         medolcdTime = medol->getTotalTime();
        delayTimeAddFish = 25;
        m_bPaused = false;
		m_bPaused1 = false;
        m_nEffectIndex = 0;
        m_bLocked = false;
        m_bCDTime = false;
		m_recordAddFishRime = 250;
    }
	//多人场
	else if (instance->getRoomNum() == 2) 
	{
		int nLevelNum = instance->getTwo_LevelNum();
		//随机随机彩金奖池金币数量
		tm* stm = GetTime1();
		int nMin = stm->tm_min%20;
		int nSingle = 20000;
		if(nLevelNum == 1)
			nSingle = 15000;
		else if(nLevelNum == 2)
			nSingle = 30000;
		else if(nLevelNum == 3)
			nSingle = 450000;
		else if(nLevelNum == 4)
			nSingle = 6750000;
		int nRandNum = nMin * nSingle + rand() % nSingle;
		Manager::getInstance()->setManyPoolGoldNum(nRandNum);

		Manager::getInstance()->setTaskOver(false);

		Manager::getInstance()->setIsStopBoom(true);
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
		//umeng::MobClickCpp::event(getStringFromInt(YM_ENTER_FOUR_PLAYER).c_str());
#endif
        //加载UI
        m_pPublicLayer = PublicLayer::create();
        m_pPublicLayer->setPosition(Vec2(size.width / 2 - 427, 0));
        this->addChild(m_pPublicLayer, PUBLIC_zOrder);
        m_pPublicLayer->setDelegateGame(this);
        m_pPublicLayer->showManyGameLayer();
        m_pPublicLayer->m_pUINode->setPosition(Vec2::ZERO);
		m_pPublicLayer->m_pUINode->setScaleX(size.width/854.0);
        medolcdTime = 0;
        delayTimeAddFish = 0;
        //设置背景图片
        levelNum = instance->getTwo_LevelNum();
		int nBgID = rand() % 10 + 1;
        char level_str[10];
        sprintf(level_str, "bg_%d.png", nBgID);
        Sprite* bg = CCSprite::create(level_str);
        bg->setPosition(Point(size.width/2, size.height/2));
        this->addChild(bg, -1, kTagBg_1);
		m_nCurrentBgID = nBgID;
        
		if(levelNum < 3)
			powerOfBullet = d_bulletPowerNew[levelNum - 1][2];
		else
			powerOfBullet = d_bulletPowerNew[levelNum - 1][3];
		instance->setM_bulletPower(powerOfBullet);
		instance->setM_BulletNum(2);
        manualCannon = ManualCannon::createCannon();
        manualCannon->initConnon(powerOfBullet , u_data->getCannonID(), 2, m_bCDTime);
        this->addChild(manualCannon, MANUALCANNON_zOrder);
        manualCannon->manualNum = 1;
		manualCannon->setDelegateGame(this);
        m_bPaused = false;
		m_bPaused1 = false;
        m_bCDTime = false;
		m_bSpeedUp = false;
		m_nSpeedIndex = 0;
        isBoom = false;
		m_bNormalFish = true;

		//随机任务鱼的ID
		this->randFishID();
		//刷新任务的界面
		m_pPublicLayer->refreshManyTask();
        
        this->addWaitIn();
        this->schedule(schedule_selector(MainScene::addAutoCannon), 0.1f);//加载虚拟玩家
		this->schedule(schedule_selector(MainScene::updateUseSkill), 1);
    }
    
    instance->m_fishVect.clear(); //清空容器
    
    MusicData* music = MusicData::getInstance();
	int music_num = rand() % 7;
	music->playMusic(music_num);

    //读取PE的plist
    const std::string testFilePath = FileUtils::getInstance()->fullPathForFilename("FishTotalBody.plist");
    plistDic = __Dictionary::createWithContentsOfFile(testFilePath.c_str());
    plistDic->retain();
    instance->setBodiesDic(dynamic_cast<__Dictionary*>(plistDic->objectForKey("bodies"))) ;
    
	m_nSkillIndex = 0;
    m_nCurLockUID = 0;
	m_nCurrentGameTime = 0;
	
    this->scheduleUpdate();
    //加载鱼
    this->schedule(schedule_selector(MainScene::updateFish), 0.05f);

    this->schedule(schedule_selector(MainScene::recordLastCoin), 1);//设置检测时间步
    this->schedule(schedule_selector(MainScene::updateUserdata), 10.0f);
    if (levelNum == 1) 
	{
        peak = limitCoin[levelNum]*0.4;
    }
	else
        peak = limitCoin[levelNum]*0.1;
    //log("peak = %d", peak);
    if (u_data->getGoldNum() >= peak) {
        isthrough = true;
    }else
        isthrough = false;
    
    //刷新技能数量
	m_pPublicLayer->refreshSkillNum();

	//补贴数据的初始化
	tm* pTime = GetTime1();
	int nMon = pTime->tm_mon + 1;
	int nDay = pTime->tm_mday;
	int nCurData = nMon*100 + nDay;

	//上次补贴的日期
	int nSubTime = UserData::getInstance()->getSubsidiesTime();
	int nSubIndex = UserData::getInstance()->getSubsidiesIndex();

	//转换成日期
	int nLastData = nSubTime / 1000000;

	if(nCurData > nLastData)
	{
		UserData::getInstance()->setSubsidiesIndex(0);
		UserData::getInstance()->setSubsidiesTime(nCurData * 1000000);
		UserData::getInstance()->save();
	}

	if(instance->getRoomNum() == 2)
	{
		music->pauseMusic();
		UserData::getInstance()->setSound(0);
		//添加遮罩
		this->addChild(ShiledLayer::create(), 1001, kTagShiledLayer);
	}

    
	this->setKeyboardEnabled(true);

    return true;
}
void MainScene::addWaitIn()
{
    //等待加入
    Sprite* sp_nomal1 = Sprite::create("waiting_player_2.png");
    Sprite* sp_select1 = Sprite::create("waiting_player_2.png");
    MenuItemSprite* item_sp1 = MenuItemSprite::create(sp_nomal1, sp_select1, CC_CALLBACK_1(MainScene::changeManualPosition,this,  Point(size.width/4, 43)));
    item_sp1->setTag(1);
    auto menu1 = MyMenu::create(item_sp1, nullptr);
    menu1->setPosition(Point(size.width/4, 43));
    menu1->setAnchorPoint(Point(0.0f, 0.0f));
    this->addChild(menu1, WAITMENU_zOrder);
    menu1->setScale(SCALE);
	menu1->setIsChosed(false);
    
    Sprite* sp_nomal2 = Sprite::create("waiting_player_2.png");
    Sprite* sp_select2 = Sprite::create("waiting_player_2.png");
    MenuItemSprite* item_sp2 = MenuItemSprite::create(sp_nomal2, sp_select2, CC_CALLBACK_1(MainScene::changeManualPosition,this,  Point(size.width/4 * 3, 43)));
    item_sp2->setTag(2);
    auto menu2 = MyMenu::create(item_sp2, nullptr);
    menu2->setPosition(Point(size.width/4 * 3, 43));
    menu2->setAnchorPoint(Point(0.0f, 0.0f));
    this->addChild(menu2, WAITMENU_zOrder);
    menu2->setScale(SCALE);
	menu2->setIsChosed(false);
    
    Sprite* sp_nomal3 = Sprite::create("waiting_player_2.png");
    Sprite* sp_select3 = Sprite::create("waiting_player_2.png");
    MenuItemSprite* item_sp3 = MenuItemSprite::create(sp_nomal3, sp_select3, CC_CALLBACK_1(MainScene::changeManualPosition,this,  Point(size.width/4, size.height - 43)));
    item_sp3->setTag(3);
    auto menu3 = MyMenu::create(item_sp3, nullptr);
    menu3->setPosition(Point(size.width/4, size.height - 43));
    menu3->setAnchorPoint(Point(0.0f, 0.0f));
    this->addChild(menu3, WAITMENU_zOrder);
    menu3->setScale(SCALE);
	menu3->setIsChosed(false);
    
    
    Sprite* sp_nomal4 = Sprite::create("waiting_player_2.png");
    Sprite* sp_select4 = Sprite::create("waiting_player_2.png");
    MenuItemSprite* item_sp4 = MenuItemSprite::create(sp_nomal4, sp_select4, CC_CALLBACK_1(MainScene::changeManualPosition,this,  Point(size.width/4 * 3, size.height - 43)));
    item_sp4->setTag(4);
    auto menu4 = MyMenu::create(item_sp4, nullptr);
    menu4->setPosition(Point(size.width/4 * 3, size.height - 43));
    menu4->setAnchorPoint(Point(0.0f, 0.0f));
    this->addChild(menu4, WAITMENU_zOrder);
    menu4->setScale(SCALE);
	menu4->setIsChosed(false);
    
    menuPosVect.pushBack(menu1);
    menuPosVect.pushBack(menu2);
    menuPosVect.pushBack(menu3);
    menuPosVect.pushBack(menu4);
    
    Sprite* lock_sp = Sprite::create("lock_arrow.png");
    manualCannon->addChild(lock_sp);
    lock_sp->setScale(SCALE);
    lock_sp->setTag(LOCK_tag);
    lock_sp->setPosition(Vec2(manualCannon->getContentSize().width/2, 100));
    
    int manu_choseNum = rand()%4;
    manualCannon->setPosition(menuPosVect.at(manu_choseNum)->getPosition());
    manualCannon->setM_chosePositionNum(manu_choseNum);
	manualCannon->changeCannonPos();
    menuPosVect.at(manu_choseNum)->setIsChosed(true);
    menuPosVect.at(manu_choseNum)->setVisible(false);
    if (manu_choseNum == 0||manu_choseNum == 1) 
	{
        manualCannon->setRotation(0);
    }
	else if (manu_choseNum == 2 || manu_choseNum == 3) 
	{
        manualCannon->setRotation(180);
        manualCannon->spell_score->setRotation(180);
        manualCannon->spell_power->setRotation(180);
        
        //log("%f, %f", manualCannon->getAnchorPoint().x, manualCannon->getAnchorPoint().y);
    }
    MoveBy* move_by = MoveBy::create(1, Vec2(0, 20));
    Sequence* move_seq = Sequence::create(move_by, move_by->reverse(), nullptr);
    Repeat* move_repeat = Repeat::create(move_seq, 5);
    FadeTo* mFade = FadeTo::create(1, 0);
    Sequence* seq = Sequence::create(move_repeat, mFade,CallFunc::create([&]{
        manualCannon->getChildByTag(LOCK_tag)->removeFromParent();
    }), nullptr);
    lock_sp->runAction(seq);

	this->refreshSkillMenuPosition();
}
void MainScene::changeManualPosition(Ref* ref, Point pos)
{
    Node* btn = dynamic_cast<Node*>(ref);
    int tag = btn->getTag();
    menuPosVect.at(manualCannon->getM_chosePositionNum())->setIsChosed(false);
    menuPosVect.at(manualCannon->getM_chosePositionNum())->setVisible(true);
    if (tag == 1) 
	{
        menuPosVect.at(0)->setVisible(false);
        menuPosVect.at(0)->setIsChosed(true);
        manualCannon->setPosition(menuPosVect.at(0)->getPosition());
        manualCannon->setRotation(0);
        manualCannon->spell_score->setRotation(0);
        manualCannon->spell_power->setRotation(0);
        manualCannon->setM_chosePositionNum(0);
    }
	else if (tag == 2) 
	{
        menuPosVect.at(1)->setVisible(false);
        menuPosVect.at(1)->setIsChosed(true);
        manualCannon->setPosition(menuPosVect.at(1)->getPosition());
        manualCannon->setRotation(0);
        manualCannon->spell_score->setRotation(0);
        manualCannon->spell_power->setRotation(0);
        manualCannon->setM_chosePositionNum(1);
    }
	else if (tag == 3) 
	{
        menuPosVect.at(2)->setVisible(false);
        menuPosVect.at(2)->setIsChosed(true);
        manualCannon->setPosition(menuPosVect.at(2)->getPosition());
        manualCannon->setRotation(180);
        manualCannon->spell_score->setRotation(180);
        manualCannon->spell_power->setRotation(180);
        manualCannon->setM_chosePositionNum(2);
    }
	else if (tag == 4) 
	{
        menuPosVect.at(3)->setVisible(false);
        menuPosVect.at(3)->setIsChosed(true);
        manualCannon->setPosition(menuPosVect.at(3)->getPosition());
        manualCannon->setRotation(180);
        manualCannon->spell_score->setRotation(180);
        manualCannon->spell_power->setRotation(180);
        manualCannon->setM_chosePositionNum(3);
    }
	manualCannon->changeCannonPos();
	this->refreshSkillMenuPosition();
//    log("tag = %d, x = %f, y = %f", tag, pos.x, pos.y);
}
void MainScene::addAutoCannon(float dt)
{
	int nCurNum = this->getCurrentCannonNum();
	int nTemp = 10;
	if(nCurNum < 2)
	{
		nTemp = 10;
	}
	else if(nCurNum == 2)
	{
		nTemp = 100;
	}
	else
	{
		nTemp = 8;
	}

	bool bAdd = rand()%nTemp < 8 ? true : false;
	
    if (bAdd) 
	{
        int nID = rand()%3;
        int choseOnePos = rand()%4;
		addAutoCannonByID(nID, choseOnePos);
    }
}
void MainScene::recordLastCoin(float delta)
{
	m_nCurrentGameTime ++;
	
	if(m_nCurrentGameTime == 1)
	{
		if(UserData::getInstance()->getFirstEnterGame() == 0)
		{
			m_pPublicLayer->showGoldGiftLayer();
			/*UserData::getInstance()->setFirstEnterGame(1);
			UserData::getInstance()->increaseGoldNum(10000);
			UserData::getInstance()->save();
			ParticleSystemQuad* pBoom = ParticleSystemQuad::create("particle/gold_particle0.plist");
			pBoom->setPosition(Vec2(size.width / 2.0f, size.height / 3.0f * 2.0f));
			this->addChild(pBoom, COIN_zOrder + 10000);
			this->refreshGoldAndDiamonds();*/
		}
	}
	else if(m_nCurrentGameTime == 30)
	{
		if(UserData::getInstance()->getFirstEnterGame() == 1)
		{
			m_pPublicLayer->showGiftLayer();
			UserData::getInstance()->setFirstEnterGame(2);
			UserData::getInstance()->save();
		}
	}
	else if(m_nCurrentGameTime == 60)
	{
		if(UserData::getInstance()->getFirstEnterGame() == 2)
		{
			m_pPublicLayer->showFirstGiftLayer();
			UserData::getInstance()->setFirstEnterGame(3);
			UserData::getInstance()->save();
		}
	}
	



    //log("===========%d, %d===========", isActivation, isCountdownApper);
    if (!isActivation&&!isCountdownApper)
    {
        recordSleepTime++;
        if (recordSleepTime == 20 && instance->getRoomNum() == 2)
        {
            isCountdownApper = true;
            CountdownLayer* countdownLayer = CountdownLayer::createCountdownLayer();
            countdownLayer->setTag(CountDown_tag);
            this->addChild(countdownLayer, COUNTDOWN_zOrder);
        }
        
    }
    if (isActivation&&isCountdownApper)
    {
        isActivation = false;
        isCountdownApper = false;
        recordSleepTime = 0;
        this->getChildByTag(CountDown_tag)->removeFromParentAndCleanup(true);
    }
    last_coin = u_data->getGoldNum();
    
    Fire* fire1 = dynamic_cast<Fire*>(this->getChildByTag(FIRE1_tag));
    Fire* fire2 = dynamic_cast<Fire*>(this->getChildByTag(FIRE2_tag));
    Fire* fire3 = dynamic_cast<Fire*>(this->getChildByTag(FIRE3_tag));
    if (fire1 && fire1->totalCorn < fire1->bulletPower && fire1->getCouldShoot())
    {
        //fire1->unscheduleAllSelectors();
		fire1->setCouldShoot(false);
        CallFunc* func1 = CallFunc::create([&]{
            Fire* fire1 = dynamic_cast<Fire*>(this->getChildByTag(FIRE1_tag));
            if (fire1) {
                menuPosVect.at(fire1->getf_chosePositionNum())->setIsChosed(false);
                menuPosVect.at(fire1->getf_chosePositionNum())->setVisible(true);
       
                fire1->removeFromParent();
                fire1 = nullptr;
            }
            
        });
        int time1 = rand()%10 + 4;
        Sequence* seq1 = Sequence::create(DelayTime::create(time1), func1, nullptr);
        fire1->runAction(seq1);
    }
    if (fire2&&fire2->totalCorn < fire2->bulletPower && fire2->getCouldShoot())
    {
        fire2->setCouldShoot(false);
        CallFunc* func2 = CallFunc::create([&]{
            Fire* fire2 = dynamic_cast<Fire*>(this->getChildByTag(FIRE2_tag));
            if (fire2)
            {
                menuPosVect.at(fire2->getf_chosePositionNum())->setIsChosed(false);
                menuPosVect.at(fire2->getf_chosePositionNum())->setVisible(true);
               
                fire2->removeFromParent();
                fire2 = nullptr;
            }
            
        });
        int time2 = rand()%10 + 4;
        Sequence* seq2 = Sequence::create(DelayTime::create(time2), func2, nullptr);
        fire2->runAction(seq2);
    }
    if (fire3&&fire3->totalCorn < fire3->bulletPower && fire3->getCouldShoot())
    {
        fire3->setCouldShoot(false);
        CallFunc* func3 = CallFunc::create([&]{
            Fire* fire3 = dynamic_cast<Fire*>(this->getChildByTag(FIRE3_tag));
            if (fire3)
            {
                menuPosVect.at(fire3->getf_chosePositionNum())->setIsChosed(false);
                menuPosVect.at(fire3->getf_chosePositionNum())->setVisible(true);
                
                fire3->removeFromParent();
                fire3 = nullptr;
            }
           
        });
        int time3 = rand()%10 + 4;
        Sequence* seq3 = Sequence::create(DelayTime::create(time3), func3, nullptr);
        fire3->runAction(seq3);
    }
}

//添加鱼群  前四种随机选择并列走或跟随走 并列走只选择贝塞尔路线 跟随走只选择样条  后几种选贝塞尔
void MainScene::updateFish(float dt)
{
    int arc_energy = rand()%100+400;
    int arc_scope = rand()%200+1000;
    int arc_three = rand()%120+400;
    int arc_four = rand()%120+500;
    int arc_oneNet = rand()%120+600;
    int arc_same = rand()%120+500;
    
	//奖金的释放
	if(Manager::getInstance()->getReleaseBoxFish())
	{
		FishBezier* fish = FishBezier::createFishBezier(BOX_FISH);
		this->addChild(fish, ZORDER_FISH + arc_num);
		fish->initFishBezier(BOX_FISH);
		fish->moveWithBezier();
		fish->setDelegateGame(this);
		instance->m_fishVect.pushBack(fish);       

		Manager::getInstance()->setReleaseBoxFish(false);
	}

    if (m_bPaused || m_bPaused1)
	{
        return;
    }

	if (isBoom)
	{
		return;
	}

	if(!isBoom && !m_bNormalFish)
	{
		m_bNormalFish = true;
		int fishnum = (int)instance->m_fishVect.size();
		for (int i = 0; i < fishnum; i++)
		{
			auto fish = dynamic_cast<FishBase*>(instance->m_fishVect.at(i));
			if (fish->getTag() != NSFISH)
			{
				m_bNormalFish = false;
				break;
			}
		}
	}

	if(!m_bNormalFish)
	{
		log("-----   !m_bNormalFish------------");
		return;
	}
	else
	{
		int nMaxNum = 30;
		if(Manager::getInstance()->getRoomNum() == 2)
			nMaxNum = 23;
		if (instance->m_fishVect.size() < nMaxNum) 
		{
            arc_num = m_recordAddFishRime % 340;
			if(instance->getRoomNum() == 1)
			{
				bool bSmall = rand()%3 < 2 ? true : false;
				if(bSmall)
				{
					arc_num = rand() % 16;
				}
				else
				{
					arc_num = rand() % 17 + 16;
				}
			}
			else
			{
				arc_num = rand() % 38;
				if(arc_num > 32)
					arc_num -= 5;
			}
			
			 if (arc_num < 3 ) 
			{
                FishSpline* fish = FishSpline::createFishSpline(arc_num);
                this->addChild(fish, ZORDER_FISH + arc_num);
                fish->initFishSpline(arc_num);
                fish->moveSpline();
				fish->setDelegateGame(this);
                instance->m_fishVect.pushBack(fish);
                
                DelayTime* delay = CCDelayTime::create(0.5);
                CallFuncN* funcN = CallFuncN::create(CC_CALLBACK_1(MainScene::addFishSpline, this, fish->fb_splineArray));
                Sequence* seq = Sequence::create(delay, funcN,delay, funcN,delay, funcN,  nullptr);
                fish->runAction(seq);
                
                FishBezier* fish_bezier = FishBezier::createFishBezier(arc_num);
                this->addChild(fish_bezier, ZORDER_FISH + arc_num);
                fish_bezier->initFishBezier(arc_num);
                fish_bezier->moveWithBezier();
                instance->m_fishVect.pushBack(fish_bezier);
                fish_bezier->setDelegateGame(this);
                for (int i = 0; i < 5; i++)
				{
                    FishBezier* fish_group = FishBezier::createFishBezier(arc_num);
                    this->addChild(fish_group, ZORDER_FISH + arc_num);
                    fish_group->initFishBezier(arc_num);
                    fish_group->fb_startPoint = fish_bezier->fb_startPoint;
                    fish_group->fb_endPoint = fish_bezier->fb_endPoint;
                    fish_group->fb_controlPoint1 = fish_bezier->fb_controlPoint1;
                    fish_group->fb_controlPoint2 = fish_bezier->fb_controlPoint2;
                    fish_group->moveWithBezier();
                    fish_group->setDelegateGame(this);
                    instance->m_fishVect.pushBack(fish_group);
                }
            }
			else if (arc_num >= 3 && arc_num < 33)
            {
                FishBezier* fish = FishBezier::createFishBezier(arc_num);
                //fish->bodiesDic = fishbodiesDic;
                this->addChild(fish, ZORDER_FISH + arc_num);
                fish->initFishBezier(arc_num);
                fish->moveWithBezier();
                fish->setDelegateGame(this);
                instance->m_fishVect.pushBack(fish);
            }
            //能量炮
            if (m_recordAddFishRime%arc_energy == 0 && m_recordAddFishRime != 0)
			//if (m_recordAddFishRime%5 == 0 && m_recordAddFishRime != 0)
			{
                
                EnergyCannonCard* enegyCard = EnergyCannonCard::create();
                this->addChild(enegyCard, ZORDER_FISH + 20);
                enegyCard->initEnergyCannonCard(ENERGYCANNON);
                enegyCard->moveWithBezier();
                enegyCard->setDelegateGame(this);
                instance->m_fishVect.pushBack(enegyCard);
            }
            //全屏炸弹
            if (m_recordAddFishRime%arc_scope == 0&& m_recordAddFishRime != 0) 
			{
                ScopeFish* scopeFish = ScopeFish::create();
                this->addChild(scopeFish, ZORDER_FISH + 21);
                scopeFish->initScopeFish(SCOPEFISH);
                scopeFish->moveWithBezier();
                scopeFish->setDelegateGame(this);
                instance->m_fishVect.pushBack(scopeFish);
                
            }
            //大三元
            if (m_recordAddFishRime %arc_three == 0&& m_recordAddFishRime != 0) 
			{
                int arc_path = rand()%2;
                MoveBy* by = MoveBy::create(15, Vec2(sanyuansixi[arc_path][0], sanyuansixi[arc_path][1]));
                Sequence* seq = Sequence::create(by, CallFuncN::create(CC_CALLBACK_1(MainScene::removeSanYuanSiXi, this)), nullptr);
                Speed* speedTemp = Speed::create(seq, 1.0f);
                SanYuanSiXi* fish_three1 = SanYuanSiXi::create();
                fish_three1->setPosition(Vec2(sanyuansixi[arc_path][2], 100));
                this->addChild(fish_three1, ZORDER_FISH + 21);
                int fish_num = rand()%10+5;
                fish_three1->initSanYuanSiXi(SANYUAN, fish_num);
				auto pTempSpeed = speedTemp->clone();
				pTempSpeed->retain();
                fish_three1->setMoveSpeed(pTempSpeed);
                fish_three1->runAction(pTempSpeed);
                instance->d_threeVect.pushBack(fish_three1);
                instance->m_fishVect.pushBack(fish_three1);
                fish_three1->setDelegateGame(this);
                
                for (int i = 0; i < 2; i++) 
				{
                    SanYuanSiXi* fish_three2 = SanYuanSiXi::create();
                    this->addChild(fish_three2, ZORDER_FISH + 21);
                    fish_three2->setPosition(Vec2(sanyuansixi[arc_path][2], fish_three1->getPositionY() + 100*sin(M_PI/2 + i*M_PI)));
                    fish_three2->initSanYuanSiXi(SANYUAN, fish_num);
					auto pTempSpeed1 = speedTemp->clone();
					pTempSpeed1->retain();
                    fish_three2->setMoveSpeed(pTempSpeed1);
                    fish_three2->runAction(pTempSpeed1);
                    fish_three2->setDelegateGame(this);
                    instance->d_threeVect.pushBack(fish_three2);
                    instance->m_fishVect.pushBack(fish_three2);
                    
                }
                
            }
            //四喜
            if (m_recordAddFishRime%arc_four == 0 && m_recordAddFishRime != 0) 
			{
                int arc_path = rand()%2;
                MoveBy* by = MoveBy::create(15, Vec2(sanyuansixi[arc_path][0], sanyuansixi[arc_path][1]));
                Sequence* seq = Sequence::create(by, CallFuncN::create(CC_CALLBACK_1(MainScene::removeSanYuanSiXi, this)), nullptr);
                Speed* speedTemp = Speed::create(seq, 1.0f);
                
                int fish_num = rand()%10+5;
                for (int i = 0; i < 2; i++) 
				{
                    SanYuanSiXi* fish_four1 = SanYuanSiXi::create();
                    fish_four1->setPosition(Vec2(sanyuansixi[arc_path][2], 240 + 60*sin(M_PI/2 + i*M_PI)));
                    instance->getM_layer()->addChild(fish_four1, ZORDER_FISH + 21);
                    fish_four1->setDelegateGame(this);
                    fish_four1->initSanYuanSiXi(SIXI, fish_num);
					auto pTempSpeed = speedTemp->clone();
					pTempSpeed->retain();
                    fish_four1->setMoveSpeed(pTempSpeed);
                    fish_four1->runAction(pTempSpeed);
                    
                    instance->d_fourVect.pushBack(fish_four1);
                    instance->m_fishVect.pushBack(fish_four1);
                }
                for (int j = 0; j < 2; j++) 
				{
                    SanYuanSiXi* fish_four2 = SanYuanSiXi::create();
                    instance->getM_layer()->addChild(fish_four2, ZORDER_FISH + 21);
                    fish_four2->setPosition(Vec2(sanyuansixi[arc_path][2], 240 + 180*sin(M_PI/2 + j*M_PI)));
					auto pTempSpeed1 = speedTemp->clone();
					pTempSpeed1->retain();
                    fish_four2->setMoveSpeed(pTempSpeed1);
                    fish_four2->initSanYuanSiXi(SIXI, fish_num);
                    fish_four2->runAction(pTempSpeed1);
                    fish_four2->setDelegateGame(this);
                    instance->d_fourVect.pushBack(fish_four2);
                    instance->m_fishVect.pushBack(fish_four2);
                }
                
            }
            //一网打尽
            if (m_recordAddFishRime%arc_oneNet == 0&& m_recordAddFishRime != 0) 
			{
                int fish_num = rand()%10+5;
                for (int i = 0; i < 4; i++) 
				{
                    FishGroup* fishgroup = FishGroup::create();
                    this->addChild(fishgroup, ZORDER_FISH + 23);
                    fishgroup->initFishGroup(YIWANGDAJIN, fish_num);
                    fishgroup->moveWithBezier();
                    instance->Y_dajinVect.pushBack(fishgroup);
                    instance->m_fishVect.pushBack(fishgroup);
                    fishgroup->setDelegateGame(this);
                }
            }
            //同类炸弹
            if (m_recordAddFishRime%arc_same == 0&& m_recordAddFishRime != 0) 
			{
                int fish_num = rand()%10+5;
                FishGroup* fishgroup_tonglei = FishGroup::create();
                this->addChild(fishgroup_tonglei, ZORDER_FISH + 24);
                fishgroup_tonglei->initFishGroup(TONGLEIZHAIDAN, fish_num);
                fishgroup_tonglei->moveWithBezier();
                instance->m_fishVect.pushBack(fishgroup_tonglei);
                fishgroup_tonglei->setDelegateGame(this);
                for (int i = 0; i < 4; i++) 
				{
                    FishBezier* fish_bezier = FishBezier::createFishBezier(fish_num);
                    this->addChild(fish_bezier, ZORDER_FISH + fish_num);
                    fish_bezier->initFishBezier(fish_num);
                    fish_bezier->moveWithBezier();
                    instance->m_fishVect.pushBack(fish_bezier);

					fish_bezier->setDelegateGame(this);
                }
                
            }
            
            
        }
            
        m_recordAddFishRime++;
            
        //添加BOSS
        if (m_recordAddFishRime == 2000)
        {
            //是不是话费鱼
			bool bPhoneFare = (rand() % 5 == 0) ? true : false;
			if(instance->getRoomNum() == 1)
			{
				bPhoneFare = false;
			}
				
			if(!bPhoneFare)
			{
				Manager::getInstance()->setCreateBossIndex(Manager::getInstance()->getCreateBossIndex() + 1);
				int arc_boss = rand() % 2;
				int level = rand() % 8 + 1;
				
				if(Manager::getInstance()->getRoomNum() == 2)
					level = Manager::getInstance()->getTaskFishID3() - 32;
				NoticeBoss* notice_boss = NoticeBoss::createNoticeBoss(arc_boss, level);
				notice_boss->setPosition(Vec2(size.width/2, size.height/2));
				this->addChild(notice_boss, ZORDER_TIP);

				FishBezier* fish = FishBezier::createFishBezier(32 + level);
				this->addChild(fish, ZORDER_FISH + arc_num);
				fish->initFishBezier(32 + level);
				fish->moveWithBezier();
				fish->setDelegateGame(this);
				instance->m_fishVect.pushBack(fish);       
			}
			//话费鱼
			else
			{
				NoticeBoss* notice_boss = NoticeBoss::createNoticeBoss(0, TELEPHONE_FARE_FISH);
				notice_boss->setPosition(Vec2(size.width/2, size.height/2));
				this->addChild(notice_boss, ZORDER_TIP);

				TelephoneFareFish* fish = TelephoneFareFish::createTelephoneFareFish();
				this->addChild(fish, ZORDER_FISH + 40);
				fish->initTelephoneFareFish(TELEPHONE_FARE_FISH);
				fish->moveWithBezier();
				fish->setDelegateGame(this);
				instance->m_fishVect.pushBack(fish);      
			}
        }
 
        //开始 鱼潮
        if (m_recordAddFishRime == 4000) 
		{         
            //加载 提示字体 闪烁
            Sprite* warning = Sprite::create("tide_notice.png");
            warning->setPosition(Point(size.width/2, size.height/2));
            this->addChild(warning, ZORDER_TIP);
                
			auto seq1 = Sequence::create(Blink::create(4, 2), 
				CallFuncN::create(CC_CALLBACK_1(MainScene::callBackRemoveSelf, this)), 
				NULL);
            warning->runAction(seq1);
            //停止加鱼
            this->unschedule(schedule_selector(MainScene::updateFish));
            int fishNums = instance->m_fishVect.size();
            for (int i = 0; i < fishNums; i++) 
			{
                FishBase* fishTemp = dynamic_cast<FishBase*>(instance->m_fishVect.at(i));
                fishTemp->getMoveSpeed()->setSpeed(10);
				fishTemp->setResumeSpeed(10.0f);
            }
                
            Sequence* seq = Sequence::create(DelayTime::create(5), 
				CallFuncN::create(CC_CALLBACK_1(MainScene::startBoom, this)), 
				NULL);
            this->runAction(seq);

				

			Sequence* seq2 = Sequence::create(DelayTime::create(4.0f), 
				CallFuncN::create(CC_CALLBACK_1(MainScene::callBackRemoveSelf, this)), 
				NULL);
				
			int nTag = this->getChildByTag(kTagBg_1) ? kTagBg_2 : kTagBg_1;
			int nCurTag = this->getChildByTag(kTagBg_1) ? kTagBg_1 : kTagBg_2;

			m_nCurrentBgID = this->getRandomMapID();
			auto pNewBg = Sprite::create(__String::createWithFormat("bg_%d.png", m_nCurrentBgID)->getCString());
			pNewBg->setPosition(Vec2(size.width + pNewBg->getContentSize().width/2, size.height / 2.0f));
			this->addChild(pNewBg, -1, nTag);

			//鱼潮粒子添加
			//添加粒子
			ParticleSystemQuad* pBoom = ParticleSystemQuad::create("particle/yuchao.plist");
			pBoom->setPosition(Vec2(0, size.height / 2.0f));
			pNewBg->addChild(pBoom, ZORDER_COIN);
			pBoom->runAction(seq2);
				
			pNewBg->runAction(MoveBy::create(4.0f, Vec2(-size.width / 2.0f - pNewBg->getContentSize().width / 2, 0.0f)));

			Sequence* seq4 = Sequence::create(MoveBy::create(4.0f, Vec2(-size.width, 0.0f)), 
				CallFuncN::create(CC_CALLBACK_1(MainScene::callBackRemoveSelf, this)), 
				NULL);
			auto pCurBg = (Sprite*)this->getChildByTag(nCurTag);
			pCurBg->runAction(seq4);

        }

    }
 
}
void MainScene::removeSanYuanSiXi(Node* s_node)
{
    FishBase* sanyuanFish = dynamic_cast<FishBase*>(s_node);
    sanyuanFish->removeFish();
}
void MainScene::startBoom(Node* p)
{
    m_bNormalFish = false;
	isBoom = true;
	log("start booming -----------------");
    //添加 造型鱼
    FishMedol* medol = FishMedol::createFishMedol();
    medol->initFishMedol();
    this->addChild(medol, 0, kTagFishMedol);
	medol->setDelegateGame(this);
    
    medolcdTime = medol->getTotalTime();
    m_recordAddFishRime = 0;//初始化 加载鱼的计数
    delayTimeAddFish = 12;
	m_bSpeedUp = true;

    this->schedule(schedule_selector(MainScene::updateFish), 0.05f);
}
void MainScene::recordBoomTime(float dt)
{
//    this->schedule(schedule_selector(MainScene::updateFish), 1);
//    m_recordAddFishRime = 0;//初始化 加载鱼的计数
//    if (instance->getRoomNum() == 1) {
//        delayTimeAddFish = 25;
//    }else if (instance->getRoomNum() == 2) {
//        delayTimeAddFish = 25;
//    }
//    this->unschedule(schedule_selector(MainScene::recordBoomTime));

    
}
void MainScene::updateUserdata(float dt)
{
	//检测是否达到峰值
	int nOverPeak = UserData::getInstance()->getOverGoldPeak();

	if(nOverPeak == 0)
	{
		int nCharge = UserData::getInstance()->getRechargeAmount();
		int nCurrentGold = UserData::getInstance()->getGoldNum();
		//充值小于等于5元的时候峰值为50000
		if(nCharge <= 5)
		{
			if(nCurrentGold > 30000)
			{
				UserData::getInstance()->setOverGoldPeak(1);
			}
		}
		else
		{
			if(nCurrentGold > nCharge * 10000)
			{
				UserData::getInstance()->setOverGoldPeak(1);
			}
		}
	}

	UserData::getInstance()->save();
}
//鱼潮之后加速
void MainScene::callBackResumeSpeed()
{
	m_bSpeedUp = false;
}
void MainScene::addFishSpline(Node* sp, PointArray* pArray)
{
    FishSpline* d_fish = dynamic_cast<FishSpline*>(sp);
    FishSpline* fish = FishSpline::createFishSpline(d_fish->fb_Kind);
    this->addChild(fish, d_fish->fb_Kind);
    fish->initFishSpline(d_fish->fb_Kind);
	
	for (int i = 0; i < fish->fb_splineArray->count(); i++)
	{
		fish->fb_splineArray->removeControlPointAtIndex(i);
	}

	for (int i = 0; i < d_fish->fb_splineArray->count(); i++)
	{
		Vec2 splinePos = d_fish->fb_splineArray->getControlPointAtIndex(i);
		fish->fb_splineArray->addControlPoint(splinePos);
	}

    fish->moveSpline();
	fish->setDelegateGame(this);
    
    
    instance->m_fishVect.pushBack(fish);//把鱼添加到数组中
}
//随机获取一个地图ID
int MainScene::getRandomMapID()
{
	int nID = m_nCurrentBgID;
	while (nID == m_nCurrentBgID)
	{
		nID = rand() % 10 + 1;
	}
	return nID;
}
//移除炮台
void MainScene::callBackRemoveFire(Node* pNode)
{
	Fire* fire1 = dynamic_cast<Fire*>(pNode);
	if (fire1)
	{
		menuPosVect.at(fire1->getf_chosePositionNum())->setIsChosed(false);
		menuPosVect.at(fire1->getf_chosePositionNum())->setVisible(true);
		fire1->removeFromParentAndCleanup(true);
		fire1 = nullptr;
	}
}
//根据ID添加自动炮
void MainScene::addAutoCannonByID(int nID, int choseOnePos)
{
	int nTag = FIRE1_tag;
	if(nID == 0)
	{
		nTag = FIRE1_tag;
	}
	else if(nID == 1)
	{
		nTag = FIRE2_tag;
	}
	else
	{
		nTag = FIRE3_tag;
	}
	m_nCurrentCannonTag = nTag;
	Fire* pFire = dynamic_cast<Fire*>(this->getChildByTag(nTag));
	int nRand = 72000 + rand() % 18000;
	if (pFire && choseOnePos == pFire->getf_chosePositionNum() && pFire->getRecordAppearTime() > nRand)
	{
		int nAppearTime = pFire->getRecordAppearTime();
		log("------(%d)------(%d)----------", nRand, nAppearTime);
		pFire->setCouldShoot(false);
		Sequence* seq1 = Sequence::create(DelayTime::create(2.5f), 
			CallFuncN::create(CC_CALLBACK_1(MainScene::callBackRemoveFire, this)), 
			NULL);
		pFire->runAction(seq1);
	}
	else if (!pFire && !menuPosVect.at(choseOnePos)->getIsChosed()) 
	{
		Fire* pFire = Fire::createFire();
		pFire->setTag(nTag);
		pFire->initFire(powerOfBullet, rand()%5, 1);
		this->addChild(pFire, MANUALCANNON_zOrder);
		pFire->fireNum = nTag - 111;
		pFire->setPosition(menuPosVect.at(choseOnePos)->getPosition());
		pFire->setf_chosePositionNum(choseOnePos);
		pFire->changeCannonPos();

		if(this->getChildByTag(kTagShiledLayer))
		{
			pFire->randTaskNum();
		}

		menuPosVect.at(choseOnePos)->setIsChosed(true);
		menuPosVect.at(choseOnePos)->setVisible(false);

		if (choseOnePos == 2 || choseOnePos == 3) 
		{
			pFire->setRotation(180);
			pFire->spell_score->setRotation(180);
			pFire->spell_power->setRotation(180);
		}
	}

		
}
//获取当前自动炮的数量
int MainScene::getCurrentCannonNum()
{
	int nNum = 0;
	auto pFire1 = this->getChildByTag(FIRE1_tag);
	auto pFire2 = this->getChildByTag(FIRE2_tag);
	auto pFire3 = this->getChildByTag(FIRE3_tag);

	if(pFire1)
		nNum ++;
	if(pFire2)
		nNum ++;
	if(pFire3)
		nNum ++;

	return nNum;
}

#pragma mark 碰撞检测
bool MainScene::onContactBegin(const cocos2d::PhysicsContact &contact)
{
    Vec2 pos = this->getLockedFishPosition();
	FishBase* fish = NULL;
	Bullet* m_bullet = NULL;
    
    if (((contact.getShapeA()->getBody()->getCategoryBitmask() & fishMask) == fishMask && 
		(contact.getShapeB()->getBody()->getCategoryBitmask() & bulletMask) == bulletMask) || 
		((contact.getShapeB()->getBody()->getCategoryBitmask() & fishMask) == fishMask && 
		(contact.getShapeA()->getBody()->getCategoryBitmask() & bulletMask) == bulletMask)) 
	{
        if (contact.getShapeA()->getBody()->getNode()->getTag() == BULLET_TAG && contact.getShapeB()->getBody()->getNode()->getTag() == FISH_TAG) 
		{
            m_bullet = dynamic_cast<Bullet*>(contact.getShapeA()->getBody()->getNode());
            fish = dynamic_cast<FishBase*>(contact.getShapeB()->getBody()->getNode());
        }
        if (contact.getShapeB()->getBody()->getNode()->getTag() == BULLET_TAG && contact.getShapeA()->getBody()->getNode()->getTag() == FISH_TAG) 
		{
            fish = dynamic_cast<FishBase*>(contact.getShapeA()->getBody()->getNode());
            m_bullet = dynamic_cast<Bullet*>(contact.getShapeB()->getBody()->getNode());
        }
        if (m_bullet->getB_isCatch() || fish->getIsCatch()) 
		{
            return false;
        }
       
		if(m_bullet->getIsCollision())
        {
			//角色自己的碰撞问题
			if( (pos.x != 0 && pos.y != 0) && !fish->getLocked() && m_bullet->atNum == 1)
			{
				return false;
			}
			Fire* fire1 = dynamic_cast<Fire*>(this->getChildByTag(FIRE1_tag));
			Fire* fire2 = dynamic_cast<Fire*>(this->getChildByTag(FIRE2_tag));
			Fire* fire3 = dynamic_cast<Fire*>(this->getChildByTag(FIRE3_tag));
			//机器人子弹锁定的碰撞(机器人锁定鱼了，但这个鱼不是被锁定的就返回)
			if(fire1 && m_bullet->atNum == 2)
			{
				if(fire1->getLockFish() && !fish->getAimedByRobot(fire1->getf_chosePositionNum()))
				{
					return false;
				}
			}
			else if(fire2 && m_bullet->atNum == 3)
			{
				if(fire2->getLockFish() && !fish->getAimedByRobot(fire2->getf_chosePositionNum()))
				{
					return false;
				}
			}
			else if(fire3 && m_bullet->atNum == 4)
			{
				if(fire3->getLockFish() && !fish->getAimedByRobot(fire3->getf_chosePositionNum()))
				{
					return false;
				}
			}

            //设置鱼 打击变色
            fish->setShader();
            //出现渔网
            Point netPoint = m_bullet->getPosition() + m_bullet->normalizedShootDis*10;
            NetSprite* net = NetSprite::createNet();
            net->initNet(m_bullet->bulletType, m_bullet->num);
            net->setPosition(netPoint);
            this->addChild(net, ZORDER_NET);
            
			//四人奖金池累计
			if(Manager::getInstance()->getRoomNum() == 2 && !Manager::getInstance()->getTaskOver())
			{
				float fRate = 0.3f;
				int nLevelNum = Manager::getInstance()->getTwo_LevelNum();
				if(nLevelNum == 2)
					fRate = 0.5f;
				else if(nLevelNum == 2)
					fRate = 0.75f;
				else if(nLevelNum == 2)
					fRate = 1.0f;
				
				Manager::getInstance()->setManyPoolGoldNum(Manager::getInstance()->getManyPoolGoldNum() + m_bullet->bulletPower * fRate);
			}

            
            int globalVal = fishValue[fish->fb_Kind];
			if(fish->fb_Kind == TELEPHONE_FARE_FISH)
				globalVal = 101;

            if (fish->randomCatch(globalVal, m_bullet->atNum))
            {
				fish->setBulletPower(m_bullet->bulletPower);
                fish->setIsCatch(true);
				fish->resumeSchedulerAndActions();
                //捕捉到能量炮的处理
                if (fish->fb_Kind == ENERGYCANNON)
                {
                    Vec2 posAim = Vec2::ZERO;
                    if (m_bullet->atNum == 1 && !manualCannon->getIsEnergyCannon())
                    {
                        manualCannon->setIsEnergyCannon(true);
                        manualCannon->setEnergyCannon();
						posAim = manualCannon->getPosition();
                    }
					else if (m_bullet->atNum == 2 &&fire1&& !fire1->getIsEnergyCannon())
                    {
                        fire1->setIsEnergyCannon(true);
                        fire1->setEnergy();
						posAim = fire1->getPosition();
                    }
					else if (m_bullet->atNum == 3 &&fire2&& !fire2->getIsEnergyCannon())
                    {
                        fire2->setIsEnergyCannon(true);
                        fire2->setEnergy();
						posAim = fire2->getPosition();
                    }
					else if (m_bullet->atNum == 4 &&fire3&& !fire3->getIsEnergyCannon())
                    {
                        fire3->setIsEnergyCannon(true);
                        fire3->setEnergy();
						posAim = fire3->getPosition();
                    }
					else
					{
						log("--------------");
					}
                    if(!posAim.equals(Vec2::ZERO))
						this->catchEnergyCannonAni(posAim);
                }
				//捕捉到话费的处理
				else if (fish->fb_Kind == TELEPHONE_FARE_FISH)
				{
					if(m_bullet->atNum == 1)
					{
						int ncurFare = UserData::getInstance()->getCurrentTelephoneFare();
						UserData::getInstance()->setCurrentTelephoneFare(ncurFare + fish->getFishTeleploneFare());
						UserData::getInstance()->save();
					}
					this->catchTelephoneFareAni(m_bullet->originPoint, fish->getFishTeleploneFare());
				}
				//捕捉到宝箱的处理
				else if (fish->fb_Kind == BOX_FISH)
				{
					if(m_bullet->atNum == 1)
					{
						m_pPublicLayer->showGetBoxLayer();
					}
				}
                //锁定解锁
                if (fish->getUID() == m_nCurLockUID)
                {
                    manualCannon->isShoot = false;
					fish->setLocked(false);
					fish->removeChildByTag(103);
                }
                    
                //删除刚体
                int num = 0;
                if(fish->getChoseOne() == SANYUAN) 
				{
                    int nSize = instance->d_threeVect.size();
                    for (int i = nSize - 1; i >=0; i--)
                    {
                        num++;
                        FishBase* fish_temp = (FishBase*)instance->d_threeVect.at(i);
                        fish_temp->setIsCatch(true);
                        fish_temp->setBindToCannonNum(1);
                        fish_temp->getPhysicsBody()->removeFromWorld();
                        m_world->removeBody(fish_temp->getPhysicsBody());
                        fish_temp->resumeSchedulerAndActions();
                    }
                    instance->d_threeVect.clear();
                }
                else if (fish->getChoseOne() == SIXI) 
				{
                    int nSize = instance->d_fourVect.size();
                    for (int i = nSize - 1; i >=0; i--)
                    {
                        num++;
                        FishBase* fish_temp = (FishBase*)instance->d_fourVect.at(i);
                        fish_temp->setIsCatch(true);
                        fish_temp->setBindToCannonNum(1);
                        fish_temp->getPhysicsBody()->removeFromWorld();
                        m_world->removeBody(fish_temp->getPhysicsBody());
                        fish_temp->resumeSchedulerAndActions();
                    }
                    instance->d_fourVect.clear();
                }
				else
                {
                    num++;
                    fish->getPhysicsBody()->removeFromWorld();
                    m_world->removeBody(fish->getPhysicsBody());
                }
                    
                //单人场数据处理
                if (instance->getRoomNum() == 1)
                {
                    fish->setBindToCannonNum(1);
                    if (fish->fb_Kind < 41) 
					{
                        //经验值
                        for (int j = 0; j < num; j++) 
						{
                            u_data = UserData::getInstance();
							int nOld = u_data->getExp_value();
                            u_data->setExp_value(u_data->getExp_value() + fish_EValue[fish->fb_Kind]);
                            m_pPublicLayer->refreshExperiencePoints(nOld);
                        }
                    }
                        
                        
                    //完善任务
                    int mission = m_pPublicLayer->getTaskIndex(instance->getOne_LevelNum());
                    if (missionData->getFishID() == fish->fb_Kind && mission == 0) 
					{
                        missionData->setFinishNum(missionData->getFinishNum()+1);
                        if (missionData->getFinishNum() == missionData->getFishNum()) 
						{
                            m_pPublicLayer->setFinishTaskIndex(m_pPublicLayer->getTaskIndex(instance->getOne_LevelNum()));
							m_pPublicLayer->finishedOneTask();
                        }
						m_pPublicLayer->refreshTaskNum();
                        u_data->save();                           
                    }
					else if (missionData->getFishID1() == fish->fb_Kind && mission == 1) 
					{
                        missionData->setFinishNum1(missionData->getFinishNum1()+1);
                        if (missionData->getFinishNum1() == missionData->getFishNum1()) 
						{
                            m_pPublicLayer->setFinishTaskIndex(m_pPublicLayer->getTaskIndex(instance->getOne_LevelNum()));
                            missionData->setUnLock(true);
                            //m_pPublicLayer->showTaskLayer();
                            //m_pPublicLayer->showLeftTaskLayer();
                            m_pPublicLayer->finishedTwoTask();
                        }
                        m_pPublicLayer->refreshTaskNum();
                        u_data->save();
                    }
                }//多人场数据处理
                else if (instance->getRoomNum() == 2)
                {
                    fish->setBindToCannonNum(m_bullet->atNum);
                  
					if (m_bullet->atNum != 1)
                    {
                        //特殊玩法鱼 将被虚拟忽略
                        if (fish->fb_Kind < 41)
                        {
                            if (m_bullet->atNum == 2 && fire1)
                            {
                                fire1->totalCorn += fish->value*m_bullet->bulletPower;
                                fire1->setF_coinIsChange(true);    
								fire1->checkTaskFish(fish->fb_Kind);
								if(fire1->getIsFinishTask() && !Manager::getInstance()->getTaskOver())
									this->catchBoxAni(m_bullet->originPoint, false);
                            }
							else if (m_bullet->atNum == 3 && fire2)
                            {
                                fire2->totalCorn += fish->value*m_bullet->bulletPower;
                                fire2->setF_coinIsChange(true);
								fire2->checkTaskFish(fish->fb_Kind);
								if(fire2->getIsFinishTask() && !Manager::getInstance()->getTaskOver())
									this->catchBoxAni(m_bullet->originPoint, false);
                            }
							else if (m_bullet->atNum == 4 && fire3)
                            {
                                fire3->totalCorn += fish->value*m_bullet->bulletPower;
                                fire3->setF_coinIsChange(true);
								fire3->checkTaskFish(fish->fb_Kind);
								if(fire3->getIsFinishTask() && !Manager::getInstance()->getTaskOver())
									this->catchBoxAni(m_bullet->originPoint, false);
                            }
                        } 
						
                    }
					else
					{
						manualCannon->checkTaskFish(fish->fb_Kind);
						if(manualCannon->getIsFinishTask() && !Manager::getInstance()->getTaskOver())
							this->catchBoxAni(m_bullet->originPoint, true);
					}
					int nNum1 = manualCannon->getFinishNum();
					int nNum2 = fire1 ? fire1->getFinishNum() : 0;
					int nNum3 = fire2 ? fire2->getFinishNum() : 0;
					int nNum4 = fire3 ? fire3->getFinishNum() : 0;

					if(fire1)fire1->refreshRanking(nNum1, nNum3, nNum4);
					if(fire2)fire2->refreshRanking(nNum1, nNum2, nNum4);
					if(fire3)fire3->refreshRanking(nNum1, nNum2, nNum3);
					manualCannon->refreshRanking(nNum2, nNum3, nNum4);
                }

                //奖励金币提示牌   
                if (fish->fb_Kind < 30 && fish->fb_Kind > 10)
                {
					//提示牌的位置
                    Point promptPoint;
                    if (m_bullet->originPoint.y < 200)
                    {
                        promptPoint = m_bullet->originPoint + Point(0, 160);
                    }
					else
                    {
                        promptPoint = m_bullet->originPoint - Point(0, 160);
                    }
                    //奖励金币提示牌
                    Sequence* awardseq = Sequence::create(DelayTime::create(1), 
						CallFuncN::create(CC_CALLBACK_1(MainScene::addCoinPrompt, this, fish->fb_Kind, promptPoint, fish->value*m_bullet->bulletPower)), 
						nullptr);
                    this->runAction(awardseq);
                }
				//金币动画和数量改变
                if (fish->fb_Kind < 41)
                {
                    //添加金币
                    Coin* coin = Coin::createCoin( fish->fb_Kind, fish->getPosition(), m_bullet->originPoint);
                    this->addChild(coin, ZORDER_COIN);
                    //添加数字
                    SpellNumber* auto_fish_Num = SpellNumber::createSpellNumber(fish->value*m_bullet->bulletPower, 1);
                    auto_fish_Num->setPosition(fish->getPosition() + Vec2(0.0f, 15.0f));
                    this->addChild(auto_fish_Num, ZORDER_COIN);
                    auto_fish_Num->addAnimation();
						
                    SpellNumber* battary_coin_num = SpellNumber::createSpellNumber(fish->value*m_bullet->bulletPower, 1);
                    battary_coin_num->addAnimation();
                    battary_coin_num->setPosition(Vec2(200, -10));
                    if (m_bullet->originPoint.y > 200)
                    {
                        battary_coin_num->setRotation(180);
                    }
                        
                    if (m_bullet->atNum == 1)
                    {
                        manualCannon->addChild(battary_coin_num);
                    }
					battary_coin_num->setPosition(manualCannon->getAddCoinPosition());
						
                }
				//boss提示牌和宝石、技能的爆率
                if (fish->fb_Kind < 41 && fish->fb_Kind >= 20)
				{
					//提示牌的位置
					Point promptPoint;
					if (m_bullet->originPoint.y < 200)
					{
						promptPoint = m_bullet->originPoint + Point(0, 160);
					}
					else
					{
						promptPoint = m_bullet->originPoint - Point(0, 160);
					}
					auto pBossCoinPrompt = BossCoinPrompt::createCoinPrompt(promptPoint, fish->value*m_bullet->bulletPower);
					this->addChild(pBossCoinPrompt, ZORDER_EFFECT + 2);

					MusicData::getInstance()->playSound(72);

					int nOverPeak = UserData::getInstance()->getOverGoldPeak();
					int nCurGold = UserData::getInstance()->getGoldNum();
					bool bSelf = m_bullet->atNum == 1 ? true : false;
					float fLow = 0.9f;
					float fLow1 = 0.9f;
					int nExtraNum = 0;
					if(nOverPeak == 1 && nCurGold < 20000)
					{
						fLow = 0.4f;
						nExtraNum = 7;
						fLow1 = 0.4f;
						if(m_bullet->bulletPower <= 10)
							nExtraNum = 2;
					}
					else
					{
						fLow = 0.2f;
						nExtraNum = 4;
						fLow1 = 0.3f;
						if(m_bullet->bulletPower <= 10)
							nExtraNum = 1;
					}

					if(CCRANDOM_0_1() < fLow)
					{
						int nRandID = rand()%4;
						int nRandNum = rand()%3 + 1;
							
						this->catchDiamondAndSkillAni(m_bullet->originPoint, fish->getPosition(), nRandID, nRandNum, bSelf);

						if(instance->getRoomNum() == 1 && m_bullet->atNum == 1)
						{
							UserData::getInstance()->increaseSkillNum(nRandID, nRandNum);
						}
						else if(instance->getRoomNum() == 2 && m_bullet->atNum == 1)
						{
							if(nRandID != SKILL_3)
								UserData::getInstance()->increaseSkillNum(nRandID, nRandNum);
						}
					}

					if(CCRANDOM_0_1() < fLow1)
					{
						int nRandNum = rand()%3 + nExtraNum + 1;
						this->catchDiamondAndSkillAni(m_bullet->originPoint, fish->getPosition(), DIAMOND, nRandNum, bSelf);
						if (m_bullet->atNum == 1)
						{
							UserData::getInstance()->increaseDiamonds(nRandNum);
							m_pPublicLayer->refreshSkillNum();
							manualCannon->refreshUserdata();
						}
						else if (m_bullet->atNum == 2 && fire1)
						{
							fire1->setDiamondsNum(fire1->getDiamondsNum() + nRandNum);
							fire1->refreshUserdata();
						}
						else if (m_bullet->atNum == 3 && fire2)
						{
							fire2->setDiamondsNum(fire2->getDiamondsNum() + nRandNum);
							fire2->refreshUserdata();
						}
						else if (m_bullet->atNum == 4 && fire3)
						{
							fire3->setDiamondsNum(fire3->getDiamondsNum() + nRandNum * 2);
							fire3->refreshUserdata();
						}
					}

					m_pPublicLayer->refreshSkillNum();
				}
            }
			
			if(m_bullet->atNum == 1)
					Manager::getInstance()->setBulletNum(Manager::getInstance()->getBulletNum() - 1);
            m_bullet->setB_isCatch(true);
            m_bullet->getPhysicsBody()->removeFromWorld();
		}
       
    }
    
    
    return true;
    
}
void MainScene::adjustTheData()
{
    
    if (!isthrough) {//未达到峰值，波形应呈增长的趋势
        
        //log("1--last_coin = %d, %d,  %d", last_coin, u_data->getGoldNum(), peak);
        if (u_data->getGoldNum() - last_coin < 0) { //当曲线跌落到50以下是，调整曲线走势，改为向上，此时把相对增率降低
            chage_pro -= 5;
        }
        if (chage_pro < 0) {
            chage_pro = 0;
        }
    }
    else if (isthrough){//到达波峰后
        //log("2--last_coin = %d, %d, %d", last_coin, u_data->getGoldNum(), peak);
        if (u_data->getGoldNum() < peak*0.7) {
            peak += limitCoin[levelNum]*0.1;
            if (peak > limitCoin[levelNum]) {
                peak = limitCoin[levelNum];
            }
            isthrough = false;
        }
        if (u_data->getGoldNum() - last_coin > 0) {
            chage_pro += 5;
        }
        if (chage_pro > 100) {
            chage_pro = 100;
        }
        
    }

}
void MainScene::addCoinPrompt(Node* node, int fKind, Point fishPoint, int coinNum)
{
    CoinPrompt* coinprompt = CoinPrompt::createCoinPrompt(fKind, fishPoint, coinNum);
    this->addChild(coinprompt, ZORDER_EFFECT + 1);
}

//检查金币是否足够
void MainScene::checkCoin(int nGold)
{
	int nCurGold = UserData::getInstance()->getGoldNum();

	if(nCurGold < nGold)
	{
		m_pPublicLayer->showCoinNotEnoughLayer();
	}
}
//捕获话费的动画
void MainScene::catchTelephoneFareAni(Vec2 posTo, int nFare)
{
	auto pSprite = Sprite::createWithSpriteFrameName("huafeiyu.png");
	pSprite->setPosition(Vec2(size.width / 2.0f, size.height / 2.0f));
	this->addChild(pSprite, ZORDER_COIN);
	pSprite->setScale(SCALE);

	int nPre = nFare / 10;
	int nLast = nFare % 10;

	//显示钱数
	auto pXmlParser = XMLParser::parseWithFile("ui/loading.xml");
	__String* info = pXmlParser->getString("fare_card");
	std::string strName = __String::createWithFormat(info->getCString(), nPre, nLast)->getCString();

	LabelTTF* pLabel = LabelTTF::create(strName, "Arial", 30);
	pLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
	pLabel->setPosition(Vec2(pSprite->getContentSize().width / 2.0f, pSprite->getContentSize().height / 3.0f * 2.0f));
	pSprite->addChild(pLabel);
	pLabel->setColor(Color3B(255, 255, 0));

	float time = sqrtf((posTo.x - size.width / 2.0f)*(posTo.x - size.width / 2.0f)+(posTo.y - size.height / 2.0f)*(posTo.y - size.height / 2.0f)) / 200.0f;
	MoveTo* moveto = MoveTo::create(time, posTo);
	auto spaw = Spawn::createWithTwoActions(moveto, ScaleTo::create(time, 0.1f));

	auto seq = Sequence::create(ScaleTo::create(1.0f, 1.5f),
		spaw, 
		CallFuncN::create(CC_CALLBACK_1(MainScene::callBackRemoveSelf, this)), 
		NULL);
	pSprite->runAction(seq);
}
//捕获能量炮的动画
void MainScene::catchEnergyCannonAni(Vec2 posTo)
{
	auto pSprite = Sprite::createWithSpriteFrameName("task_icon_39.png");
	pSprite->setPosition(Vec2(size.width / 2.0f, size.height / 2.0f));
	this->addChild(pSprite, ZORDER_COIN);
	pSprite->setScale(SCALE);

	float time = sqrtf((posTo.x - size.width / 2.0f)*(posTo.x - size.width / 2.0f)+(posTo.y - size.height / 2.0f)*(posTo.y - size.height / 2.0f)) / 200.0f;
	MoveTo* moveto = MoveTo::create(time, posTo);
	auto spaw = Spawn::createWithTwoActions(moveto, ScaleTo::create(time, 0.1f));

	auto seq = Sequence::create(ScaleTo::create(1.0f, 1.5f),
		spaw, 
		CallFuncN::create(CC_CALLBACK_1(MainScene::callBackRemoveSelf, this)), 
		NULL);
	pSprite->runAction(seq);
}
//捕获宝箱的动画
void MainScene::catchBoxAni(Vec2 posTo, bool bSelf)
{
	Manager::getInstance()->setTaskOver(true);
	auto pSprite = Sprite::create("caijinbaohe_1.png");
	pSprite->setPosition(Vec2(size.width / 2.0f, size.height / 2.0f));
	this->addChild(pSprite, ZORDER_COIN);
	pSprite->setScale(SCALE);

	float time = sqrtf((posTo.x - size.width / 2.0f)*(posTo.x - size.width / 2.0f)+(posTo.y - size.height / 2.0f)*(posTo.y - size.height / 2.0f)) / 200.0f;
	MoveTo* pMoveTo = MoveTo::create(time, posTo);
	auto pSpawn = Spawn::createWithTwoActions(pMoveTo, ScaleTo::create(time, 0.1f));
	Sequence* seq = NULL;
	if(bSelf)
	{
		seq = Sequence::create(ScaleTo::create(1.0f, 1.5f),
			pSpawn, 
			CallFuncN::create(CC_CALLBACK_1(MainScene::callBackRemoveSelf, this)), 
			CallFuncN::create(CC_CALLBACK_1(MainScene::callBackGetBoxLayer, this)), 
			NULL);
	}
	else
	{
		seq = Sequence::create(ScaleTo::create(1.0f, 1.5f),
			pSpawn, 
			CallFuncN::create(CC_CALLBACK_1(MainScene::callBackRemoveSelf, this)), 
			NULL);
	}
	pSprite->runAction(seq);
}
//捕获钻石和技能的动画
void MainScene::catchDiamondAndSkillAni(Vec2 posTo, Vec2 posInit, int nType, int nNum, bool self)
{
	Animation *panimation = NULL;
	Sprite* pSprite = NULL;
	if(instance->getRoomNum() == 2 && nType == SKILL_3)
	{
		nType = DIAMOND;
	}
	for (int i = 0; i < nNum; i++)
	{
		std::string strPath = "task_icon_39.png";
		if(nType == SKILL_1)
			strPath = "ui/public/skill_1.png";
		else if(nType == SKILL_2)
			strPath = "ui/public/skill_2.png";
		else if(nType == SKILL_3)
			strPath = "ui/public/skill_3.png";
		else if(nType == SKILL_4)
			strPath = "ui/public/skill_4.png";
		else if(nType == DIAMOND)
		{
			strPath = "ui/public/icon_gem.png";
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("animation/gem.plist");
			//获取动画帧数和缓存中的标记
			Vector<SpriteFrame*> array1(4);

			for (int i=1; i <= 4; i++)
			{
				std::string format = "gem_a%d.png";
				__String* strName = __String::createWithFormat(format.c_str(), i);
				SpriteFrame* pframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(strName->getCString());
				array1.pushBack(pframe);
			}
			panimation = Animation::createWithSpriteFrames(array1, 0.1f);
		}
		if(nType == DIAMOND)
			pSprite = Sprite::createWithSpriteFrameName("gem_a1.png");
		else
			pSprite = Sprite::create(strPath);
		pSprite->setPosition(getRandomPositionByOffset(posInit, 100, 100));
		this->addChild(pSprite, ZORDER_COIN);
		pSprite->setScale(SCALE);

		float time = sqrtf((posTo.x - size.width / 2.0f)*(posTo.x - size.width / 2.0f)+(posTo.y - size.height / 2.0f)*(posTo.y - size.height / 2.0f)) / 200.0f;
		MoveTo* moveto = MoveTo::create(time, posTo);
		auto spaw = Spawn::createWithTwoActions(moveto, ScaleTo::create(time, 0.1f));

		auto seq = Sequence::create(ScaleTo::create(1.0f, 1.2f),
			spaw, 
			CallFuncN::create(CC_CALLBACK_1(MainScene::callBackRemoveSelf, this)), 
			NULL);
		pSprite->runAction(seq);

		if(nType == DIAMOND)
			pSprite->runAction(RepeatForever::create(Animate::create(panimation)));
	}

}
//显示捕捉到宝箱的动画
void MainScene::callBackGetBoxLayer(Node* pNode)
{
	m_pPublicLayer->showGetBoxLayer(); 
}

/*****************多人场任务**************/
//随机任务鱼ID
void MainScene::randFishID()
{
	int nID1 = 0;
	int nID2 = 0;
	int nID3 = 0;
	nID1 = rand()%2 + 28;
	nID2 = rand()%3 + 30;
	nID3 = rand()%8 + 33;
	Manager::getInstance()->setTaskFishID1(nID1);
	Manager::getInstance()->setTaskFishID2(nID2);
	Manager::getInstance()->setTaskFishID3(nID3);
}
//获取完成个数
int MainScene::getFinishNum()
{
	int nNum1 = manualCannon->getTaskFishNum1();
	int nNum2 = manualCannon->getTaskFishNum2();
	int nNum3 = manualCannon->getTaskFishNum3();

	return nNum1 + nNum2 + nNum3;
}

#pragma mark 道具释放
/*****************道具释放****************/
//定屏
void MainScene::useToolStopFish(Ref* pObj)
{
	//log("useToolStopFish");
	if(m_bPaused)return;

	if(!checkSkillNum(0)) return;

#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng::MobClickCpp::event(getStringFromInt(YM_USE_SKILL_1).c_str());
#endif
	MenuItemImage* pItem = (MenuItemImage*)pObj;

	/*int nCurGold = UserData::getInstance()->getGoldNum();
	if(nCurGold < 200)
	{
		m_pPublicLayer->showCoinNotEnoughLayer();
		return;
	}
	else
	{
		UserData::getInstance()->setGoldNum(nCurGold - 200);
		Manager::getInstance()->setIsCoinChange(true);
	}*/
    
	addCDBar(pItem, 1, SkillTime1);
    
	for (int i = 0; i < instance->m_fishVect.size(); i++)
	{
		FishBase* pFish = (FishBase*)instance->m_fishVect.at(i);
		//pFish->stopActionByTag(kTagFishMoveAction);
		if(!pFish->getIsCatch() && pFish->fb_Kind != BOX_FISH)
		{
			pFish->pauseSchedulerAndActions();
		}
	}
	auto seq = Sequence::create(DelayTime::create(SkillTime1),
                                CallFuncN::create(CC_CALLBACK_1(MainScene::callBackResumeAction, this)),
                                nullptr);
	this->runAction(seq);
     
	m_bPaused = true;
    instance->setIsStopBoom(true);
	m_nEffectIndex = 0;
	this->schedule(schedule_selector(MainScene::updateStopEffect), 1/60.0);
}
//极速
void MainScene::useToolSpeedUp(Ref* pObj)
{
	//log("useToolSpeedUp");
	if(manualCannon->getFrameNum() == 2)return;

	if(!checkSkillNum(1)) return;
    
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng::MobClickCpp::event(getStringFromInt(YM_USE_SKILL_2).c_str());
#endif
	/*int nCurGold = UserData::getInstance()->getGoldNum();
	if(nCurGold < 200)
	{
		m_pPublicLayer->showCoinNotEnoughLayer();
		return;
	}
	else
	{
		UserData::getInstance()->setGoldNum(nCurGold - 200);
		Manager::getInstance()->setIsCoinChange(true);
	}*/

	manualCannon->speedUp();
    
	MenuItemImage* pItem = (MenuItemImage*)pObj;
    
	addCDBar(pItem, 1, SkillTime2);
}
//狂暴
void MainScene::useToolRage(Ref* pObj)
{
	//log("useToolRage");
	if(manualCannon->getRage())
	{
		return;
	}
	
	if(!checkSkillNum(2)) return;

#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng::MobClickCpp::event(getStringFromInt(YM_USE_SKILL_3).c_str());
#endif
	/*int nCurGold = UserData::getInstance()->getGoldNum();
	if(nCurGold < 200)
	{
		m_pPublicLayer->showCoinNotEnoughLayer();
		return;
	}
	else
	{
		UserData::getInstance()->setGoldNum(nCurGold - 200);
		Manager::getInstance()->setIsCoinChange(true);
	}*/
    
	manualCannon->rage();
    
	MenuItemImage* pItem = (MenuItemImage*)pObj;
    
	addCDBar(pItem, 1, SkillTime3);
}
//锁定
void MainScene::useToolLockFish(Ref* pObj)
{
	//log("useToolLockFish");
	if(m_bCDTime)
	{
		return;
	}

	if(!checkSkillNum(3)) return;
	
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng::MobClickCpp::event(getStringFromInt(YM_USE_SKILL_4).c_str());
#endif
	/*int nCurGold = UserData::getInstance()->getGoldNum();
	if(nCurGold < 200)
	{
		m_pPublicLayer->showCoinNotEnoughLayer();
		return;
	}
	else
	{
		UserData::getInstance()->setGoldNum(nCurGold - 200);
		Manager::getInstance()->setIsCoinChange(true);
	}*/

	m_bCDTime = true;
    
	auto seq = Sequence::create(DelayTime::create(SkillTime4),
                                CallFuncN::create(CC_CALLBACK_1(MainScene::callBackRemoveLock, this)),
                                nullptr);
	this->runAction(seq);
    
	MenuItemImage* pItem = (MenuItemImage*)pObj;
    
	addCDBar(pItem, 1, SkillTime4);
}
//万炮
void MainScene::useToolTenThousand(Ref* pObj)
{
	if(manualCannon->bulletPower == 10000)
	{
		return;
	}

	if(!checkSkillNum(4)) return;

	//金币不足100W不能释放次技能
	int nCurGold = UserData::getInstance()->getGoldNum();
	/*if(nCurGold < 200)
	{
	m_pPublicLayer->showCoinNotEnoughLayer();
	return;
	}
	else
	{
	if(nCurGold < 1000000)
	{
	m_pPublicLayer->showPublicSureLayer("skill_tip");
	return;
	}
	else
	{
	UserData::getInstance()->setGoldNum(nCurGold - 200);
	Manager::getInstance()->setIsCoinChange(true);
	}
	}*/
	if(nCurGold < 1000000)
	{
		m_pPublicLayer->showPublicSureLayer("skill_tip");
		return;
	}

#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng::MobClickCpp::event(getStringFromInt(YM_USE_SKILL_5).c_str());
#endif

	m_nOldPower = manualCannon->bulletPower;
	manualCannon->bulletPower = 10000;
	//manualCannon->spell_power->setString("10000");
	manualCannon->changePower();
	auto seq = Sequence::create(DelayTime::create(SkillTime5),
                                CallFuncN::create(CC_CALLBACK_1(MainScene::callBackRemoveTenThousand, this)),
                                nullptr);
	this->runAction(seq);
    
	MenuItemImage* pItem = (MenuItemImage*)pObj;
    
	addCDBar(pItem, 1, SkillTime5);
}
//机器人释放道具
void MainScene::useToolStopFish1()
{
	m_bPaused1 = true;
	instance->setIsStopBoom(true);
	m_nEffectIndex1 = 0;
	for (int i = 0; i < instance->m_fishVect.size(); i++)
	{
		FishBase* pFish = (FishBase*)instance->m_fishVect.at(i);
		if(!pFish->getIsCatch() && pFish->fb_Kind != BOX_FISH)
		{
			pFish->pauseSchedulerAndActions();
		}
	}
	auto seq = Sequence::create(DelayTime::create(SkillTime1),
		CallFuncN::create(CC_CALLBACK_1(MainScene::callBackResumeAction1, this)),
		nullptr);
	this->runAction(seq);

	this->schedule(schedule_selector(MainScene::updateStopEffect1), 1/60.0);
}
void MainScene::useToolLockFish1()
{

}//回复运动
void MainScene::stopShooting()
{
	manualCannon->isShoot = false;

}
//停止鱼潮
void MainScene::stopFishBoom()
{
	isBoom = false;
}
//显示礼包
void MainScene::showGiftLayer()
{
	if(!m_pPublicLayer->getIsShowGiftLayer())
	{
		m_pPublicLayer->showGiftLayer();
	}
}
//显示破产礼包
void MainScene::showUnenoughLayer()
{
	if(!m_pPublicLayer->getIsShowFreeGoldLayer())
	{
		m_pPublicLayer->showUnenoughGiftLayer();
	}
}
//显示商店
void MainScene::shopLayer()
{
	if(!m_pPublicLayer->getIsShowShopLayer())
	{
		m_pPublicLayer->showShopLayer();
	}
}
//刷新游戏金币和钻石数量
void MainScene::refreshGoldAndDiamonds()
{
	Manager::getInstance()->setIsCoinChange(true);
	manualCannon->refreshUserdata();
}
//随机任务鱼的ID
void MainScene::resetManyTask()
{
	manualCannon->setTaskFishNum1(0);
	manualCannon->setTaskFishNum2(0);
	manualCannon->setTaskFishNum3(0);
	Fire* fire1 = dynamic_cast<Fire*>(this->getChildByTag(FIRE1_tag));
	Fire* fire2 = dynamic_cast<Fire*>(this->getChildByTag(FIRE2_tag));
	Fire* fire3 = dynamic_cast<Fire*>(this->getChildByTag(FIRE3_tag));
	if(fire1)fire1->resetTaskData();
	if(fire2)fire2->resetTaskData();
	if(fire3)fire3->resetTaskData();

	this->randFishID();
}
//获取任务进度
int MainScene::getCurrentFinishNum(int id)
{
	if(id == 0)
		return manualCannon->getTaskFishNum1();
	else if(id == 1)
		return manualCannon->getTaskFishNum2();
	else if(id == 2)
		return manualCannon->getTaskFishNum3();

	return 0;
}


void MainScene::callBackResumeAction(Node* pNode)
{
	if(!m_bPaused1)
	{
		for (int i = 0; i < instance->m_fishVect.size(); i++)
		{
			FishBase* pFish = (FishBase*)instance->m_fishVect.at(i);
			pFish->resumeSchedulerAndActions();
		}
	}

	this->unschedule(schedule_selector(MainScene::updateStopEffect));
	this->removeChildByTag(102);
	m_bPaused = false;
	instance->setIsStopBoom(false);
	//addCDBar(pNode, 0);
}
void MainScene::callBackResumeAction1(Node* pNode)
{
	if(!m_bPaused)
	{
		for (int i = 0; i < instance->m_fishVect.size(); i++)
		{
			FishBase* pFish = (FishBase*)instance->m_fishVect.at(i);
			pFish->resumeSchedulerAndActions();
		}
	}

	this->unschedule(schedule_selector(MainScene::updateStopEffect1));
	instance->setIsStopBoom(false);
	m_bPaused1 = false;
}//回复CD
void MainScene::callBackResumeCD1(Node* pNode)
{
	m_bPaused = false;
}
//移除自己
void MainScene::callBackRemoveSelf(Node* pNode)
{
	pNode->removeFromParent();
}
//显示定住特效
void MainScene::updateStopEffect(float dt)
{
	m_nEffectIndex++;
    
	//大的定子
	if(m_nEffectIndex == 1)
	{
		auto pStopSp = Sprite::create("ui/skill/fix_bomb.png");
		pStopSp->setPosition(Vec2(size.width / 2, size.height / 2));
		pStopSp->setScale(0.45);
		this->addChild(pStopSp, ZORDER_STOP);
        
		auto seq = Sequence::create(ScaleTo::create(0.4, 2, 2),
                                    ScaleTo::create(0.1, 1.5, 1.5),
                                    ScaleTo::create(0.2, 1.8, 1.8),
                                    ScaleTo::create(0.1, 1.2, 1.2),
                                    FadeOut::create(1.5f),
                                    CallFuncN::create(CC_CALLBACK_1(MainScene::callBackRemoveSelf, this)),
                                    nullptr);
		pStopSp->runAction(seq);
	}
    
	if(m_nEffectIndex > 32 && m_nEffectIndex < 96)
	{
		int nRound = (m_nEffectIndex - 32) % 64;
        
		int nRow = nRound / 8;
		int nCol = nRound % 8;
		float fRadius = nRow * 50.0f + 100;
        
		float tx = cos(CC_DEGREES_TO_RADIANS(nCol*45 + /*CCRANDOM_0_1()**/45)) * fRadius;
		float ty = sin(CC_DEGREES_TO_RADIANS(nCol*45 + /*CCRANDOM_0_1()**/45)) * fRadius;
        
		Vec2 pos = Vec2(size.width/2 + tx, size.height / 2 + ty);
        
		auto sp = Sprite::create("ui/skill/batch_fix_frame.png");
		sp->setScale(0.4f);
		float fRot = calcAngleFromOriginToTarget(Vec2(size.width/2, size.height / 2), pos);
		sp->setRotation(-fRot - 90);
		sp->setPosition(pos);
		this->addChild(sp, ZORDER_STOP - 1);
        
		auto seq = Sequence::create(
                                    DelayTime::create(2.5),
                                    CallFuncN::create(CC_CALLBACK_1(MainScene::callBackRemoveSelf, this)),
                                    nullptr);
		sp->runAction(seq);
	}
    
}
void MainScene::updateStopEffect1(float dt)
{
	m_nEffectIndex1++;

	//大的定子
	if(m_nEffectIndex1 == 1)
	{
		auto pStopSp = Sprite::create("ui/skill/fix_bomb.png");
		pStopSp->setPosition(Vec2(size.width / 2, size.height / 2));
		pStopSp->setScale(0.6);
		this->addChild(pStopSp, ZORDER_STOP);

		auto seq = Sequence::create(ScaleTo::create(0.4, 2, 2),
			ScaleTo::create(0.1, 1.5, 1.5),
			ScaleTo::create(0.2, 1.8, 1.8),
			ScaleTo::create(0.1, 1.2, 1.2),
			FadeOut::create(1.5f),
			CallFuncN::create(CC_CALLBACK_1(MainScene::callBackRemoveSelf, this)),
			nullptr);
		pStopSp->runAction(seq);
	}

	if(m_nEffectIndex1 > 32 && m_nEffectIndex1 < 96)
	{
		int nRound = (m_nEffectIndex1 - 32) % 64;

		int nRow = nRound / 8;
		int nCol = nRound % 8;
		float fRadius = nRow * 50.0f + 100;

		float tx = cos(CC_DEGREES_TO_RADIANS(nCol*45 + 45)) * fRadius;
		float ty = sin(CC_DEGREES_TO_RADIANS(nCol*45 + 45)) * fRadius;

		Vec2 pos = Vec2(size.width/2 + tx, size.height / 2 + ty);

		auto sp = Sprite::create("ui/skill/batch_fix_frame.png");
		sp->setScale(0.4f);
		float fRot = calcAngleFromOriginToTarget(Vec2(size.width/2, size.height / 2), pos);
		sp->setRotation(-fRot - 90);
		sp->setPosition(pos);
		this->addChild(sp, ZORDER_STOP - 1);

		auto seq = Sequence::create(
			DelayTime::create(2.5),
			CallFuncN::create(CC_CALLBACK_1(MainScene::callBackRemoveSelf, this)),
			nullptr);
		sp->runAction(seq);
	}

}//移除锁定状态
void MainScene::callBackRemoveLock(Node* pNode)
{
	m_bCDTime = false;
	manualCannon->isShoot = false;
	for (int i = 0; i < instance->m_fishVect.size(); i++)
	{
		FishBase* pFish = (FishBase*)instance->m_fishVect.at(i);
		pFish->removeChildByTag(103);
		pFish->setLocked(false);
		m_nCurLockUID = 0;
	}
}
//移除万炮状态
void MainScene::callBackRemoveTenThousand(Node* pNode)
{
	manualCannon->bulletPower = m_nOldPower;
	//manualCannon->power_label->setString(getStringFromInt(m_nOldPower));
    manualCannon->changePower();
}
//处理锁定事件
void MainScene::handleLockEvent(Vec2 pos)
{
	if(!m_bCDTime)
	{
		return;
	}
	unsigned int nID = 0;
	//点击选中要瞄准的鱼
	for (int i = 0; i < instance->m_fishVect.size(); i++)
	{
		//FishBase* pFish = (FishBase*)instance->m_fishVect.at(i);
        auto pFish = dynamic_cast<FishBase*>(instance->m_fishVect.at(i));
		//Rect r = pFish->boundingBox();
        if (pFish->fb_Kind < 41 && pFish->fb_Kind > 10 && !pFish->getIsCatch()) 
		{
			if(!pFish->getBasicSprite1())
			{
				continue;
			}
            log("fb_Kind = %d, %d", pFish->fb_Kind, pFish->getLocked());
            Point p_sp = pFish->convertToWorldSpace(pFish->getBasicSprite1()->getPosition());
            log("-----handleLockEvent---1---");
            Rect r1 = pFish->getBasicSprite1()->boundingBox();
			log("-----handleLockEvent---2---");
			Size fishSize = r1.size;
			Rect r = Rect(p_sp.x - fishSize.width / 2, p_sp.y - fishSize.height/2, fishSize.width, fishSize.height);
            //log("%f,%f, %f, %f", r.origin.x, r.origin.y, r.size.width, r.size.height);
            if(r.containsPoint(pos) && pFish->getUID() != m_nCurLockUID)
            {
				log("-----handleLockEvent---3---");
                Sprite* pLock = Sprite::create("ui/skill/lock_cross.png");
				log("-----handleLockEvent---4---");
                pLock->setScale(0.75);
				log("-----handleLockEvent---5---");
                pLock->setPosition(Vec2(pFish->getContentSize().width / 2, pFish->getContentSize().height / 4 * 3));
                //pLock->setScale(0.5f);
                pFish->addChild(pLock, 10, 103);
				log("-----handleLockEvent---6---");
                m_bLocked = true;
                pFish->setLocked(true);
                nID = pFish->getUID();
				log("-----handleLockEvent---7---");
				m_nCurLockUID = pFish->getUID();
				manualCannon->isShoot = true;
				Manager::getInstance()->setCurLockUID(m_nCurLockUID);
				log("-----handleLockEvent---8---");
				break;
            }
        }
        
	}
    
	//清除选中的鱼
	if(nID > 0)
	{
		for (int i = 0; i < instance->m_fishVect.size(); i++)
		{
			FishBase* pFish = (FishBase*)instance->m_fishVect.at(i);
			if(nID != pFish->getUID())
			{
				pFish->removeChildByTag(103);
				pFish->setLocked(false);
			}
		}
	}
    
}
//获取时候有被锁定的鱼
Vec2 MainScene::getLockedFishPosition()
{
	if(m_bCDTime)
	{
		for (int i = 0; i < instance->m_fishVect.size(); i++)
		{
			FishBase* pFish = (FishBase*)instance->m_fishVect.at(i);
			if(pFish->getUID() == m_nCurLockUID && !pFish->getIsCatch() && pFish->getChildByTag(103))
			{
				return pFish->getPosition();
			}
		}
	}
    
	return Vec2(0, 0);
}
//添加刷新CD显示
void MainScene::addCDBar(Node* pNode, int nType, int nTime)
{
	Sprite* pTimeSP = Sprite::create("ui/skill/skill_mask.png");
	ProgressTimer* pTime = ProgressTimer::create(pTimeSP);
	if(nType == 0)
	{
		pTime->setType(ProgressTimer::Type::BAR);
		pTime->setAnchorPoint(Vec2(0.5, 0.5));
		pTime->setMidpoint(Vec2(0.5, 0));
		pTime->setBarChangeRate(Vec2(0, 1));
		pTime->setPercentage(100);
		auto seq = Sequence::create(ProgressTo::create(nTime, 0),
                                    CallFuncN::create(CC_CALLBACK_1(MainScene::callBackRemoveSelf, this)),
                                    nullptr);
		pTime->runAction(seq);
	}
	else
	{
		pTime->setType(ProgressTimer::Type::RADIAL);
		pTime->setAnchorPoint(Vec2(0.5, 0.5));
		pTime->setMidpoint(Vec2(0.5, 0.5));
		pTime->setPercentage(100);
		auto seq = Sequence::create(ProgressTo::create(nTime, 0),
                                    CallFuncN::create(CC_CALLBACK_1(MainScene::callBackRemoveSelf, this)),
                                    nullptr);
		pTime->runAction(seq);
	}
	pTime->setRotationY(180);
	pTime->setPosition(Vec2(pNode->getContentSize().width / 2, pNode->getContentSize().height / 2));
	pNode->addChild(pTime);
    
	//添加边条
	Sprite* pBar1 = Sprite::create("ui/skill/skill_light_line.png");
	pBar1->setAnchorPoint(Vec2(0.25f, 0.5f));
	pBar1->setPosition(pTime->getPosition());
	pBar1->setRotation(-90);
	pNode->addChild(pBar1);
	auto seq1 = Sequence::create(DelayTime::create(nTime),
                                 CallFuncN::create(CC_CALLBACK_1(MainScene::callBackRemoveSelf, this)), 
                                 nullptr);
	pBar1->runAction(seq1);
    
	Sprite* pBar2 = Sprite::create("ui/skill/skill_light_line.png");
	pBar2->setRotation(-90);
	pBar2->setAnchorPoint(Vec2(0.25f, 0.5f));
	pBar2->setPosition(pTime->getPosition());
	pNode->addChild(pBar2);
	auto seq2 = Sequence::create(RotateBy::create(nTime, 360),
                                 CallFuncN::create(CC_CALLBACK_1(MainScene::callBackRemoveSelf, this)), 
                                 nullptr);
	pBar2->runAction(seq2);
}
void MainScene::chageManualCannon(int kind, int num)
{
    Point OldCannonPos = manualCannon->getPosition();
	int nPosID = manualCannon->getM_chosePositionNum();
    manualCannon->removeFromParent();
    manualCannon = ManualCannon::createCannon();
    manualCannon->initConnon(instance->getM_bulletPower(), kind, num, m_bCDTime);
    this->addChild(manualCannon, MANUALCANNON_zOrder);
    manualCannon->manualNum = 1;
    manualCannon->setPosition(OldCannonPos);
    if (OldCannonPos.y > 200) 
	{
        manualCannon->setRotation(180);
        manualCannon->spell_score->setRotation(180);
        manualCannon->spell_power->setRotation(180);
    }
	manualCannon->setM_chosePositionNum(nPosID);
	manualCannon->changeCannonPos();
	manualCannon->setDelegateGame(this);
}
//更新技能条的位置（多人场）
void MainScene::refreshSkillMenuPosition()
{
	m_pPublicLayer->refreshSkillMneuPosition(manualCannon->getM_chosePositionNum());
}
//更新机器人的技能释放
void MainScene::updateUseSkill(float dt)
{
	if(m_nSkillIndex > 30*CCRANDOM_0_1() + 60)
	{
		Fire* fire1 = dynamic_cast<Fire*>(this->getChildByTag(FIRE1_tag));
		Fire* fire2 = dynamic_cast<Fire*>(this->getChildByTag(FIRE2_tag));
		Fire* fire3 = dynamic_cast<Fire*>(this->getChildByTag(FIRE3_tag));
		if(fire1 || fire2 || fire3 && !m_bPaused)
		{
			this->useToolStopFish1();
			m_nSkillIndex = 0;
		}
	}
	m_nSkillIndex++;

	if(m_nSpeedIndex > 10*CCRANDOM_0_1() + 30)
	{
        Fire* fire1 = dynamic_cast<Fire*>(this->getChildByTag(FIRE1_tag));
        Fire* fire2 = dynamic_cast<Fire*>(this->getChildByTag(FIRE2_tag));
        Fire* fire3 = dynamic_cast<Fire*>(this->getChildByTag(FIRE3_tag));
		int nID = rand() % 3;
		if(fire1 && nID == 0)
		{
			fire1->speedUp();
			m_nSpeedIndex = 0;
		}
		else if(fire2 && nID == 1)
		{
			fire2->speedUp();
			m_nSpeedIndex = 0;
		}
		else if(fire3 && nID == 2)
		{
			fire3->speedUp();
			m_nSpeedIndex = 0;
		}
	}
	m_nSpeedIndex++;
}
//检测道具数量
bool MainScene::checkSkillNum(int id)
{
	int nNeed = 10;
	int nNum = UserData::getInstance()->getSkillNum1();
	if(id == 0)
	{
		nNum = UserData::getInstance()->getSkillNum1();
		nNeed = 10;
	}
	else if(id == 1)
	{
		nNum = UserData::getInstance()->getSkillNum2();
		nNeed = 10;
	}
	else if(id == 2)
	{
		nNum = UserData::getInstance()->getSkillNum3();
		nNeed = 10;
	}
	else if(id == 3)
	{
		nNum = UserData::getInstance()->getSkillNum4();
		nNeed = 10;
	}
	else if(id == 4)
	{
		nNum = UserData::getInstance()->getSkillNum5();
		nNeed = 10;
	}

	if(nNum > 0)
	{
		UserData::getInstance()->decreaseSkillNum(id, 1);
	}
	else if(UserData::getInstance()->getDiamonds() >= nNeed)
	{
		UserData::getInstance()->decreaseDiamonds(nNeed);
	}
	else
	{
		m_pPublicLayer->showDiamondShopLayer();
		return false;
	}

	m_pPublicLayer->refreshSkillNum();
	manualCannon->refreshUserdata();

	return true;

}

#pragma mark 触摸事件
void MainScene::onEnter()
{
    Layer::onEnter();
    //物理世界碰撞检测 监听
    auto p_listener = EventListenerPhysicsContact::create();
    p_listener->onContactBegin = CC_CALLBACK_1(MainScene::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(p_listener, this);
    
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	m_pEventListener = EventListenerTouchOneByOne::create();
	m_pEventListener->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan,this);
	m_pEventListener->onTouchMoved = CC_CALLBACK_2(MainScene::onTouchMoved,this);
	m_pEventListener->onTouchEnded = CC_CALLBACK_2(MainScene::onTouchEnded,this);
	m_pEventListener->setSwallowTouches(true);//不向下传递触摸
	//dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    dispatcher->addEventListenerWithFixedPriority(m_pEventListener, 10);
    log("--------------------MainScene::onEnter()---------------------------------");
    
}
void MainScene::onExit()
{
    
    UserData::getInstance()->save();
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

bool MainScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    isActivation = true;
    
    this->setManualRotation(touch->getLocation());
    manualCannon->isShoot = true;
    manualCannon->rcordTime = 0;
    
    Vec2 pos = this->getLockedFishPosition();
	if(pos.equals(Vec2::ZERO))
	{
		manualCannon->setClickPoint(touch->getLocation());//点击位置传值
	}
    this->handleLockEvent(touch->getLocation());
    return true;
}
void MainScene::onTouchMoved(Touch *touch, Event *unused_event)
{
    isActivation = true;
    this->setManualRotation(touch->getLocation());
    manualCannon->isShoot = true;
    
	Vec2 pos = this->getLockedFishPosition();
	if(pos.equals(Vec2::ZERO))
	{
		manualCannon->setClickPoint(touch->getLocation());//点击位置传值
	}
}
void MainScene::onTouchEnded(Touch *touch, Event *unused_event)
{
    
    this->setManualRotation(touch->getLocation());
    manualCannon->isShoot = false;
    
}
void MainScene::onTouchCancelled(Touch *touch, Event *unused_event)
{
    
}

//按键响应
void MainScene::onKeyReleased(EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
	switch (keycode)
	{
	case EventKeyboard::KeyCode::KEY_ESCAPE:
		m_pPublicLayer->showSelectLayer();
		break;
	case EventKeyboard::KeyCode::KEY_MENU:
		break;
	default:
		break;
	}
}

//控制大炮的角度
void MainScene::setManualRotation(Point clickPoint)
{

//    Point dis = clickPoint - manualCannon->battery->convertToWorldSpace(manualCannon->cannon->getPosition());
//    float m_angle = dis.getAngle();
//    float m_rotation = CC_RADIANS_TO_DEGREES(-1*m_angle) + 90;
//    manualCannon->cannon->setRotation(m_rotation);
    
    Point cannonPoint = manualCannon->battery->convertToWorldSpace(manualCannon->cannon->getPosition());
    float fAngle = calcAngleFromOriginToTarget(cannonPoint, clickPoint);
    if(manualCannon->getPositionY() > 200)
        manualCannon->cannon->setRotation(-fAngle + 90 + 180);
    else
        manualCannon->cannon->setRotation(-fAngle + 90);
    
    
    
    
}
void MainScene::update(float dt)
{
    Vec2 pos = this->getLockedFishPosition();
    if (pos.x != 0&&pos.y != 0) 
	{
        manualCannon->setClickPoint(pos);
		this->setManualRotation(pos);
        manualCannon->isShoot = true;
		//log("setClickPoint   pos  x == (%f), y== (%f)", pos.x, pos.y);
    }
	if(m_bCDTime)
	{
		for (int i = 0; i < instance->m_fishVect.size(); i++)
		{
			FishBase* pFish = (FishBase*)instance->m_fishVect.at(i);
			if(pFish->getUID() == m_nCurLockUID)
			{
				Vec2 pos1 = pFish->getPosition();
				if(pos1.x < 0|| pos1.x > size.width || pos1.y < 0 || pos1.y > size.height)
				{
					pFish->setLocked(false);
					pFish->removeChildByTag(103);
					manualCannon->isShoot = false;
				}
			}
		}
	}

	//多人场第一个房间，金币高于30000就被踢出
	/*if(Manager::getInstance()->getRoomNum() == 2)
	{
	if(Manager::getInstance()->getTwo_LevelNum() == 1)
	{
	int nTag = FIRE1_tag;
	for (int i = 0; i < 3; i++)
	{
	nTag = FIRE1_tag + i;
	auto pFire = dynamic_cast<Fire*>(this->getChildByTag(nTag));
	if(pFire && pFire->totalCorn > 30000)
	{
	menuPosVect.at(pFire->getf_chosePositionNum())->setIsChosed(false);
	menuPosVect.at(pFire->getf_chosePositionNum())->setVisible(true);
	pFire->removeFromParentAndCleanup(true);
	}
	}
	}

	}*/
}
MainScene::MainScene():m_recordAddFishRime(0), isActivation(false), recordSleepTime(0), isCountdownApper(false), isBlastingMachine(false), isBoom(false)
{}
MainScene::~MainScene()
{
    plistDic->release();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(m_pEventListener);

	Manager::getInstance()->cleanBuffer();
}
// 帧缓存  对于batchNode图片只需缓存plist文件
void MainScene::loadCache()
{
    //缓存 鱼
    SpriteFrameCache* cache = CCSpriteFrameCache::getInstance();
    //缓存 炮台
    cache->addSpriteFramesWithFile("cannon_atlas.plist", "cannon_atlas.png");
    
    
    cache->addSpriteFramesWithFile("fish_frame_224.plist", "fish_frame_224.png");
    cache->addSpriteFramesWithFile("fish_frame_54.plist", "fish_frame_54.png");
    cache->addSpriteFramesWithFile("fish_frame_23.plist", "fish_frame_23.png");
    cache->addSpriteFramesWithFile("fish_frame_26.plist", "fish_frame_26.png");
    cache->addSpriteFramesWithFile("fish_frame_241.plist", "fish_frame_241.png");
    cache->addSpriteFramesWithFile("fish_frame_252.plist", "fish_frame_252.png");
    cache->addSpriteFramesWithFile("fish_frame_49.plist", "fish_frame_49.png");
    cache->addSpriteFramesWithFile("fish_frame_244.plist", "fish_frame_244.png");
    cache->addSpriteFramesWithFile("fish_frame_262.plist", "fish_frame_262.png");
    cache->addSpriteFramesWithFile("fish_frame_28.plist", "fish_frame_28.png");
    cache->addSpriteFramesWithFile("fish_frame_29.plist", "fish_frame_29.png");
    cache->addSpriteFramesWithFile("fish_frame_97.plist", "fish_frame_97.png");
    cache->addSpriteFramesWithFile("fish_frame_31.plist", "fish_frame_31.png");
    
    cache->addSpriteFramesWithFile("fish_frame_1.plist", "fish_frame_1.png");
    cache->addSpriteFramesWithFile("fish_frame_2.plist", "fish_frame_2.png");
    cache->addSpriteFramesWithFile("fish_frame_3.plist", "fish_frame_3.png");
    cache->addSpriteFramesWithFile("fish_frame_4.plist", "fish_frame_4.png");
    cache->addSpriteFramesWithFile("fish_frame_18.plist", "fish_frame_18.png");
    cache->addSpriteFramesWithFile("fish_frame_31.plist", "fish_frame_31.png");
    cache->addSpriteFramesWithFile("fish_frame_166.plist", "fish_frame_166.png");
    cache->addSpriteFramesWithFile("fish_frame_6.plist", "fish_frame_6.png");
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
