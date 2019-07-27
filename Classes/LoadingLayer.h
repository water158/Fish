#ifndef _LOADING_LAYER_H_
#define _LOADING_LAYER_H_

#include "cocos2d.h"
#include "FishBase.h"
#include "MainScene.h"
#include "Data.h"
#include "MyTools.h"
#include "MyXmlParser.h"
using namespace cocos2d;

class LoadingLayer : public Layer
{
public:
	LoadingLayer();
	virtual ~LoadingLayer();
	CREATE_FUNC(LoadingLayer);
	virtual bool init();

	void callBackBegin(Node* pNode);

	void addRandFish();
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual void onTouchCancelled(Touch *touch, Event *unused_event);


	Size size;
};
#endif