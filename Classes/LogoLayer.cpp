#include "LogoLayer.h"
#include "Manager.h"
#include "MenuLayer.h"
#include "LoadingLayer.h"
LogoLayer::LogoLayer()
{

}
LogoLayer::~LogoLayer()
{

}
bool LogoLayer::init()
{
	Layer::init();

	Size size = Director::getInstance()->getWinSize();

	LayerColor* pLayerColor = LayerColor::create(Color4B(0, 0, 0, 255));
	pLayerColor->setPosition(Vec2::ZERO);
	this->addChild(pLayerColor);

	auto pBack = Sprite::create("buyulogo.jpg");
	pBack->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(pBack);

	auto seq = Sequence::create(DelayTime::create(1.5f), 
		CallFuncN::create(CC_CALLBACK_1(LogoLayer::callBackBegin, this)), 
		NULL);

	this->runAction(seq);

	return true;
}

void LogoLayer::callBackBegin(Node* pNode)
{
	auto pScene = Scene::create();
	pScene->addChild(LoadingLayer::create());
	Director::getInstance()->replaceScene(pScene);
}