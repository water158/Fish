#ifndef _LOGO_LAYER_H_
#define _LOGO_LAYER_H_

#include "cocos2d.h"
using namespace cocos2d;

class LogoLayer : public Layer
{
public:
	LogoLayer();
	virtual ~LogoLayer();
	CREATE_FUNC(LogoLayer);
	virtual bool init();

	void callBackBegin(Node* pNode);
};
#endif