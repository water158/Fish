//
//  Fire.cpp
//  Helloworld
//
//  Created by etgame on 15-1-28.
//
//

#include "Fire.h"

#include "FishBase.h"
#include "Bullet.h"
#include "MyTools.h"
#define BULLET_INDEX 30
#include "Manager.h"
#include "Data.h"

Fire::Fire():neanestFish(nullptr), origRotation(0), f_coinIsChange(false), recordAppearTime(0), fireNum(0), energyTimer(0), f_nOldCannonID(0)//, isThere(false)
{
	m_bCouldLockFish = true;
	m_nTaskFishNum1 = 0;
	m_nTaskFishNum2 = 0;
	m_nTaskFishNum3 = 0;
	recordAppearTime = 0;
	f_nCurrentRanking = 0;
}
Fire::~Fire()
{
	auto pPre = (NoseeFish*)this->getChildByTag(NSFISH);
	if(pPre)
		instance->m_fishVect.eraseObject(pPre);
	this->removeChildByTag(NSFISH);
}


Fire* Fire::createFire()
{
    Fire* pRet = new Fire();
    if (pRet&&pRet->init()) 
	{
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

void Fire::initFire( int powerOfBullet, int _type, int _num)
{
    bulletPower = powerOfBullet;
    type = _type;
    if (type == 1) 
	{
        type = 2;
    }
    num = _num;
	m_bSpeedUp = false;
    f_recordTime = 0;
    f_nFrameNum = 0;
    ori_rate = 0;
	m_bCouldShoot = true;
	isEnergyCannon = false;
	m_bShootBoss = false;
    
    f_recordshootKind = 0;
    instance = Manager::getInstance();
    size = Director::getInstance()->getWinSize();
    if (type == 0) 
	{
        f_nFrameNum = 10;
    }
	else if (type == 1)
    {
        f_nFrameNum = 9;
    }
	else if (type == 2)
    {
        f_nFrameNum = 8;
    }
	else if (type == 3)
    {
        f_nFrameNum = 7;
    }
    else if (type == 4)
    {
        f_nFrameNum = 6;
    }
    setScope(400);
 
    int initCoin = limitCoin[instance->getTwo_LevelNum()];
    if (initCoin == 0) 
	{
        initCoin = 10000;
    }

    // 初始化金币数
    totalCorn = rand() % 10000000  + initCoin * 10; 
	if(Manager::getInstance()->getTwo_LevelNum() == 1)
	{
		totalCorn = rand() % 100000 + 500000;
	}

	//随机炮台威力
	this->randCannonPower();

    this->initCannonBase();//创建炮台
    
    this->schedule(schedule_selector(Fire::simulationPlyaer), 1);
    this->scheduleUpdate();
    
    //隐藏加减按钮
	auto pUp = battery->getChildByTag(100);
    auto pDown = battery->getChildByTag(101);
	pUp->setVisible(false);
	pDown->setVisible(false);

	this->refreshUserdata();

	this->removeChildByTag(kTagFreeGoldBack);
}
//随机威力
void Fire::randCannonPower()
{
	int nLevelNum = Manager::getInstance()->getTwo_LevelNum();
	int nRandNum = rand() % 10;
	if(nLevelNum < 3)
		nRandNum = rand() % 5;
	bulletPower = d_bulletPowerNew[nLevelNum - 1][nRandNum];
	num = m_arrCannonFlameNumNew[nLevelNum - 1][nRandNum];
	//随机大炮的中类
	int fireKind = rand() % 5;
	int nRoomNum = Manager::getInstance()->getTwo_LevelNum();
	while (fireKind == 1)
	{
		fireKind = rand() % 5;
	}
	if(nRoomNum != 1)
	{
		while (fireKind <= 1)
		{
			fireKind = rand() % 5;
		}
	}
	type = fireKind;
}

void Fire::simulationPlyaer(float dt)
{
    int i = rand()%30;
    int time = rand()%30;
    if (time == i) 
	{
        if (isEnergyCannon)
        {
            return;
        }
        //this->unschedule(schedule_selector(Fire::rotateAndShoot));
        this->unscheduleUpdate();
        CallFunc* func1 = CallFunc::create([&]
        {
			int nOldNum = num;
            this->randCannonPower();
            __String* cannon_chage_str = __String::createWithFormat("%s%d.png", cannon_str[type].c_str(), num);
            SpriteFrame* cannon_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(cannon_chage_str->getCString());
            cannon->setSpriteFrame(cannon_frame);
            SpriteFrame* battery_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(battery_str[type]);
            battery->setSpriteFrame(battery_frame);
            SpriteFrame* displayer_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(display_str[type]);
            display->setSpriteFrame(displayer_frame);
            
            SpriteFrame* upFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(up_menu[type]);
            up_sp->setSpriteFrame(upFrame);
            SpriteFrame* downFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(down_menu[type]);
            down_sp->setSpriteFrame(downFrame);

            this->removeFlame(nOldNum);
          
            this->changePower();
            
            this->setFlame(num);
        });
        CallFunc* func2 = CallFunc::create([&]
        {
            this->scheduleUpdate();
        });
        Sequence* f_seq = Sequence::create(DelayTime::create(1), func1, DelayTime::create(1), func2, nullptr);
        this->runAction(f_seq);
    }
    
}
void Fire::update(float dt)
{
	if(!m_bCouldShoot)
	{
		return;
	}

    recordAppearTime++;
	this->checkNearestFish();
	m_bShootBoss = aimAtBoss();
		
    Point rotateVector;
    
    //控制鱼在屏幕可视范围内 可被射击
    if (f_recordTime%f_nFrameNum == 0 && instance->m_fishVect.contains(neanestFish)
		/*&& nearPoint.x > 0 && nearPoint.y > 0 && nearPoint.x < size.width && nearPoint.y < size.height*/)
	{
        rotateVector = nearPoint - battery->convertToWorldSpace(cannon->getPosition());
        float rotateRadians = rotateVector.getAngle();
        float rotateDegress = 0;
        if (this->getPositionY() >  200)
		{
            rotateDegress = CC_RADIANS_TO_DEGREES(-1*rotateRadians) + 90 + 180;
        }
		else
            rotateDegress = CC_RADIANS_TO_DEGREES(-1*rotateRadians) + 90;
        
        Sequence* seq = Sequence::create(RotateTo::create(0.1f, rotateDegress-origRotation), CCCallFuncN::create(CC_CALLBACK_1(Fire::shoot, this)), nullptr);
        cannon->runAction(seq);
        f_recordTime = 0;
    }
    f_recordTime++;
    
    //刷新金币显示
    if (f_coinIsChange)
    {
        this->changeScore(totalCorn);
        f_coinIsChange = true;
    }
    energyTimer++;
    
    //能量炮到时间
    if (energyTimer == 400  && isEnergyCannon)
    {
        type = f_nOldCannonID;
        __String* str = __String::createWithFormat("%s%d.png", cannon_str[type].c_str(), num);
        SpriteFrame* cannon_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str->getCString());
        cannon->setDisplayFrame(cannon_frame);
        this->setFlame(num);
        isEnergyCannon = false;
        bulletPower /= 2;
        //移除能量炮粒子
        this->removeChildByTag(105);
    }
    
}

void Fire::shoot(Node* ref)
{
    //炮台的缩放
    ScaleTo* to1 = ScaleTo::create(0.05f, 0.95f);
    ScaleTo* to2 = ScaleTo::create(0.05f, 1);
    Sequence* seq = Sequence::createWithTwoActions(to1, to2);
    cannon->runAction(seq);
    
    
    if (neanestFish && totalCorn >= bulletPower) 
	{
        flame1->setVisible(true);
        DelayTime* timer = DelayTime::create(0.01f);
        
        Sequence* seq_flame = Sequence::create(timer, CallFuncN::create(CC_CALLBACK_1(Fire::setFlameBlink, this)), nullptr);
        flame1->runAction(seq_flame);
        if (flame2) 
		{
            flame2->setVisible(true);
            flame2->runAction(seq_flame->clone()); //克隆一个动作，否则动作removeAction时，找不到target
        }
        if (flame3) 
		{
            flame3->setVisible(true);
            flame3->runAction(seq_flame->clone());
        }
        //添加子弹
        Bullet* bullet1 = Bullet::createBullet();
       
        bullet1->initBullet( battery->convertToWorldSpace(cannon->getPosition()),  nearPoint,  bulletPower, type, num);
        bullet1->setRotation(this->getRotation());
        bullet1->atNum = fireNum; //确定子弹发出的炮台 作为标记
        instance->getM_layer()->addChild(bullet1, ZORDER_BULLET);
        bullet1->setHolderID(this->getf_chosePositionNum());
        if (isEnergyCannon)
        {
            totalCorn -= bulletPower/2;
        }else
            totalCorn -= bulletPower;
        
        f_coinIsChange = true;
        
    }
    //neanestFish = nullptr;
}
void Fire::setFlameBlink(Node* ref)
{
    ref->setVisible(false);
}

//随机位置
void Fire::randPosition(Node* pNode)
{
	auto pNsFish = (NoseeFish*)pNode;
	//计算时间
	Vec2 posOld = pNsFish->getPosition();
	Vec2 posNew = Vec2(rand()%(int)size.width, rand()%(int)size.height);
	float fTime = abs(posNew.x - posOld.x) / 200.0f + CCRANDOM_0_1()*0.5f;

	auto seq = Sequence::create(MoveTo::create(fTime, posNew), 
		CallFuncN::create(CC_CALLBACK_1(Fire::randPosition, this)),
		NULL);
	
	pNsFish->runAction(seq);
}
//瞄准BOSS
bool Fire::aimAtBoss()
{
	auto pNsFish = (NoseeFish*)this->getChildByTag(NSFISH);
	//不可锁定目标就返回
	if(!m_bCouldLockFish)
	{
		pNsFish->resumeSchedulerAndActions();
		return false;
	}

	//有无被锁定的
	for (auto &fish : instance->m_fishVect)
	{
		FishBase* pFish = dynamic_cast<FishBase*>(fish);

		if (pFish->getAimedByRobot(this->getf_chosePositionNum()) && !pFish->getIsCatch()) 
		{
			Vec2 pos = pFish->getPosition();
			if(pos.x > 0 && pos.y > 0 && pos.x < size.width && pos.y < size.height)
			{
				nearPoint = pFish->getPosition();
				m_bLockFish = true;
				return true;
			}
			else
			{
				pNsFish->resumeSchedulerAndActions();
				m_bLockFish = false;
				return false;
			}
		}
	}
	m_bLockFish = false;
	//遍历一遍寻找锁定目标
	int nAimID1 = Manager::getInstance()->getTaskFishID1();
	int nAimID2 = Manager::getInstance()->getTaskFishID2();
	int nAimID3 = Manager::getInstance()->getTaskFishID3();
	bool bHasBoss = false;
	for (auto &fish : instance->m_fishVect)
	{
		FishBase* pfish = dynamic_cast<FishBase*>(fish);
		
		if ((pfish->fb_Kind == nAimID1 || pfish->fb_Kind == nAimID2 || pfish->fb_Kind == nAimID3) && !pfish->getIsCatch()) 
		{
			Vec2 pos = pfish->getPosition();
			
			if(pos.x > 0 && pos.y > 0 && pos.x < size.width && pos.y < size.height)
			{
				bHasBoss = true;
				if(CCRANDOM_0_1() < 0.5f && m_bCouldShoot && !this->getIsFinishTaskFish(pfish->fb_Kind))
				{
					pfish->setAimedFish(this->getf_chosePositionNum());
					nearPoint = pfish->getPosition();
					pNsFish->pauseSchedulerAndActions();
					if(this->getCurrentRanking() == 1)
					{
						this->speedUp();
					}
					return true;
				}
				else
				{
					continue;
				}
			}
		}
	}

	if(bHasBoss)
	{
		m_bCouldLockFish = false;
		auto seq = Sequence::createWithTwoActions(DelayTime::create(rand()%5), 
			CallFuncN::create(CC_CALLBACK_1(Fire::callBackCouldLockFish, this)));
		this->runAction(seq);
		return false;
	}
	//没有目标，也没有锁定目标
	pNsFish->resumeSchedulerAndActions();
	return false;
}

void Fire::checkNearestFish()
{
	//没有目标鱼就添加一个
	if(!this->getChildByTag(NSFISH))
	{
		NoseeFish* fish = NoseeFish::createNoseeFish();
		fish->setPosition(Vec2(size.width/2, size.height/2));
		fish->initNoseeFish(1);
		this->addChild(fish);
		fish->setTag(NSFISH);
		instance->m_fishVect.pushBack(fish);
		instance->setIsLockFish(false);

		auto seq = Sequence::create(CallFuncN::create(CC_CALLBACK_1(Fire::randPosition, this)), NULL);
		fish->runAction(seq);
	}
	//有目标鱼就更新一下位置
	else
	{
		auto pPre = (NoseeFish*)this->getChildByTag(NSFISH);
		
		neanestFish = pPre;
		nearPoint = pPre->getPosition();
	}

}
void Fire::setEnergy()
{
    energyTimer = 0;
    
    this->addEnergyCannonLv();
    __String* str = __String::createWithFormat("%s%d.png", cannon_str[5].c_str(), num);
    SpriteFrame* cannon_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str->getCString());
    cannon->setDisplayFrame(cannon_frame);
    this->setFlame(num);
    bulletPower *= 2;
    isEnergyCannon = true;
    //f_nFrameNum = 2;
    
    //添加能量炮粒子
	ParticleSystemQuad* pBoom = ParticleSystemQuad::create("particle/powerEffect.plist");
	pBoom->setPosition(Vec2(0.0f, -40.0f));
	this->addChild(pBoom, COIN_zOrder, 105);
}
//回复锁定boss
void Fire::callBackCouldLockFish(Node* pNode)
{
	m_bCouldLockFish = true;
}
//能量炮时，显示为下一等级炮
void Fire::addEnergyCannonLv()
{
	f_nOldCannonID = type;
	type = 5;
}
void Fire::changeCannonPos()
{
	int nPosID = this->getf_chosePositionNum();
	if(nPosID == 0)
		labelSlot->setPosition(Vec2(-battery->getContentSize().width , -40));
	else if(nPosID == 1)
		labelSlot->setPosition(Vec2(battery->getContentSize().width , -40));
	else if(nPosID == 2)
		labelSlot->setPosition(Vec2(battery->getContentSize().width , -40));
	else if(nPosID == 3)
		labelSlot->setPosition(Vec2(-battery->getContentSize().width , -40));

	auto pHeadIcon = this->getChildByTag(106);
	pHeadIcon->setPosition(Vec2(labelSlot->getPositionX() - labelSlot->getContentSize().width/2 * labelSlot->getScale() + 
		pHeadIcon->getContentSize().width/2 * pHeadIcon->getScale(), 
		labelSlot->getPositionY() + pHeadIcon->getContentSize().height/2.0f * pHeadIcon->getScale() + 
		labelSlot->getContentSize().height/2.0f * labelSlot->getScale() - 13.0f));
	
	if(nPosID == 2 || nPosID == 3)
	{
		pHeadIcon->setRotation(180);
		pHeadIcon->setPosition(Vec2(labelSlot->getPositionX() - labelSlot->getContentSize().width/2 * labelSlot->getScale() + 
			pHeadIcon->getContentSize().width/2 * pHeadIcon->getScale() - 18.0f, 
			labelSlot->getPositionY() + pHeadIcon->getContentSize().height/2.0f * pHeadIcon->getScale() + 
			labelSlot->getContentSize().height/2.0f * labelSlot->getScale() - 6.0f));
	}
}
//更新用户信息
void Fire::refreshUserdata()
{
	auto pHeadIcon = this->getChildByTag(106);
	auto pLabel1 = (LabelTTF*)pHeadIcon->getChildByTag(107);
	auto pLabel2 = (LabelTTF*)pHeadIcon->getChildByTag(108);
	auto pLabel3 = (LabelTTF*)pHeadIcon->getChildByTag(kTagDiamondsNum);

	std::string strID = "ID: ";
	std::string strName = __String::createWithFormat("LV: %d", rand() % 10 + 1)->getCString();
	pLabel1->setString(strName);
	m_nPlayerID = rand() % 900000 + 100000;
	pLabel2->setString(strID + getStringFromInt(m_nPlayerID));
	std::string strDiamonds = "x" + getStringFromInt(this->getDiamondsNum());
	pLabel3->setString(strDiamonds);
}
//加速射击
void Fire::speedUp()
{
	if(m_bSpeedUp) return;
    ori_rate = f_nFrameNum;
	this->setF_nFrameNum(2);
	auto seq = Sequence::create(DelayTime::create(10),
		CallFuncN::create(CC_CALLBACK_1(Fire::callBackResumeSpeed, this)),
		NULL);
	this->runAction(seq);

	m_bSpeedUp = true;
}
void Fire::callBackResumeSpeed(Node* pNode)
{
    f_nFrameNum = ori_rate;
    this->setF_nFrameNum(f_nFrameNum);
    
	//this->unschedule(schedule_selector(Fire::rotateAndShoot));
	//this->schedule(schedule_selector(Fire::rotateAndShoot), 0.1f);

	m_bSpeedUp = false;
}
//随机任务完成的数量
void Fire::randTaskNum()
{
	//随机随机彩金奖池金币数量
	tm* stm = GetTime1();
	int nMin = stm->tm_min%20;

	int nTemp = nMin / 2;
	if(nTemp == 0)
		nTemp ++;

	m_nTaskFishNum1 = rand() % nTemp;
	m_nTaskFishNum2 = rand() % (nMin / 4 + 1);
	m_nTaskFishNum3 = 0;
}
//获取此ID的鱼是否已经捕捉够
bool Fire::getIsFinishTaskFish(int nID)
{
	if(nID < 30)
	{
		if(this->getTaskFishNum1() >= 10)
			return true;
	}
	else if(nID >= 30 && nID <= 32)
	{
		if(this->getTaskFishNum2() >= 5)
			return true;
	}
	else
	{
		if(this->getTaskFishNum3() >= 1)
			return true;
	}

	return false;
}