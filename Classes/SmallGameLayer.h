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

	//��ʾѺע����
	void showBetLayer();
	//��ʾ��ת����
	void showRotateLayer();
	//��ʾ�������
	void showResultLayer();
	
	//��ʼ
	void callBackBegin(Ref* pObject, ui::Widget::TouchEventType ty);
	//��ע
	void callBackBet(Ref* pObject, ui::Widget::TouchEventType ty);
	//�л�����
	void callBackSwitchRate(Ref* pObject, ui::Widget::TouchEventType ty);
	//�˳�
	void callBackExit(Ref* pObject, ui::Widget::TouchEventType ty);
	//����
	void callBackGoon(Ref* pObject, ui::Widget::TouchEventType ty);
	//�����±��ȡ�Ƕ�
	float getRotationByID(int nID);
	//����ID�����ø�����ť
	void setBrightByID(int nID);
	//ˢ�½��
	void refreshGold();
	//���λ��
	int randPosition();
	//ͨ��ID��ȡѺע�ı���
	int getBetRateByID(int nID);
	//ͨ����ǰת�ǻ�ȡID
	int getIDByCurRotation(float fRot);
	//ͨ��ID��Ӳ��Ŷ���
	void addAnimationByID(int nID);
	//��ȡһ����Ѻע
	int getTotalBetRate();
	//��ʾ�������
	void callBackShowResultLayer(Node* pNode);
	//��ȡ�Ƿ�Ѻע�˽��
	bool getIsBet();
	//�����
	void addFish();

private:
	Size size;
	//�Ƿ�������ת
	bool m_bRotating;
	ui::Widget* m_pUINode;
	//��ǰ�ı���
	int m_nCurrentRate;
	//��ǰ�����ID
	int m_nCurrentRandomID;
	//����ͼ��
	PublicLayer* m_pPublicLayer;
};
#endif