#ifndef _SMALLGAME_LAYER_H_
#define _SMALLGAME_LAYER_H_

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/UIButton.h"
#include "cocostudio/CocoStudio.h"
#include "MyButton.h"
#include "ui/CocosGUI.h"
#include "UserData.h"
#include "MyTools.h"
#include "PublicLayer.h"

using namespace cocos2d;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace cocostudio;

class SmallGameLayer : public Layer
{
public:
	SmallGameLayer();
	virtual ~SmallGameLayer();
	CREATE_FUNC(SmallGameLayer);
	virtual bool init();

	virtual void update(float dt);

	//显示押注界面
	void showBetLayer();
	//显示旋转界面
	void showRotateLayer();
	//显示结果界面
	void showResultLayer();
	
	//开始
	void callBackBegin(Ref* pObject, ui::Widget::TouchEventType ty);
	//下注
	void callBackBet(Ref* pObject, ui::Widget::TouchEventType ty);
	//切换倍率
	void callBackSwitchRate(Ref* pObject, ui::Widget::TouchEventType ty);
	//退出
	void callBackExit(Ref* pObject, ui::Widget::TouchEventType ty);
	//继续
	void callBackGoon(Ref* pObject, ui::Widget::TouchEventType ty);
	//根据下标获取角度
	float getRotationByID(int nID);
	//根据ID来设置高亮按钮
	void setBrightByID(int nID);
	//刷新金币
	void refreshGold();
	//随机位置
	int randPosition();
	//通过ID获取押注的倍率
	int getBetRateByID(int nID);
	//通过当前转角获取ID
	int getIDByCurRotation(float fRot);
	//通过ID添加播放动画
	void addAnimationByID(int nID);
	//获取一共得押注
	int getTotalBetRate();
	//显示结果界面
	void callBackShowResultLayer(Node* pNode);
	//获取是否押注了金币
	bool getIsBet();
	//添加鱼
	void addFish();

private:
	Size size;
	//是否正在旋转
	bool m_bRotating;
	ui::Widget* m_pUINode;
	//当前的倍率
	int m_nCurrentRate;
	//当前随机的ID
	int m_nCurrentRandomID;
	//公共图层
	PublicLayer* m_pPublicLayer;
};
#endif