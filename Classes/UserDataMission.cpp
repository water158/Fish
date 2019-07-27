#include "UserDataMission.h"


UserDataMission::UserDataMission()
{
}

UserDataMission::~UserDataMission()
{
}

UserDataMission* UserDataMission::create(TiXmlElement *elm)
{
	UserDataMission* data = new UserDataMission();

	if(data && data->loadData(elm))
	{
		data->autorelease();
		return data;
	}

	return NULL;
}


bool UserDataMission::loadData(TiXmlElement *elm)
{
    //属性
	this->setLevelNum(atoi(elm->Attribute("LevelNum")));
    this->setFishID(atoi(elm->Attribute("FishID")));
    this->setFishNum(atoi(elm->Attribute("FishNum")));
	this->setFinishNum(atoi(elm->Attribute("FinishNum")));
	this->setFishID1(atoi(elm->Attribute("FishID1")));
	this->setFishNum1(atoi(elm->Attribute("FishNum1")));
	this->setFinishNum1(atoi(elm->Attribute("FinishNum1")));
    this->setMissionSitate(atoi(elm->Attribute("MissionSitate")));
    this->setUnLock(strncmp(elm->Attribute("Unlock"), "1", 1) == 0 ? true : false);
    

	return true;
}