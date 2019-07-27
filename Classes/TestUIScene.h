#ifndef __UI_SCENE_H__
#define __UI_SCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/UIButton.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"


using namespace cocos2d;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace cocostudio;


class TestUIScene : public cocos2d::Layer
{
public:
	CREATE_FUNC(TestUIScene);
	virtual bool init();

	//单人模式
	void callBackSingle(Ref* pObject, ui::Widget::TouchEventType ty);
	//多人模式
	void callBackMany(Ref* pObject, ui::Widget::TouchEventType ty);

	cocos2d::ui::Widget* m_pUINode;
	Size size;
};

#endif