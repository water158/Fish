//
//  FishBase.cpp
//  Helloworld
//
//  Created by etgame on 15-1-27.
//
//

#include "FishBase.h"
#include "Data.h"
#include "ManualCannon.h"
#include "Manager.h"
#include "PixCell.h"
#include "Coin.h"
#include "Manager.h"
#include "MusicData.h"
#include "UserData.h"
using namespace std;


FishBase::FishBase():isCatch(false), arc_die(0), isGray(true), isSpecial(false), bindToCannonNum(1), choseOne(1)
{
	this->setUID(UserData::getInstance()->getUID());
	m_pDelegateGame = NULL;
	moveSpeed = NULL;
	animSpeed = NULL;
	greySpriteSpeed = NULL;
	fishbody = NULL;
	fb_splineArray = NULL;
	m_nFishTeleploneFare = 0;
	m_nResumeSpeed = 1.0f;
	m_bTVFish = false;
	m_nBulletPower = -1;
	m_bAimedByRobot1 = false;
	m_bAimedByRobot2 = false;
	m_bAimedByRobot3 = false;
	m_bAimedByRobot4 = false;
	value = 0;
	log("FishBase::FishBase()");
}
FishBase::~FishBase()
{
	log("FishBase::~FishBase()");
	
	if(moveSpeed)
	{
		log("------moveSpeed Reference == (%d) ---------------", moveSpeed->getReferenceCount());
		CC_SAFE_RELEASE_NULL(moveSpeed);
	}
	if(fishbody)
	{
		CC_SAFE_RELEASE_NULL(fishbody);
	}
	/*if(animSpeed)
	{
	animSpeed->release();
	}
	if(greySpriteSpeed)
	{
	greySpriteSpeed->release();
	}*/
}
void FishBase::kindOfFish(int number)
{
    value = fishValue[number];
	if(this->fb_Kind == TELEPHONE_FARE_FISH)
	{
		value = 100;
	}
}

void FishBase::hpOfFish(int number)
{
    typeHp = fishHp[number];
	if(this->fb_Kind == TELEPHONE_FARE_FISH)
	{
		typeHp = 100;
	}
}

void FishBase::numOfScore(int number)
{}

void FishBase::initFishBase(int fKind)
{
    instance = Manager::getInstance();
    size = Director::getInstance()->getWinSize();
    fishInRoom = instance->getRoomNum();
    fb_Kind = fKind;
    //随机设定鱼游动的时间，一次控制鱼的帧动画帧率 传值给RedSprite使两个帧动画帧率一致
    if (fb_Kind < 33)
    {
        time = 40;
    }else
        time = rand() % 20 + 50;

	if(fb_Kind == BOX_FISH)
	{
		time = 50;
	}
    //鱼类
    if (fb_Kind < 41 || fb_Kind == SCOPEFISH) 
	{ 
        //初始化精灵
        string f_name = d_fishName[fb_Kind][0];
        string f_full_name = f_name + "01" + ".png";
      
        auto basisSp = Sprite::createWithSpriteFrameName(f_full_name.c_str());
        this->addChild(basisSp, 4, BASICSP);

		//设置鱼的动画
        this->fishAnimation(fb_Kind);
        basisSp->runAction(animSpeed);

        //初始化阴影
        if (isGray)
        {
            graySprite = Sprite::createWithSpriteFrameName(f_full_name.c_str());
            graySprite->setPosition(Vec2(-200, -200));
            graySprite->setColor(Color3B(0, 0, 0));
            graySprite->setOpacity(120);
            this->getParent()->addChild(graySprite, ZORDER_FISH_SHILED);
            graySprite->setScale(SCALE);
			greySpriteSpeed = animSpeed->clone();
			//greySpriteSpeed->retain();
            graySprite->runAction(greySpriteSpeed);
        }
        //设定 每种鱼的血量
        this->hpOfFish(fb_Kind); 
        this->kindOfFish(fb_Kind);
		//鱼的总血量
        hp = fish_basisHp * typeHp;
    }
	//能量炮
    if (fb_Kind == ENERGYCANNON) 
	{ 
        this->initWithSpriteFrameName("task_icon_39.png");
        hp = 10;
        arc_die = 200;
    }
    
	//话费鱼
	if (fb_Kind == TELEPHONE_FARE_FISH) 
	{ 
		this->initWithSpriteFrameName("huafeiyu.png");
		hp = 10;
		arc_die = 100;
		value = 100;
	}

	//宝盒
	if (fb_Kind == BOX_FISH) 
	{ 
		this->initWithFile("caijinbaohe_1.png");
		hp = 10;
		arc_die = 100;
		value = 100;
	}
    
    this->setTag(FISH_TAG);
    
    this->setLocked(false);
	
    //设置刚体
    if (isSetbody) 
	{
       this->setBody(); 
    }
    this->setScale(SCALE);
    
}

void FishBase::fishAnimation(int fishType)
{
    int f_t_num = atoi(d_fishName[fishType][1].c_str());
    string f_name = d_fishName[fishType][0];

    auto animation = Animation::create();
    //animation->retain();
    for (int i = 1; i <= f_t_num; i++) 
	{
        
        __String* f_all_name = nullptr;
        
        if (i < 10) {
            f_all_name = __String::createWithFormat("%s0%d.png",f_name.c_str(), i);
        }else {
            f_all_name = __String::createWithFormat("%s%d.png",f_name.c_str(), i);
        }
     
        //CCLog("f_all_namu = %s", f_all_name->getCString());
        SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(f_all_name->getCString());
        animation->addSpriteFrame(frame);
    }
    //this->getPhysicsBody()->sets
    // 设置动画播放的属性 2秒 6帧
    animation->setDelayPerUnit(0.1);
    // 设置精灵帧的使用方式, 做完动画还原成初始帧
    animation->setRestoreOriginalFrame(false);
    // -1无限重复
    animation->setLoops(-1);
    auto fishAnimate = Animate::create(animation);
    
    //fishAnimate->retain();
    animSpeed = Speed::create(fishAnimate, 1);
	//animSpeed->retain();
    //animation->release();
    //fishAnimate->release();
}
void FishBase::catchAnimation(int fishType)
{
    this->stopAllActions();
    this->unscheduleUpdate();
    if (fb_Kind < 41||fb_Kind == SCOPEFISH) 
	{
        animSpeed->setSpeed(4);
    }
    FadeTo* to = FadeTo::create(2, 0.1f);
   
    CallFunc* func = CallFunc::create(CC_CALLBACK_0(FishBase::removeFish, this));
    Sequence* seq = Sequence::create( to, func, nullptr);
    if(fb_Kind == TELEPHONE_FARE_FISH || fb_Kind == BOX_FISH)
		this->runAction(seq);
	else
		this->getBasicSprite1()->runAction(seq);

	if (this->getUID() == Manager::getInstance()->getCurLockUID()) 
	{
		this->setLocked(false);
		this->removeChildByTag(103);
		if(this->getDelegateGame())
			this->getDelegateGame()->stopShooting();
	}
}

void FishBase::setShader()
{
    if (fb_Kind != ENERGYCANNON && fb_Kind != TELEPHONE_FARE_FISH && fb_Kind != BOX_FISH)
    {
        this->getBasicSprite1()->setColor(Color3B(255, 100, 100));
        DelayTime* time = CCDelayTime::create(0.2f);
        CallFunc* func = CCCallFunc::create(CC_CALLBACK_0(FishBase::removeShader, this));
        Sequence* seq = CCSequence::create(time, func, nullptr);
        this->runAction(seq);
    }
    if (fb_Kind == ENERGYCANNON || fb_Kind == TELEPHONE_FARE_FISH || fb_Kind == BOX_FISH)
    {
        this->setColor(Color3B(255, 100, 100));
        DelayTime* time = CCDelayTime::create(0.2f);
        CallFunc* func = CCCallFunc::create(CC_CALLBACK_0(FishBase::removeShader, this));
        Sequence* seq = CCSequence::create(time, func, nullptr);
        this->runAction(seq);
    }
    
    
}
void FishBase::removeShader()
{
    if (fb_Kind != ENERGYCANNON && fb_Kind != TELEPHONE_FARE_FISH && fb_Kind != BOX_FISH)
    {
        this->getBasicSprite1()->setColor(Color3B(255, 255, 255));
    }
	else if (fb_Kind == ENERGYCANNON || fb_Kind == TELEPHONE_FARE_FISH || fb_Kind == BOX_FISH)
    {
        this->setColor(Color3B(255, 255, 255));
    }
    
}
void FishBase::removeFishBody()
{
    if (isSetbody) 
	{
//        fishbody->removeFromWorld();
//        this->setPhysicsBody(nullptr);
//        log("-----fish->setPhysicsBody(nullptr);-----");
    }
    
    if (this->getUID() == Manager::getInstance()->getCurLockUID()) 
	{
        this->setLocked(false);
		this->removeChildByTag(103);
		if(this->getDelegateGame())
			this->getDelegateGame()->stopShooting();
    }
}
void FishBase::removeFish()
{
	//电视里面的鱼直接移除掉
	/*if(this->getTVFish())
	{
	this->removeFromParentAndCleanup(true);
	return;
	}*/
    /*if (this->getUID() == Manager::getInstance()->getCurLockUID()) 
	{
        this->setLocked(false);
		this->removeChildByTag(103);
		if(this->getDelegateGame())
			this->getDelegateGame()->stopShooting();
    }*/
    if (fb_Kind < 41 || fb_Kind == SCOPEFISH) 
	{
         graySprite->removeFromParent();
    }
    instance->m_fishVect.eraseObject(this);
    //log("fb_Kind = %d", fb_Kind);
	this->stopAllActions();
	this->unscheduleAllSelectors();
    this->removeFromParent();
    
    //log("-----fish->removeFromParent();-----");
    
}
void FishBase::removeAllThings()
{
    this->removeFishBody();
    this->removeFish();
    
}
bool FishBase::randomCatch(int uprate1, int nID)
{
	int nOverPeak = UserData::getInstance()->getOverGoldPeak();
	//机器人的碰撞
	if(nID > 1)
	{
		int nAimID1 = Manager::getInstance()->getTaskFishID1();
		int nAimID2 = Manager::getInstance()->getTaskFishID2();
		int nAimID3 = Manager::getInstance()->getTaskFishID3();
		int nRate = 2;
		float fPercent = 0.8f;
		if(fb_Kind == nAimID1 || fb_Kind == nAimID2 || fb_Kind == nAimID3)
			fPercent = 0.5f;
		
		if(fb_Kind < 42)
		{
			nRate = ((float)fishValue[fb_Kind] * fPercent);
			return rand()%nRate == 0 ? true : false;
		}
		else
		{
			nRate = ((float)arc_die * fPercent);
			return rand()%nRate == 1 ? true : false;
		}
	}
	//未到达峰值的时候
	if(nOverPeak == 0)
	{
		int nRate = 2;
		if(fb_Kind < 42)
		{
			nRate = ((float)fishValue[fb_Kind] * 0.8f);
			return rand()%nRate == 0 ? true : false;
		}
		else
		{
			nRate = ((float)arc_die * 0.8f);
			return rand()%nRate == 1 ? true : false;
		}
	}
	
	//特殊处理一些鱼的概率
	if(fb_Kind == 4)
	{
		return rand() % 8 == 2 ? true : false;
	}
	else if(fb_Kind == 9)
	{
		return rand() % (21 + rand() % 21) == 10 ? true : false;
	}
	else if(fb_Kind == 14)
	{
		return rand() % 18 == 10 ? true : false;
	}
	else if(fb_Kind == 19)
	{
		return rand() % 30 == 10 ? true : false;
	}
	else if(fb_Kind == 24)
	{
		return rand() % 70 == 10 ? true : false;
	}
	else if(fb_Kind == 29)
	{
		return rand() % (240 + rand()%120) == 10 ? true : false;
	}

	//话费鱼的概率
	if (fb_Kind == TELEPHONE_FARE_FISH) 
	{
		int nFare = UserData::getInstance()->getCurrentTelephoneFare();
		if(nFare > 900)
		{
			return false;
		}
		if(rand() % (nFare + 100) == 90)
		{
			return true;
		}
		return false;
	}
	else
	{
		if(fb_Kind < 42)
		{
			return rand()%fishValue[fb_Kind] == 1 ? true : false;
		}
		else
		{
			return rand()%arc_die == 1 ? true : false;
		}
	}

}

void FishBase::setBody()
{
    Manager* instance = Manager::getInstance();
    //instance->loadFishBodies(fb_Kind);
    fishbody = instance->loadFishBodies(fb_Kind);
    this->setPhysicsBody(fishbody);
	//instance->setFishBodies(NULL);
}
void FishBase::theSpecialEffects()
{
    if (bindToCannonNum == 1)
	{
        UserData::getInstance()->setGoldNum(UserData::getInstance()->getGoldNum() + value*(this->getBulletPower()));
        instance->setIsCoinChange(true);  //给金币刷新一个标记
    }
    
    if (fb_Kind >= 16)
    {
		MusicData::getInstance()->playSound(40);
		int nType = 1;
		if(fb_Kind >= 24 && fb_Kind < 30)
		{
			nType = 1;
		}
		else if(fb_Kind >= 30)
		{
			nType = 2;
		}
        //添加粒子效果 和奖励面板
        for (int i = 0; i < 30; i++)
        {
            if (i < 24) 
			{
                PixCell* pixcell = PixCell::createPixCell(nType, this->getPosition());
                pixcell->setRotation((float)13*i + (float)(rand()%5-2));
                
                instance->getM_layer()->addChild(pixcell, ZORDER_EFFECT);
            }
			else
            {
                PixCell* pixcell = PixCell::createPixCell(nType, this->getPosition());
                pixcell->setScale((float)(rand()%4+5)/10);
                pixcell->setRotation((float)13*i + (float)(rand()%5-2));
                
                instance->getM_layer()->addChild(pixcell, ZORDER_EFFECT);
            }
        }
    }
    //加载音乐
    if (fb_Kind == 20)
    {
        MusicData* music = MusicData::getInstance();
        music->playSound(58);
    }
    else if (fb_Kind == 21)
    {
        MusicData* music = MusicData::getInstance();
        music->playSound(67);
    }
    else if (fb_Kind == 22)
    {
        MusicData* music = MusicData::getInstance();
        music->playSound(66);
    }
     else if (fb_Kind == 23)
    {
        MusicData* music = MusicData::getInstance();
        music->playSound(65);
    }else if (fb_Kind == 24)
    {
        MusicData* music = MusicData::getInstance();
        music->playSound(66);
    }else if (fb_Kind == 25)
    {
        MusicData* music = MusicData::getInstance();
        music->playSound(67);
    }else if (fb_Kind == 26)
    {
        MusicData* music = MusicData::getInstance();
        music->playSound(68);
    }else if (fb_Kind == 27)
    {
        MusicData* music = MusicData::getInstance();
        music->playSound(55);
    }else if (fb_Kind == 28)
    {
        MusicData* music = MusicData::getInstance();
        music->playSound(56);
    }else if (fb_Kind == 29)
    {
        MusicData* music = MusicData::getInstance();
        music->playSound(57);
    }else if (fb_Kind == 30)
    {
        MusicData* music = MusicData::getInstance();
        music->playSound(58);
    }else if (fb_Kind == 31)
    {
        MusicData* music = MusicData::getInstance();
        music->playSound(59);
    }else if (fb_Kind == 32)
    {
        MusicData* music = MusicData::getInstance();
        music->playSound(60);
    }else if (fb_Kind == 33)
    {
        MusicData* music = MusicData::getInstance();
        music->playSound(61);
    }else if (fb_Kind == 34)
    {
        MusicData* music = MusicData::getInstance();
        music->playSound(62);
    }else if (fb_Kind == 35)
    {
        MusicData* music = MusicData::getInstance();
        music->playSound(63);
    }
    
   
        if (fb_Kind < 41)
        {
            
            //添加爆炸粒子
            if (fb_Kind > 10&&!isSpecial)
            {
                //添加粒子
                ParticleSystemQuad* pBoom = ParticleSystemQuad::create("particle/Desktop.plist");
                pBoom->setPosition(this->getPosition());
                instance->getM_layer()->addChild(pBoom, ZORDER_EFFECT);
            }else if (isSpecial)
            {
                //添加粒子
                ParticleSystemQuad* pBoom = ParticleSystemQuad::create("particle/explosion_small.plist");
                pBoom->setPosition(this->getPosition());
                instance->getM_layer()->addChild(pBoom, ZORDER_EFFECT);
            }
            

              
        }
    
    
    
}

Sprite* FishBase::getBasicSprite1()
{
	auto pSprite = (Sprite*)this->getChildByTag(BASICSP);
	return pSprite;
}
//加速进场
void FishBase::enterSpeedUp()
{
	this->getMoveSpeed()->setSpeed(6.0f);
	auto seq = Sequence::create(DelayTime::create(1.0f), 
		CallFunc::create(CC_CALLBACK_0(FishBase::callBackResumeSpeed, this)), 
		NULL);

	this->runAction(seq);
}
void FishBase::callBackResumeSpeed()
{
	this->getMoveSpeed()->setSpeed(m_nResumeSpeed);
}
//设置锁定BOSS
void FishBase::setAimedFish(int num)
{
	if(num == 0)
		this->setAimedByRobot1(true);
	else if(num == 1)
		this->setAimedByRobot2(true);
	else if(num == 2)
		this->setAimedByRobot3(true);
	else if(num == 3)
		this->setAimedByRobot4(true);
}
//获取是否被机器人锁定
bool FishBase::getAimedByRobot(int num)
{
	if(num == 0)
		return getAimedByRobot1();
	else if(num == 1)
		return getAimedByRobot2();
	else if(num == 2)
		return getAimedByRobot3();
	else if(num == 3)
		return getAimedByRobot4();
}
