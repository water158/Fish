#ifndef _SHILED_LAYER_H_
#define _SHILED_LAYER_H_

#include "cocos2d.h"
#include "FishBase.h"
#include "Data.h"
#include "MyTools.h"
#include "MyXmlParser.h"
using namespace cocos2d;

class ShiledLayer : public Layer
{
public:
	ShiledLayer();
	virtual ~ShiledLayer();
	CREATE_FUNC(ShiledLayer);
	virtual bool init();

	void callBackBegin(Node* pNode);

	void addRandFish();
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
  
	Size size;
};
#endif