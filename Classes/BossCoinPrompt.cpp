#include "BossCoinPrompt.h"
#include "MyTools.h"


BossCoinPrompt::BossCoinPrompt()
{

}
BossCoinPrompt::~BossCoinPrompt()
{

}

BossCoinPrompt* BossCoinPrompt::createCoinPrompt(Point _point, int _coinNum)
{
    BossCoinPrompt* pRet = new BossCoinPrompt();
    if (pRet&&pRet->init(_point, _coinNum)) 
	{
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}
bool BossCoinPrompt::init(Point _point, int _coinNum)
{
	Node::init();

	this->addDiedAnimation();

	int nCoinNum = _coinNum;

    Sequence* seq = Sequence::create(ScaleTo::create(0.5f, 1.5f),
		ScaleTo::create(0.5f, 1.0f),
		DelayTime::create(1.5f), 
		CallFunc::create(CC_CALLBACK_0(BossCoinPrompt::removeBossCoinPrompt, this)), nullptr);
    this->runAction(seq);
    
	std::string strNum = getStringFromInt(nCoinNum);
	int nLength = strlen(strNum.c_str());

	float fHalf = 0;

	fHalf = nLength/2.0f;
	
	Node* pNumNode = Node::create();
	float fWidth = 0;
	int nIndex = 0;
	while (nCoinNum) 
	{
		__String* pStr = __String::createWithFormat("big_%d.png", nCoinNum % 10);
		Sprite* pNum = Sprite::createWithSpriteFrameName(pStr->getCString());
		pNum->setScale(0.8f);
		pNum->setAnchorPoint(Vec2(0.8f, 0.5f));
		pNumNode->addChild(pNum, 1);
		nCoinNum = nCoinNum/10;
		fWidth = (pNum->getContentSize().width - 20.0f) * pNum->getScale();
		pNum->setPosition(Point(-fWidth * nIndex + fHalf * fWidth, 0));
		nIndex++;
	}

	pNumNode->setPosition(Vec2(0, 0));
    this->addChild(pNumNode);
    pNumNode->setRotation(45);
	auto pSeq = Sequence::createWithTwoActions(RotateBy::create(0.5f, -90), RotateBy::create(0.5f, 90));
	pNumNode->runAction(RepeatForever::create(pSeq));

    Point randPoint = Point(20, 20);
    this->setPosition(_point);


    return true;
}
void BossCoinPrompt::removeBossCoinPrompt()
{
    this->removeFromParentAndCleanup(true);
}
void BossCoinPrompt::addDiedAnimation()
{
	//获取动画帧数和缓存中的标记
	Vector<SpriteFrame*> array1(4);

	for (int i=1; i <= 4; i++)
	{
		SpriteFrameCache *pcache = SpriteFrameCache::getInstance();
		std::string format = "color_plate_anim_%02d.png";
		__String* strName = __String::createWithFormat(format.c_str(), i);
		SpriteFrame* pframe = pcache->getSpriteFrameByName(strName->getCString());
		array1.pushBack(pframe);
	}
	//根据数组创建动画并添加到动画缓存
	Animation *panimation = Animation::createWithSpriteFrames(array1, 0.1f);

	Sprite* sp = Sprite::create();
	sp->setAnchorPoint(Vec2(0.5f, 0.5f));
	sp->setPosition(Vec2(0, 0));
	this->addChild(sp, 0);
	sp->setScale(0.75f);

	auto seq = Sequence::create(Repeat::create(Animate::create(panimation), 100), 
		NULL);
	sp->runAction(seq);
}