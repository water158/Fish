#ifndef _GIFT_LAYER_H_
#define _GIFT_LAYER_H_

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/UIButton.h"
#include "cocostudio/CocoStudio.h"
#include "MyTools.h"
#include "PublicLayer.h"
#include "MyButton.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace cocostudio;
class GiftLayer : public Layer
{
public:
	GiftLayer();
	virtual ~GiftLayer();
	CREATE_FUNC(GiftLayer);
	virtual bool init();

	void callBackClose(Ref* pObject, ui::Widget::TouchEventType ty);
	void callBackBuy(Ref* pObject, ui::Widget::TouchEventType ty);
};
#endif