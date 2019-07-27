#include "TestUIScene.h"

bool TestUIScene::init()
{
	Layer::init();

	size = Director::getInstance()->getWinSize();

	m_pUINode = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui/main/NewUi_1.json");
	m_pUINode->setPosition(Vec2(0, 0));
	this->addChild(m_pUINode, 1); 

	//Sprite* pSprite = Sprite::createWithSpriteFrameName("t_icon_2.png");
	
	//单人模式按钮
	ui::Button* buttonSingle = (ui::Button *)ui::Helper::seekWidgetByTag(m_pUINode, 7);
	buttonSingle->addTouchEventListener(CC_CALLBACK_2(TestUIScene::callBackSingle, this));
	//0//buttonSingle->setClickPicture();
	//多人模式按钮
	ui::Button* buttonMany = (ui::Button *)ui::Helper::seekWidgetByTag(m_pUINode, 8);
	buttonMany->addTouchEventListener(CC_CALLBACK_2(TestUIScene::callBackMany, this));


	Layout* pMain = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 4);
	pMain->setVisible(true);
	pMain->setTouchEnabled(true);
	Layout* pList1 = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 26);
	pList1->setVisible(false);
	pList1->setTouchEnabled(false);
	pList1->runAction(MoveBy::create(0.01, Vec2(-854, 0)));
	Layout* pMany = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 111);
	pMany->setVisible(false);
	pMany->setTouchEnabled(false);
	pMany->runAction(MoveBy::create(0.01, Vec2(854, 0)));

	return true;
}

//单人模式
void TestUIScene::callBackSingle(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		Layout* pList1 = (ui::Layout *)ui::Helper::seekWidgetByTag(m_pUINode, 26);
		pList1->setVisible(true);

		auto seq = Sequence::createWithTwoActions(MoveBy::create(0.3, Vec2(size.width + 40, 0)), MoveBy::create(0.1, Vec2(-40, 0)));
		m_pUINode->runAction(seq);

		ui::ScrollView * pScrollView = (ui::ScrollView *)ui::Helper::seekWidgetByTag(m_pUINode, 165);
		pScrollView->setInnerContainerSize(Size(1800, 200));
	}
}
//多人模式
void TestUIScene::callBackMany(Ref* pObject, ui::Widget::TouchEventType ty)
{
	if(ty == ui::Widget::TouchEventType::ENDED)
	{
		
		Layout* pMany = (ui::Layout*)ui::Helper::seekWidgetByTag(m_pUINode, 111);
		pMany->setVisible(true);

		auto seq = Sequence::createWithTwoActions(MoveBy::create(0.3, Vec2(-size.width - 40, 0)), MoveBy::create(0.1, Vec2(40, 0)));
		m_pUINode->runAction(seq);
		ui::ScrollView * pScrollView = (ui::ScrollView *)ui::Helper::seekWidgetByTag(m_pUINode, 147);
		pScrollView->setInnerContainerSize(Size(1000, 200));
	}
}