#include "LoadingLayer.h"
#include "Manager.h"
#include "MenuLayer.h"
LoadingLayer::LoadingLayer()
{

}
LoadingLayer::~LoadingLayer()
{

}
bool LoadingLayer::init()
{
	Layer::init();

	size = Director::getInstance()->getWinSize();
	
	LayerColor* pLayerColor = LayerColor::create(Color4B(0, 0, 0, 255));
	pLayerColor->setPosition(Vec2::ZERO);
	this->addChild(pLayerColor);

    addRandFish();


	XMLParser* xmlparser = XMLParser::parseWithFile("ui/loading.xml");
	int nID = rand() % 15 + 1;
	Manager::getInstance()->setTipID(nID);
	__String* strID = __String::createWithFormat("loading_tip_%d", nID);
	__String* info = xmlparser->getString(strID->getCString());
	Label* pLabel = Label::create(info->getCString(), "Arial", 18);
	pLabel->setAnchorPoint(Vec2(0.5, 0.5));
	pLabel->setPosition(Vec2(size.width / 2.0, 30.0f));
	this->addChild(pLabel);

	Label* pLabelLoading = Label::create(xmlparser->getString("loading")->getCString(), "Arial", 14);
	pLabelLoading->setAnchorPoint(Vec2(0.5, 0.5));
	pLabelLoading->setPosition(Vec2(size.width - pLabelLoading->getContentSize().width, 30.0f));
	this->addChild(pLabelLoading);
	pLabelLoading->setColor(Color3B(255, 255, 0));
    
    this->setTouchEnabled(true);
 
	auto seq = Sequence::create(DelayTime::create(0.01f), 
		CallFuncN::create(CC_CALLBACK_1(LoadingLayer::callBackBegin, this)), 
		NULL);

	this->runAction(seq);

	return true;
}
void LoadingLayer::addRandFish()
{
	int arr[6] = {78, 155, 156, 164, 274, 381};
	int arr1[6] = {6, 5, 9, 6, 8, 15};
	int nRand = rand() % 6;
	int nFrameNum = arr1[nRand];
	int nID = arr[nRand];
	Manager::getInstance()->setFishID(nRand);
	SpriteFrameCache *pcache = SpriteFrameCache::getInstance();
	std::string str = __String::createWithFormat("ui/main/fish_frame_%d.plist", nID)->getCString();
	pcache->addSpriteFramesWithFile(str);

	//获取动画帧数和缓存中的标记
	Vector<SpriteFrame*> array1(nFrameNum);

	for (int i=1; i <= nFrameNum; i++)
	{
		std::string format = getStringFromInt(nID) + "_%02d.png";
		__String* strName = __String::createWithFormat(format.c_str(), i);
		SpriteFrame* pframe = pcache->getSpriteFrameByName(strName->getCString());
		array1.pushBack(pframe);
	}
	Animation *panimation = Animation::createWithSpriteFrames(array1, 0.1f);

	Sprite* sp = Sprite::create();
	sp->setRotation(-90);
	sp->setPosition(Vec2(size.width - 60, 80));
	this->addChild(sp);

	sp->runAction(RepeatForever::create(Animate::create(panimation)));
}

void LoadingLayer::callBackBegin(Node* pNode)
{
    Manager::getInstance()->setEnterLoading(false);

	bool bEnterGame = Manager::getInstance()->getEnterGame();
	if(bEnterGame)
	{
		auto pScene = MainScene::scene();
		Director::getInstance()->replaceScene(pScene);
	}
	else
	{
		Manager::getInstance()->setEnterGame(true);
		auto pScene = Scene::create();
		pScene->addChild(MenuLayer::create());
		Director::getInstance()->replaceScene(pScene);
	}
}
bool LoadingLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
    return false;
}
void LoadingLayer::onTouchMoved(Touch *touch, Event *unused_event)
{

}
void LoadingLayer::onTouchEnded(Touch *touch, Event *unused_event)
{

}
void LoadingLayer::onTouchCancelled(Touch *touch, Event *unused_event)
{

}