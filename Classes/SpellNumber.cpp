//
//  SpellNumber.cpp
//  Qianpaobuyu
//
//  Created by etgame on 15-6-9.
//
//

#include "SpellNumber.h"
#include "MyTools.h"
SpellNumber::SpellNumber()
{}
SpellNumber::~SpellNumber()
{}
SpellNumber* SpellNumber::createSpellNumber(int num, int nType)
{
    SpellNumber* pRet = new SpellNumber();
    if (pRet&&pRet->initSpellNumber(num, nType)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}
bool SpellNumber::initSpellNumber(int num, int nType)
{
    m_nType = nType;
    if (num == 0)
    {
        __String* num_str = __String::createWithFormat("yellow_%d.png", num%10);
        Sprite* num_sp = Sprite::createWithSpriteFrameName(num_str->getCString());
        num_sp->setAnchorPoint(Vec2(0.5f, 0.5f));
        num_sp->setPosition(Point::ZERO);
        this->addChild(num_sp, 1);
		num_sp->setScale(0.7);
    }
    
    
    int i = 0;
    std::string str = getStringFromInt(num);
    int numLong = strlen(str.c_str());
    bool isPrime = false; //判断数字的位数是否为奇数
    if (numLong%2) {
        isPrime = true;
    }else
        isPrime = false;
    
    int longHalf = numLong/2; //数字位数的一半
    float yello_width = 16; //一个数字的宽度
    
    while (num)
    {
        i++;
        __String* num_str = __String::createWithFormat("yellow_%d.png", num%10);
        Sprite* num_sp = Sprite::createWithSpriteFrameName(num_str->getCString());
        num_sp->setAnchorPoint(Vec2(0.5f, 0.5f));
		num_sp->setScale(0.7);
        if (isPrime)
        {
            if (i <= longHalf)
            {
                num_sp->setPosition(Point(yello_width*(longHalf - i + 1) , 0));
            }else if (i == longHalf + 1)
            {
                num_sp->setPosition(Point(0, 0));
            }else if (i > longHalf + 1)
            {
                num_sp->setPosition(Point(-yello_width*(i - longHalf - 1) , 0));
            }
        }else
        {
            if (i <= longHalf)
            {
                num_sp->setPosition(Point(yello_width*(longHalf - i ) + yello_width/2, 0));
            }else if (i > longHalf)
            {
                num_sp->setPosition(Point(-yello_width*(i - longHalf-1) - yello_width/2, 0));
            }
        }
        
        
        
        this->addChild(num_sp, 1, i);
        
        num /= 10;
        
    }
    
    if(nType == 1)
	{
		auto pFirst = this->getChildByTag(i);
		if(pFirst)
		{
			auto pAdd = Sprite::createWithSpriteFrameName("yellow_jia.png");
			pAdd->setPosition(pFirst->getPositionX() - 20.0f, pAdd->getPositionY());
			this->addChild(pAdd, 1, 100);
		}

	}
    
    
    
    return true;
}
void SpellNumber::addAnimation()
{
	//屏幕上的数量，带缩放和渐隐
	if(m_nType == 1)
	{
		auto seq1 = Sequence::create(ScaleTo::create(0.3f, 1.5f), 
			ScaleTo::create(0.3f, 1.0f), 
			DelayTime::create(0.6f), 
			CallFunc::create([&]{this->removeNum();}), 
			NULL);
		this->runAction(seq1);
		int nTag = 1;
		while (this->getChildByTag(nTag))
		{
			auto pNum = this->getChildByTag(nTag);

			Sequence* seq2 = Sequence::create(DelayTime::create(0.6), FadeOut::create(0.6f), NULL);
			pNum->runAction(seq2);

			nTag ++;
		}
		auto pAdd = this->getChildByTag(100);
		Sequence* seq2 = Sequence::create(DelayTime::create(0.6), FadeOut::create(0.6f), NULL);
		pAdd->runAction(seq2);
	}
	//金币框的上面的
	else if(m_nType == 2)
	{

	}
	else
	{
		FadeTo* fade = FadeTo::create(1.0f, 0);
		Sequence* seq = Sequence::create(DelayTime::create(1), fade, CallFunc::create([&]{
			this->removeNum();
		}), nullptr);
		this->runAction(seq);
	}
}
void SpellNumber::removeNum()
{
    this->removeFromParentAndCleanup(true);
}