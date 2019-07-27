#ifndef _USERDATAMISSION_H_
#define _USERDATAMISSION_H_

#include "cocos2d.h"
#include "tinyxml.h"

USING_NS_CC;
using namespace std;

class UserDataMission : public Ref
{
public:
    UserDataMission(void);
	virtual ~UserDataMission(void);
    static UserDataMission* create(TiXmlElement *elm);

    //加载自身的数据
	bool loadData(TiXmlElement *elm);
    
private:
	
    CC_SYNTHESIZE (int, levelNum, LevelNum);
    CC_SYNTHESIZE (int, fishID, FishID);
    CC_SYNTHESIZE (int, fishNum, FishNum);
	CC_SYNTHESIZE (int, finishNum, FinishNum);
	CC_SYNTHESIZE (int, fishID1, FishID1);
	CC_SYNTHESIZE (int, fishNum1, FishNum1);
	CC_SYNTHESIZE (int, finishNum1, FinishNum1);
    CC_SYNTHESIZE (int, missionSitate, MissionSitate);
    CC_SYNTHESIZE(bool, unLock, UnLock);
};

#endif