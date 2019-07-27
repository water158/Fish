//
//  CoinPrompt.cpp
//  Buyudaren
//
//  Created by etgame on 15-4-28.
//
//

#include "CoinPrompt.h"
#include "MyTools.h"

CoinPrompt* CoinPrompt::createCoinPrompt(int index, Point _point, int _coinNum)
{
    CoinPrompt* pRet = new CoinPrompt();
    if (pRet&&pRet->initCoinPrompt(index, _point, _coinNum)) 
	{
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}
bool CoinPrompt::initCoinPrompt(int index, Point _point, int _coinNum)
{
    fishKind = index;
    point = _point;
    coinNum = _coinNum;
    
    bg_sp = Sprite::createWithSpriteFrameName("getMoney_bg.png");
    this->addChild(bg_sp);
    RotateBy* rotateby = RotateBy::create(1, 180);
	auto pSeq = Sequence::create(ScaleTo::create(0.5f, 1.5f), ScaleTo::create(0.5f, 1.0f), RotateBy::create(1, 500), NULL);
    bg_sp->runAction(pSeq);
    switch (fishKind) 
	{
        case 24:
            fish_name = Sprite::createWithSpriteFrameName("getDabaisha.png");
            break;
        case 26:
            fish_name = Sprite::createWithSpriteFrameName("getDajinsha.png");
            break;
        case 28:
            fish_name = Sprite::createWithSpriteFrameName("getHaitun.png");
            break;
        case 30:
            fish_name = Sprite::createWithSpriteFrameName("getHangjinsha.png");
            break;
    
        default:
            break;
    }
    if (fish_name) 
	{
        fish_name->setPosition(Point(0, -50));
        this->addChild(fish_name, 10);
    }
   
    Sequence* seq = Sequence::create(DelayTime::create(2), CallFunc::create(CC_CALLBACK_0(CoinPrompt::removeCoinPrompt, this)), nullptr);
    this->runAction(seq);
    
	std::string strNum = getStringFromInt(coinNum);
	int nLength = strlen(strNum.c_str());

	float fHalf = 0;

	fHalf = nLength/2.0f;
	
	//coinNum = 1000;
	Node* pNumNode = Node::create();
	float fWidth = 0;
	int nIndex = 0;
	while (coinNum) 
	{
		__String* pStr = __String::createWithFormat("big_%d.png", coinNum%10);
		Sprite* pNum = Sprite::createWithSpriteFrameName(pStr->getCString());
		pNum->setScale(0.8f);
		pNum->setAnchorPoint(Vec2(0.9f, 0.5f));
		pNumNode->addChild(pNum, 1);
		coinNum = coinNum/10;
		fWidth = (pNum->getContentSize().width - 20.0f) * pNum->getScale();
		pNum->setPosition(Point(-fWidth * nIndex, 0));
		nIndex++;
	}
	pNumNode->setPosition(Vec2(fHalf * fWidth, 0));
    this->addChild(pNumNode);
    
    Point randPoint = Point(0, 0);
    this->setPosition(point  + randPoint);
    return true;
}
void CoinPrompt::removeCoinPrompt()
{
    this->removeFromParentAndCleanup(true);
}

CoinPrompt::CoinPrompt():bg_sp(nullptr), fish_name(nullptr)
{

}
CoinPrompt::~CoinPrompt()
{

}