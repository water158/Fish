//
//  CannonBase.cpp
//  Helloworld
//
//  Created by etgame on 15-3-6.
//
//

#include "CannonBase.h"
#include "Data.h"

#include "Manager.h"
#include "MusicData.h"
#include "UserData.h"

#define SPELL_POWER_TAG 11
#define SPELL_SCORE_TAG 12

void CannonBase::initCannonBase()
{
    instance = Manager::getInstance();
    ca_music = MusicData::getInstance();
    battery = Sprite::createWithSpriteFrameName(battery_str[type]);
    __String* str = __String::createWithFormat("%s%d.png", cannon_str[type].c_str(), num);
    
    //创建大炮
    cannon = Sprite::createWithSpriteFrameName(str->getCString());
    cannon->setAnchorPoint(Point(0.5f, 0.0f));
    cannon->setPosition(Point(battery->getContentSize().width/2, 0));
    //创建显示面板
    display = Sprite::createWithSpriteFrameName(display_str[type]);
    display->setPosition(Point(battery->getContentSize().width/2, battery->getContentSize().height/2+1));
    battery->addChild(cannon, 8);
    battery->addChild(display, 10);
    
    this->addChild(battery,1);
   
    up_sp = Sprite::createWithSpriteFrameName(up_menu[type]);
    MenuItemSprite* up_item = MenuItemSprite::create(up_sp, up_sp, CC_CALLBACK_1(CannonBase::updateCannon, this));
    up_item->setTag(2);
    Menu* up_menu1 = Menu::createWithItem(up_item);
    up_menu1->setPosition(Point(cannon->getContentSize().width - 10, up_sp->getContentSize().height/2 - 20.0f));
    battery->addChild(up_menu1, 11, 100);

    down_sp = Sprite::createWithSpriteFrameName(down_menu[type]);
    MenuItemSprite* down_item = MenuItemSprite::create(down_sp, down_sp, CC_CALLBACK_1(CannonBase::updateCannon, this));
    down_item->setTag(1);
    Menu* down_menu1 = Menu::createWithItem(down_item);
    down_menu1->setPosition(Point(10, down_sp->getContentSize().height/2 - 20.0f));
    battery->addChild(down_menu1, 11, 101);
    
    spell_power = SpellNumber::createSpellNumber(bulletPower);
    spell_power->setPosition(Point(display->getContentSize().width/2, display->getContentSize().height/2 - 40));
    spell_power->setTag(SPELL_POWER_TAG);
    display->addChild(spell_power);
    
    //显示金币槽
    labelSlot = Sprite::createWithSpriteFrameName("money_bg.png");
    labelSlot->setPosition(Vec2(battery->getContentSize().width + 20.0f, -40));
    this->addChild(labelSlot, 12);
    labelSlot->setScale(0.6);
    //显示金币
    spell_score = SpellNumber::createSpellNumber(totalCorn);
    spell_score->setPosition(Point(labelSlot->getContentSize().width/2, labelSlot->getContentSize().height/2));
    spell_score->setTag(SPELL_SCORE_TAG);
    labelSlot->addChild(spell_score, 100);
    spell_score->setScale(1.5);

	//显示头像
	std::string strHead = "head.png";
	/*if(CCRANDOM_0_1() < 0.5f)
	strHead = "head1.png";*/
	auto pHeadIcon = Sprite::create(strHead);
	pHeadIcon->setScale(1.0f);
	pHeadIcon->setPosition(Vec2(labelSlot->getPositionX() - labelSlot->getContentSize().width/2 * labelSlot->getScale() + 
		pHeadIcon->getContentSize().width/2 * pHeadIcon->getScale(), 
		labelSlot->getPositionY() + pHeadIcon->getContentSize().height/2.0f * pHeadIcon->getScale() + 
		labelSlot->getContentSize().height/2.0f * labelSlot->getScale() - 13.0f));
	this->addChild(pHeadIcon, 12, 106);

	//省份
	auto pProvince = LabelTTF::create("shanxi", "Arial", 14);
	pProvince->setAnchorPoint(Vec2(0.0f, 0.0f));
	pProvince->setPosition(Vec2(pHeadIcon->getContentSize().width / 5.0f * pHeadIcon->getScale() * 2.0f + 5.0f, 
		11.0f));
	pHeadIcon->addChild(pProvince, 0, 107);
	pProvince->setColor(Color3B(0, 255, 255));
	//ID
	auto pLabelID = LabelTTF::create("ID: 123456", "Arial", 20);
	pLabelID->setAnchorPoint(Vec2(0.0f, 0.0f));
	pLabelID->setPosition(Vec2(pHeadIcon->getContentSize().width / 5.0f * pHeadIcon->getScale() * 2.0f, 
		pHeadIcon->getContentSize().height / 2.0f * pHeadIcon->getScale() - pLabelID->getContentSize().height / 2.0f - 2.0f));
	pHeadIcon->addChild(pLabelID, 0, 108);
	//钻石数量
	std::string strDiamonds = "x" + getStringFromInt(rand() % 1000);
	auto pLabelDiamonds = LabelTTF::create(strDiamonds, "Arial", 20);
	pLabelDiamonds->setAnchorPoint(Vec2(0.0f, 0.0f));
	pLabelDiamonds->setPosition(Vec2(pHeadIcon->getContentSize().width / 2.0f * pHeadIcon->getScale() + 5.0f, 
		pHeadIcon->getContentSize().height / 3 * 2.0f * pHeadIcon->getScale() - 2.0f));
	pHeadIcon->addChild(pLabelDiamonds, 0, kTagDiamondsNum);
	//VIP等级
	auto pVIPSprite = Sprite::create("text_vip.png");
    pVIPSprite->setAnchorPoint(Vec2(0.0f, 0.5f));
	pVIPSprite->setPosition(Vec2(10.0f, pVIPSprite->getContentSize().height - 3.0f));
	pHeadIcon->addChild(pVIPSprite, 12);
	int nCurVIP = rand() % 3;
	auto pVIPNum = SpellNumber::createSpellNumber(nCurVIP);
	pVIPNum->setScale(0.6f);
	pVIPNum->setPosition(Vec2(pVIPSprite->getContentSize().width + pVIPSprite->getPositionX() + 8.0f, 
		pVIPSprite->getPositionY() + 2.0f));
	pHeadIcon->addChild(pVIPNum, 12, kTagVIPNum);

	//名次
	if(Manager::getInstance()->getRoomNum() == 2)
	{
		auto pSprite = Sprite::create(__String::createWithFormat("ui/public/day_%d.png", 1)->getCString());
		pHeadIcon->addChild(pSprite, 0, kTagFinishRank);
	}


    this->setFlame(num);
    this->scheduleUpdate();
    this->setScale(SCALE);
    

	//添加免费金币领取按钮
	auto pMenuItem = MenuItemImage::create("button_freegold1.png", "button_freegold1.png", 
		CC_CALLBACK_1(CannonBase::callBackReceiveFreeGold, this));
	pMenuItem->setPosition(Vec2(0.0f, 0.0f));
	pMenuItem->setTag(0);

	//按钮上的文字
	auto pWords = LabelTTF::create("", "Arial", 32);
	pWords->setAnchorPoint(Vec2(0.0f, 0.5f));
	pWords->setPosition(Vec2(pMenuItem->getContentSize().width / 5.0f * 3.0f + 25.0f, 
		pMenuItem->getContentSize().height / 2.0f + 3.0f));
	pMenuItem->addChild(pWords, 0, kTagMenuWords);

	auto pMenu = Menu::create(pMenuItem, NULL);
	pMenu->setPosition(Vec2(0, 100));
	this->addChild(pMenu, 1, kTagFreeGoldBack);
	pMenu->setVisible(false);

	//已领完
	auto pRewarded = Sprite::create("button_freegold2.png");
	pRewarded->setPosition(pMenu->getPosition());
	this->addChild(pRewarded, 0, kTagRewarded);
	pRewarded->setVisible(false);
}
//领取免费金币的回调
void CannonBase::callBackReceiveFreeGold(Ref* pObj)
{
	int nIndex = UserData::getInstance()->getSubsidiesIndex();
	if(nIndex == 2 || nIndex == 4 || nIndex == 6)
	{
		MusicData::getInstance()->playSound(23);
		tm* pTime = GetTime1();
		int nMon = pTime->tm_mon + 1;
		int nDay = pTime->tm_mday;
		int nHour = pTime->tm_hour;
		int nMin = pTime->tm_min;
		int nSec = pTime->tm_sec;

		int nCurData = nSec + nMin*100 + nHour*10000 + nDay*1000000 + nMon*100000000;
		UserData::getInstance()->setSubsidiesTime(nCurData);
		UserData::getInstance()->setSubsidiesIndex(nIndex + 1);

		UserData::getInstance()->setGoldNum(UserData::getInstance()->getGoldNum() + 2000);
		UserData::getInstance()->save();

		Manager::getInstance()->setIsCoinChange(true);

		this->getChildByTag(kTagFreeGoldBack)->setVisible(false);
		this->getChildByTag(kTagRewarded)->setVisible(false);
		
		//this->schedule(schedule_selector(CannonBase::collectCoinEffect), 5.0/60.0f, 8, 0.1f);
		Size size = Director::getInstance()->getWinSize();
		//添加粒子
		ParticleSystemQuad* pBoom = ParticleSystemQuad::create("particle/gold_particle0.plist");
		pBoom->setPosition(Vec2(size.width / 2.0f, size.height / 3.0f * 2.0f));
		Manager::getInstance()->getM_layer()->addChild(pBoom, COIN_zOrder + 10000);
	}

}
//回收金币的效果
void CannonBase::collectCoinEffect(float dt)
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
	Manager::getInstance()->getM_layer()->addChild(sp, 1000);

	sp->runAction(RepeatForever::create(Animate::create(panimation)));

	auto seq = Sequence::create(MoveBy::create(0.5f, Vec2(0.0f, -50.0f)), 
		MoveTo::create(1.0, this->getPosition()),
		CallFuncN::create(CC_CALLBACK_1(CannonBase::callBackRemoveSelf, this)),
		NULL);

	sp->runAction(seq);
}
//移除自己
void CannonBase::callBackRemoveSelf(Node* pNode)
{
	pNode->removeFromParent();
}
void CannonBase::setFlame(int _num)
{
    flame1 = nullptr;
    flame2 = nullptr;
    flame3 = nullptr;

    switch (num) 
	{
        case 1:
            flame1 = createWithSpriteFrameName(flame_str[type].c_str());
            flame1->setAnchorPoint(Point(0.5, 0));
            flame1->setPosition(Point(cannon->getContentSize().width/2, cannon->getContentSize().height-16));
            cannon->addChild(flame1);
            flame1->setVisible(false);
            break;
        case 2:
            flame1 = createWithSpriteFrameName(flame_str[type].c_str());
            flame1->setAnchorPoint(Point(0.5, 0));
            flame1->setPosition(Point(cannon->getContentSize().width/2 - 15, cannon->getContentSize().height-16));
            cannon->addChild(flame1);
            flame1->setVisible(false);
            
            flame2 = createWithSpriteFrameName(flame_str[type].c_str());
            flame2->setAnchorPoint(Point(0.5, 0));
            flame2->setPosition(Point(cannon->getContentSize().width/2 + 15, cannon->getContentSize().height-16));
            cannon->addChild(flame2);
            flame2->setVisible(false);
            break;
        case 3:
            flame1 = createWithSpriteFrameName(flame_str[type].c_str());
            flame1->setAnchorPoint(Point(0.5, 0));
            flame1->setPosition(Point(cannon->getContentSize().width/2 - 30, cannon->getContentSize().height-16));
            cannon->addChild(flame1);
            flame1->setVisible(false);
            
            flame2 = createWithSpriteFrameName(flame_str[type].c_str());
            flame2->setAnchorPoint(Point(0.5, 0));
            flame2->setPosition(Point(cannon->getContentSize().width/2, cannon->getContentSize().height-16));
            cannon->addChild(flame2);
            flame2->setVisible(false);
            
            flame3 = createWithSpriteFrameName(flame_str[type].c_str());
            flame3->setAnchorPoint(Point(0.5, 0));
            flame3->setPosition(Point(cannon->getContentSize().width/2 + 30, cannon->getContentSize().height-16));
            cannon->addChild(flame3);
            flame3->setVisible(false);
            break;
            
        default:
            break;
    }

}
void CannonBase::removeFlame(int _num)
{
    if (_num == 1) 
	{
        flame1->removeFromParent();
    }else if (_num == 2) 
	{
        flame1->removeFromParent();
        flame2->removeFromParent();
    }else if (_num == 3) 
	{
        flame1->removeFromParent();
        flame2->removeFromParent();
        flame3->removeFromParent();
    }
}
//更新 炮的威力
void CannonBase::updateCannon(Ref* sender)
{
	//能量炮不能换
	if(this->getIsEnergyCannon())
	{
		return;
	}

    ca_music->playSound(33);
    Node* btn_node = dynamic_cast<Node*>(sender);
    int btn = btn_node->getTag();
    bool isChageCannon = false;
	if (instance->getRoomNum() == 1) 
	{
		if (btn == 1) 
		{
            c_powerNum--;
            if (c_powerNum < 0)
            {
                c_powerNum = 15;
            }
            
            if (c_powerNum == 2)
            {
                num = 1;
                isChageCannon = true;
            }
            else if (c_powerNum == 9)
            {
                num = 2;
                isChageCannon = true;
            }
            else if (c_powerNum == 15)
            {
                num = 3;
                isChageCannon = true;
            }
		}
		else if (btn == 2) 
		{
            c_powerNum++;
            if (c_powerNum > 15)
            {
                c_powerNum = 0;
            }
            if (c_powerNum == 3) {
                num = 2;
                isChageCannon = true;
            }
            else if (c_powerNum == 10)
            {
                num = 3;
                isChageCannon = true;
            }
            else if (c_powerNum == 0)
            {
                num = 1;
                isChageCannon = true;
            }
		}
        bulletPower = f_bulletPower[c_powerNum];
        
		instance->setM_bulletPower(bulletPower); //记录炮台威力
		instance->setM_BulletNum(num);

	}
	else if (instance->getRoomNum() == 2)
	{
		int nLevelNum = instance->getTwo_LevelNum() - 1;

		if (btn == 1) 
		{
			c_powerNum--;
			if(nLevelNum == 2 || nLevelNum == 3)
			{
				if (c_powerNum < 0)
				{
					c_powerNum = 9;
				}
			}
			else
			{
				if (c_powerNum < 0)
				{
					c_powerNum = 4;
				}
			}

			bulletPower = d_bulletPowerNew[nLevelNum][c_powerNum];

			if(nLevelNum == 2 || nLevelNum == 3)
			{
				if (bulletPower == d_bulletPowerNew[nLevelNum][9])
				{
					num = 3;
					isChageCannon = true;
				}
				else if (bulletPower == d_bulletPowerNew[nLevelNum][5]) 
				{
					num = 2;
					isChageCannon = true;
				} else if (bulletPower == d_bulletPowerNew[nLevelNum][2]) 
				{
					num = 1;
					isChageCannon = true;
				}
			}
			else
			{
				if (bulletPower == d_bulletPowerNew[nLevelNum][4])
				{
					num = 3;
					isChageCannon = true;
				}
				else if (bulletPower == d_bulletPowerNew[nLevelNum][3]) 
				{
					num = 2;
					isChageCannon = true;
				} else if (bulletPower == d_bulletPowerNew[nLevelNum][1]) 
				{
					num = 1;
					isChageCannon = true;
				}
			}
		}
		if (btn == 2)
		{
			c_powerNum++;
			if(nLevelNum == 0 || nLevelNum == 1)
			{
				if (c_powerNum == 5)
				{
					c_powerNum = 0;
				}
			}
			else
			{
				if (c_powerNum == 10)
				{
					c_powerNum = 0;
				}
			}

			bulletPower = d_bulletPowerNew[nLevelNum][c_powerNum];
			if(nLevelNum == 0 || nLevelNum == 1)
			{
				if (bulletPower == d_bulletPowerNew[nLevelNum][0])
				{
					num = 1;
					isChageCannon = true;
				}
				else if (bulletPower == d_bulletPowerNew[nLevelNum][2]) 
				{
					num = 2;
					isChageCannon = true;
				}
				else if (bulletPower == d_bulletPowerNew[nLevelNum][4])
				{
					num = 3;
					isChageCannon = true;
				}
			}
			else
			{
				if (bulletPower == d_bulletPowerNew[instance->getTwo_LevelNum() - 1][0])
				{
					num = 1;
					isChageCannon = true;
				}
				else if (bulletPower == d_bulletPowerNew[nLevelNum][3]) 
				{
					num = 2;
					isChageCannon = true;
				}
				else if (bulletPower == d_bulletPowerNew[nLevelNum][6])
				{
					num = 3;
					isChageCannon = true;
				}
			}
		}
		instance->setM_bulletPower(bulletPower); //记录炮台威力
		instance->setM_BulletNum(num);
	}

	int nID = UserData::getInstance()->getCannonID();
    UserData::getInstance()->setCannonEnergyByID(nID, bulletPower);
	UserData::getInstance()->save();
    if (isChageCannon) 
	{
        cannon->removeFromParent();
        cannon = NULL;
        __String* c_str = __String::createWithFormat("%s%d.png", cannon_str[type].c_str(), num);
        cannon = Sprite::createWithSpriteFrameName(c_str->getCString());
        cannon->setAnchorPoint(Point(0.5, 0));
        cannon->setPosition(Point(battery->getContentSize().width/2, 0));
        battery->addChild(cannon);
        this->setFlame(num);
    }
    
    this->addChangeAnimation(Vec2(0, 0));

    this->changePower();
    this->changeCannonPos();
}
void CannonBase::changePower()
{
    display->removeChildByTag(SPELL_POWER_TAG);

    spell_power = SpellNumber::createSpellNumber(bulletPower);
    spell_power->setPosition(Point(display->getContentSize().width/2, display->getContentSize().height/2 - 40));
    spell_power->setTag(SPELL_POWER_TAG);
    display->addChild(spell_power);
    if (this->getPositionY() > 200)
    {
        spell_power->setRotation(180);
    }
    
}
void CannonBase::changeScore(int coinNum)
{
    labelSlot->removeChildByTag(SPELL_SCORE_TAG);
	
    spell_score = SpellNumber::createSpellNumber(coinNum);
    spell_score->setPosition(Point(labelSlot->getContentSize().width/2, labelSlot->getContentSize().height/2));
    spell_score->setTag(SPELL_SCORE_TAG);
    spell_score->setScale(1.5);
    labelSlot->addChild(spell_score, 100);
    if (this->getPositionY() > 200)
    {
        spell_score->setRotation(180);
    }
}
//改变金币框的位置
void CannonBase::changeCannonPos()
{
	int nPosID = this->getM_chosePositionNum();
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
	
	if(nPosID == 2 || nPosID == 3 && instance->getRoomNum() == 2)
	{
		pHeadIcon->setRotation(180);
		pHeadIcon->setPosition(Vec2(labelSlot->getPositionX() - labelSlot->getContentSize().width/2 * labelSlot->getScale() + 
			pHeadIcon->getContentSize().width/2 * pHeadIcon->getScale() - 18.0f, 
			labelSlot->getPositionY() + pHeadIcon->getContentSize().height/2.0f * pHeadIcon->getScale() + 
			labelSlot->getContentSize().height/2.0f * labelSlot->getScale() - 6.0f));
	}
	else
	{
		pHeadIcon->setRotation(0);
	}

	//领取金币的条
	auto pFreeGold = ((Menu*)this->getChildByTag(kTagFreeGoldBack))->getChildByTag(0);
	auto pFreeGold1 = this->getChildByTag(kTagRewarded);
	if(nPosID == 2 || nPosID == 3 && instance->getRoomNum() == 2)
	{
		pFreeGold->setRotation(180);
		pFreeGold1->setRotation(180);
	}
	else
	{
		pFreeGold->setRotation(0);
		pFreeGold1->setRotation(0);
	}
}
//在金币条上显示的数量
Vec2 CannonBase::getAddCoinPosition()
{
	return Vec2(labelSlot->getPositionX(), labelSlot->getPositionY() + 90.0f);
}
void CannonBase::addChangeAnimation(Vec2 pos)
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("change_cannon_anim.plist");
	//获取动画帧数和缓存中的标记
	Vector<SpriteFrame*> array1(7);

	for (int i=1; i <= 7; i++)
	{
		SpriteFrameCache *pcache = SpriteFrameCache::getInstance();
		std::string format = "change_cannon_anim_%02d.png";
		__String* strName = __String::createWithFormat(format.c_str(), i);
		SpriteFrame* pframe = pcache->getSpriteFrameByName(strName->getCString());
		array1.pushBack(pframe);
	}
	//根据数组创建动画并添加到动画缓存
	Animation *panimation = Animation::createWithSpriteFrames(array1, 0.1f);

	Sprite* sp = Sprite::create();
	sp->setAnchorPoint(Vec2(0.5, 0));
	sp->setPosition(Vec2(battery->getContentSize().width/2, 0.0f));
	battery->addChild(sp, 100);
	//sp->setScale(0.75f);

	auto seq = Sequence::create(Repeat::create(Animate::create(panimation), 1), 
		CallFuncN::create(CC_CALLBACK_1(CannonBase::callBackRemove, this)), 
		NULL);
	sp->runAction(seq);

	MusicData::getInstance()->playSound(73);
}
void CannonBase::callBackRemove(Node* pNode)
{
	pNode->removeFromParent();
}
//更新用户信息
void CannonBase::refreshUserdata()
{

}
//更新名次
void CannonBase::refreshRanking(int nNum1, int nNum2, int nNum3)
{
	int nNum = this->getFinishNum();
	int nRank = 1;
	if(nNum < nNum1)
		nRank ++;
	if(nNum < nNum2)
		nRank ++;
	if(nNum < nNum3)
		nRank ++;

	f_nCurrentRanking = nRank;

	auto pHeadIcon = (Sprite*)this->getChildByTag(106);
	pHeadIcon->removeChildByTag(kTagFinishRank);

	auto pSprite = Sprite::create(__String::createWithFormat("ranking_%d.png", nRank)->getCString());
	pSprite->setPosition(Vec2(pHeadIcon->getContentSize().width * pHeadIcon->getScale() / 4.0f - 7.0f, 
		pHeadIcon->getContentSize().height * pHeadIcon->getScale() / 3.0f * 2.0f));
	pHeadIcon->addChild(pSprite, 0, kTagFinishRank);
}
//检测任务鱼
void CannonBase::checkTaskFish(int nID)
{
	if(nID < 28 || Manager::getInstance()->getTaskOver())
		return;

	int nID1 = Manager::getInstance()->getTaskFishID1();
	int nID2 = Manager::getInstance()->getTaskFishID2();
	int nID3 = Manager::getInstance()->getTaskFishID3();
	int nNum1 = this->getTaskFishNum1();
	int nNum2 = this->getTaskFishNum2();
	int nNum3 = this->getTaskFishNum3();

	int nAimNum = 10;

	if(nID == nID1)
	{
		if(nNum1 < 10)
			this->setTaskFishNum1(nNum1 + 1);
	}
	else if(nID == nID2)
	{
		if(nNum2 < 5)
			this->setTaskFishNum2(nNum2 + 1);
	}
	else if(nID == nID3)
	{
		if(nNum3 < 1)
			this->setTaskFishNum3(nNum3 + 1);
	}
}
//获取完成的个数
int CannonBase::getFinishNum()
{
	return m_nTaskFishNum1 + m_nTaskFishNum2 + m_nTaskFishNum3;
}
//重置任务数据
void CannonBase::resetTaskData()
{
	m_nTaskFishNum1 = 0;
	m_nTaskFishNum2 = 0;
	m_nTaskFishNum3 = 0;
}
//获取是否完成任务
bool CannonBase::getIsFinishTask()
{
	if(this->getTaskFishNum1() >= 10 && this->getTaskFishNum2() >= 5 && this->getTaskFishNum3() >= 1)
	{
		return true;
	}
	return false;
}

CannonBase::CannonBase():scope(0), flame1(nullptr), flame2(nullptr), flame3(nullptr)
{
	if(Manager::getInstance()->getRoomNum() == 2 && Manager::getInstance()->getTwo_LevelNum() < 3)
		c_powerNum = 2;
	else
		c_powerNum = 3;
	m_nDiamondsNum = rand() % 1000;
	m_nTaskFishNum1 = 0;
	m_nTaskFishNum2 = 0;
	m_nTaskFishNum3 = 0;
	m_bLockFish = false;
}

CannonBase::~CannonBase()
{
}



void CannonBase::kindOfWeapon(int number)
{
    
}

void CannonBase::initPlayer()
{
   
    player = CCSprite::create("Icon-114.png");
    
    
    
}


