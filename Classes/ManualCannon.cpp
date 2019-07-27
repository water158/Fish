//
//  ManualCannon.cpp
//  Helloworld
//
//  Created by etgame on 15-1-29.
//
//

#include "ManualCannon.h"
#include "Bullet.h"
#include "Manager.h"
#include "Data.h"
#define BULLET_INDEX  50
#include "MainScene.h"

#include "MusicData.h"
#include "UserData.h"
//构造函数把点击点传过来
ManualCannon::ManualCannon() : clickpoint(Point(0, 0)), CDTimer(0), energyTimer(0)
{
    isEnergyCannon = false;
	m_pDelegateGame = NULL;
	m_pXmlParser = NULL;
}
ManualCannon::~ManualCannon()
{
	if(m_pXmlParser)
	{
		m_pXmlParser->release();
	}
}

ManualCannon* ManualCannon::createCannon()
{
    ManualCannon* pRet = new ManualCannon();
    if (pRet && pRet->init()) 
	{
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

void ManualCannon::initConnon(  int powerOfBullet, int _type, int _num, bool& _cdtimer)
{
	m_pXmlParser = XMLParser::parseWithFile("ui/loading.xml");
	m_pXmlParser->retain();

    CDTimer = _cdtimer;
    instance = Manager::getInstance();
    mc_music = MusicData::getInstance();
    bulletPower = powerOfBullet;

	if (instance->getRoomNum() == 1)
	{
		for (int i = 0; i < 16; i++)
		{
			if (f_bulletPower[i] == bulletPower) 
			{
				c_powerNum = i;
			}
		}
	}else if (instance->getRoomNum() == 2)
	{
		int nNum = 10;
		if(instance->getTwo_LevelNum() < 3)
			nNum = 5;
		for (int  i = 0; i < nNum; i++)
		{
			if (d_bulletPower[instance->getTwo_LevelNum()-1][i] == bulletPower) 
			{
				c_powerNum = i;
			}
		}
	}

    type = _type;
    num = _num;
    isShoot = false;
    rcordTime = 0;
    totalCorn = UserData::getInstance()->getGoldNum();
    
    
    if (type == 0) {
        m_nFrameNum = 10;
    }else if (type == 1)
    {
        m_nFrameNum = 9;
    }else if (type == 2)
    {
        m_nFrameNum = 8;
    }else if (type == 3)
    {
        m_nFrameNum = 7;
    }
    else if (type == 4)
    {
        m_nFrameNum = 6;
    }
	m_bRage = false;
    
    
    //初始化值
    setScope(100);
    this->initCannonBase(); //添加炮台
    
	this->refreshUserdata();

	this->schedule(schedule_selector(ManualCannon::updateSubsidies), 1.0f);
}

void ManualCannon::update(float dt)
{
    float fAngle = 0;
    if (isShoot&&rcordTime % m_nFrameNum == 0) 
	{
        if (UserData::getInstance()->getGoldNum() >= bulletPower) 
		{
			//炮台的缩放效果
			ScaleTo* to1 = CCScaleTo::create(0.05, 0.95);
			ScaleTo* to2 = CCScaleTo::create(0.05, 1);
			Sequence* seq = CCSequence::createWithTwoActions(to1, to2);
			cannon->runAction(seq);

            flame1->setVisible(true);
            DelayTime* timer = DelayTime::create(0.01f);
            Sequence* seq_flame = Sequence::create(timer, CallFuncN::create(CC_CALLBACK_1(ManualCannon::setFlameBlink, this)), nullptr);
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
            
            int nNum = 1;
            if(this->getRage())
            {
                nNum = 5;
            }
            //log("==============");
			mc_music->playSound(28);
            for (int i = 0; i < nNum; i++)
            {
                if (UserData::getInstance()->getGoldNum() < bulletPower) {
                    return;
                }
                //加载音乐
                //if (i == 0)
                    //mc_music->playSound(28);
                
                //SimpleAudioEngine::getInstance()->stopEffect(MusicData::getInstance()->getS_ID());
                
                //添加子弹
                Bullet* bullet1 = Bullet::createBullet();
                
                float fRot = (i - 2)*30;
                
                Vec2 byPos = battery->convertToWorldSpace(cannon->getPosition());
                
                fAngle = calcAngleFromOriginToTarget(byPos, clickpoint) + fRot;
                
                if(nNum == 1)
                {
                    fAngle += 60;
                }
                float tx = cos(CC_DEGREES_TO_RADIANS(fAngle)) * 100;
                float ty = sin(CC_DEGREES_TO_RADIANS(fAngle)) * 100;
                
                Vec2 click = Vec2(tx, ty) + byPos;
                // log("tx = %f,%f", tx, ty);
                if(nNum == 1)
                {
  
                }

                bullet1->initBullet(byPos,  click,  bulletPower, type, num);
                //log("----------%f========%f========%f=======%f==", cannon->getAnchorPoint().x, cannon->getAnchorPoint().y, bullet->getAnchorPoint(). x, bullet->getAnchorPoint().y);
                bullet1->setRotation(cannon->getRotation());

                //确定子弹发出的炮台 作为标记
                bullet1->atNum = manualNum; 
                instance->getM_layer()->addChild(bullet1, BULLET_INDEX);
                instance->setIsCoinChange(true);
				Manager::getInstance()->setBulletNum(Manager::getInstance()->getBulletNum() + 1);
				//能量炮 获得金币加倍，消耗不加倍
                if (isEnergyCannon) 
				{ 
                    UserData::getInstance()->setGoldNum(UserData::getInstance()->getGoldNum() - bulletPower/2);
                }
				else
				{
                    UserData::getInstance()->setGoldNum(UserData::getInstance()->getGoldNum() - bulletPower);
				}
                
            }
        }
        else
		{
			if(m_pDelegateGame && Manager::getInstance()->getBulletNum() == 0)
			{
				int nBuyGift = UserData::getInstance()->getBuyGift();
				int nSubsidiesIndex = UserData::getInstance()->getSubsidiesIndex();
				if(nBuyGift < 2 && nSubsidiesIndex != 7)
				{
					m_pDelegateGame->showGiftLayer();
				}
				else if(nBuyGift >= 2 && nSubsidiesIndex != 7)
				{
					m_pDelegateGame->shopLayer();
				}
				if(UserData::getInstance()->getSubsidiesIndex() == 0)
				{
					UserData::getInstance()->setSubsidiesIndex(1);
					UserData::getInstance()->save();
				}
				this->getChildByTag(kTagFreeGoldBack)->setVisible(true);
				this->getChildByTag(kTagRewarded)->setVisible(true);
				if(nSubsidiesIndex == 7)
				{
					this->getChildByTag(kTagFreeGoldBack)->setVisible(false);
					m_pDelegateGame->showUnenoughLayer();
				}
			}
		}
        rcordTime = 0;
        
    }
    rcordTime++;
    energyTimer++;
    if (energyTimer % 400 == 0 && isEnergyCannon) 
	{
		type = m_nOldCannonID;

		 __String* str = __String::createWithFormat("%s%d.png", cannon_str[type].c_str(), num);
        SpriteFrame* cannon_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str->getCString());
        cannon->setDisplayFrame(cannon_frame);

        this->setFlame(num);
        isEnergyCannon = false;
        bulletPower /= 2;

		//移除能量炮粒子
		this->removeChildByTag(105);
    }
    //刷新金币显示
    if (instance->getIsCoinChange()) 
	{
	   int nNum = UserData::getInstance()->getGoldNum();
	   log("---(%d)------3333333333333333333333333-----", nNum);
	   if(nNum == -1)
	   {
		   log("-------+++++++++------");
	   }
       this->changeScore(nNum);
       instance->setIsCoinChange(false);
    }
    
}
//设置能量炮
void ManualCannon::setEnergyCannon()
{
	energyTimer = 0;

    this->addEnergyCannonLv();

    //__String* str = __String::createWithFormat("%s%d.png", cannon_str[5].c_str(), num);
    //SpriteFrame* cannon_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str->getCString());
	__String* str = __String::createWithFormat("cannon_power%d.png", num);
	SpriteFrame* cannon_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str->getCString());
    cannon->setDisplayFrame(cannon_frame);
    
    this->setFlame(num);
    isEnergyCannon = true;
    
    bulletPower *= 2;

	//添加能量炮粒子
	ParticleSystemQuad* pBoom = ParticleSystemQuad::create("particle/powerEffect.plist");
	pBoom->setPosition(Vec2(0.0f, -40.0f));
	this->addChild(pBoom, COIN_zOrder, 105);
}
//能量炮时，显示为下一等级炮
void ManualCannon::addEnergyCannonLv()
{
	m_nOldCannonID = type;
	type = 5;
}
//更新用户信息
void ManualCannon::refreshUserdata()
{
	auto pHeadIcon = this->getChildByTag(106);
	auto pLabel1 = (LabelTTF*)pHeadIcon->getChildByTag(107);
	auto pLabel2 = (LabelTTF*)pHeadIcon->getChildByTag(108);
	auto pLabel3 = (LabelTTF*)pHeadIcon->getChildByTag(kTagDiamondsNum);

	int nID = UserData::getInstance()->getPlayerID();
	int nLv = UserData::getInstance()->getExp_value() / 10000 + 1;
	int nDIamondsNum = UserData::getInstance()->getDiamonds();
	std::string strID = "ID: ";
	std::string strName = __String::createWithFormat("lv: %d", nLv)->getCString();
	pLabel1->setString(strName);
	pLabel2->setString(strID + getStringFromInt(nID));
	std::string strDiamonds = "x" + getStringFromInt(nDIamondsNum);
	pLabel3->setString(strDiamonds);

	//刷新VIP等级
	Vec2 posOld = pHeadIcon->getChildByTag(kTagVIPNum)->getPosition();
	pHeadIcon->removeChildByTag(kTagVIPNum);
	int nCurVIP = UserData::getInstance()->getCurVipLevel();
	auto pVIPNum = SpellNumber::createSpellNumber(nCurVIP);
	pVIPNum->setScale(0.6f);
	pVIPNum->setPosition(posOld);
	pHeadIcon->addChild(pVIPNum, 12, kTagVIPNum);
}
//加速
void ManualCannon::speedUp()
{
	if(this->getFrameNum() == 2)return;
    
	this->setFrameNum(2);
    
	auto seq = Sequence::create(DelayTime::create(SkillTime2),
                                CallFunc::create(CC_CALLBACK_0(ManualCannon::resumeSpeed, this)),
                                NULL);
	this->runAction(seq);
}
//恢复速度
void ManualCannon::resumeSpeed()
{
	this->setFrameNum(6);
}
//狂暴
void ManualCannon::rage()
{
	if(this->getRage())
	{
		return;
	}
	this->setRage(true);
	auto seq = Sequence::create(DelayTime::create(SkillTime3),
                                CallFunc::create(CC_CALLBACK_0(ManualCannon::cancelRage, this)),
                                NULL);
	this->runAction(seq);
}
//消除狂暴
void ManualCannon::cancelRage()
{
	this->setRage(false);
}

void ManualCannon::setFlameBlink(Node* ref)
{
    ref->setVisible(false);

}


//更新补贴时间
void ManualCannon::updateSubsidies(float dt)
{
	this->setWordsByIndex(0);
}

//通过下标设置显示
void ManualCannon::setWordsByIndex(int nIndex)
{
	int nSubIndex = UserData::getInstance()->getSubsidiesIndex();
	int nSubTime = UserData::getInstance()->getSubsidiesTime();
	auto pWords = (LabelTTF*)(((Menu*)this->getChildByTag(kTagFreeGoldBack))->getChildByTag(0)->getChildByTag(kTagMenuWords));

	tm* pTime = GetTime1();
	int nMon = pTime->tm_mon + 1;
	int nDay = pTime->tm_mday;
	int nHour = pTime->tm_hour;
	int nMin = pTime->tm_min;
	int nSec = pTime->tm_sec;

	int nOldHour = (nSubTime / 10000) % 100;
	int nOldMin = (nSubTime / 100) % 100;
	int nOldSec = nSubTime % 100;
	int nOldData = nOldSec + nOldMin*60 + nOldHour*3600;

	int nCurData = nSec + nMin*60 + nHour*3600;

	if(nSubIndex == 1)
	{
		__String* info = m_pXmlParser->getString("free_gold_1");
		pWords->setString(info->getCString());
		UserData::getInstance()->setSubsidiesIndex(nSubIndex + 1);
	}
	else if(nSubIndex == 3)
	{
		//时间未到就刷新时间
		if(nCurData - nOldData < 60)
		{
			int nMin1 = 0;	
			int nSec1 = 60 - abs(nCurData - nOldData);
			log("--nSec--==-(%d)---nOldSec-==-(%d)--nSec1-===--(%d)-------", nSec, nOldSec, nSec1);
			std::string str = "%02d:%02d";
			__String* pFormat = __String::createWithFormat(str.c_str(), nMin1, nSec1);
			__String* info = m_pXmlParser->getString("free_gold_2");
			__String* pWord = __String::createWithFormat(info->getCString(), pFormat->getCString());
			pWords->setString(pFormat->getCString());
		}
		//时间到了
		else
		{
			UserData::getInstance()->setSubsidiesIndex(nSubIndex + 1);
			__String* info = m_pXmlParser->getString("free_gold_1");
			pWords->setString(info->getCString());
		}
	}
	else if(nSubIndex == 5)
	{
		//时间未到就刷新时间
		if(nCurData - nOldData < 300)
		{
			int nMin1 = 4 - abs(nCurData - nOldData)/60;	
			int nSec1 = 60 - abs(nCurData - nOldData)%60;
			std::string str = "%02d:%02d";
			__String* pFormat = __String::createWithFormat(str.c_str(), nMin1, nSec1);
			__String* info = m_pXmlParser->getString("free_gold_2");
			__String* pWord = __String::createWithFormat(info->getCString(), pFormat->getCString());
			pWords->setString(pFormat->getCString());
		}
		//时间到了
		else
		{
			UserData::getInstance()->setSubsidiesIndex(nSubIndex + 1);
			__String* info = m_pXmlParser->getString("free_gold_1");
			pWords->setString(info->getCString());
		}
	}
	else if(nSubIndex == 7)
	{
		this->getChildByTag(kTagFreeGoldBack)->setVisible(false);
		//__String* info = m_pXmlParser->getString("free_gold_3");
		//pWords->setString(info->getCString());
	}

}
