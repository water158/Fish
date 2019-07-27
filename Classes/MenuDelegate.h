#ifndef _MENU_DELEGATE_
#define _MENU_DELEGATE_

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class MenuDelegate
{
public:
	//点击多人关卡
	virtual void clickManyMission() = 0;

	//点击单人游戏
	virtual void clickSingleGame() = 0;
	//点击多人游戏
	virtual void clickManyGame() = 0;
};

#endif