#ifndef __BossCoinPrompt__
#define __BossCoinPrompt__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

class BossCoinPrompt : public Node
{
public:
	BossCoinPrompt();
	virtual ~BossCoinPrompt();

    static BossCoinPrompt* createCoinPrompt(Point _point, int _coinNum);

    bool init(Point _point, int _coinNum);

    void removeBossCoinPrompt();

	void addDiedAnimation();

};
#endif
