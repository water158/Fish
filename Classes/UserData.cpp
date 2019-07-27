#include "UserData.h"

UserData::UserData()
{
}

UserData::~UserData()
{
}

UserData* UserData::s_Instance = NULL;

UserData* UserData::getInstance(void)
{
    if(!s_Instance)
    {
        s_Instance = new UserData();
        s_Instance->init();
    }

    return s_Instance;
}

void UserData::init()
{
    this->setCannon1If(0);
    this->setSelectedMissionID(0);
	//this->setGoldNum(1);
	m_nUID = 1000;
	m_nCurrentTelephoneFare = 0;
	m_nBuyGift = 0;
	m_nFirstShopping = 0;
	m_nOverGoldPeak = 0;
	m_nSubsidiesTime = 0;
	m_nSubsidiesIndex = 0;
	//m_nDiamonds = 0;
	m_nVipLevel = 0;
	m_nRechargeAmount = 0;
	m_nSkillNum1 = 0;
	m_nSkillNum2 = 0;
	m_nSkillNum3 = 0;
	m_nSkillNum4 = 0;
	m_nSkillNum5 = 0;
	m_nFirstEnterGame = 0;

	checkVersion();
}

void UserData::check()
{
    m_sUserDataPath = FileUtils::getInstance()->getWritablePath() + "userdata_out.xml";
    //log("m_sUserDataPath = %s", m_sUserDataPath.c_str());
    bool exsit = isFileExist(m_sUserDataPath.c_str());

    if(!exsit)
    {
        copyData("userdata.xml", "userdata_out.xml");
    }
}

void UserData::checkVersion()
{
    check();

	loadPackage();
	//获取手机里的存储版本号
	load();

	//获取新包里的版本号
	Data data = FileUtils::getInstance()->getDataFromFile("userdata.xml");
	TiXmlDocument *xmlDoc = new TiXmlDocument();
	xmlDoc->LoadMemory((const char*)data.getBytes(), data.getSize());
    //获取根元素
    TiXmlElement *elmRoot = xmlDoc->RootElement();
    int nNewV = atoi(elmRoot->Attribute("version"));
    delete xmlDoc;

	//在这里检测版本号
	if(nNewV > this->getDataVersion())
	{
		copyData("userdata.xml", "userdata_out.xml");
		this->setDataVersion(nNewV);
		save();
	}
}

void UserData::load()
{
    check();

	Data data = FileUtils::getInstance()->getDataFromFile(m_sUserDataPath.c_str());

	TiXmlDocument *xmlDoc = new TiXmlDocument();
	xmlDoc->LoadMemory((const char*)data.getBytes(), data.getSize());

    //获得数据的根元素
    TiXmlElement *elmRoot = xmlDoc->RootElement();

	//版本号
	this->setDataVersion(atoi(elmRoot->Attribute("version")));

    //UserData
    TiXmlElement *elmUserDate = elmRoot->FirstChildElement();

    //UserLevel 金币
    TiXmlElement *elmCursor = elmUserDate->FirstChildElement();
	//this->setGoldNum(atoi(elmCursor->FirstChild()->Value()));
	m_pGoldNum = elmCursor->FirstChild()->Value();
	int nGoldNum = this->getGoldNum();
	
	//炮 解锁情况
	elmCursor = elmCursor->NextSiblingElement();
	this->setCannon1If(atoi(elmCursor->FirstChild()->Value()));
	this->setCannon1Energy(atoi(elmCursor->Attribute("lv")));
    elmCursor = elmCursor->NextSiblingElement();
	this->setCannon2If(atoi(elmCursor->FirstChild()->Value()));
	this->setCannon2Energy(atoi(elmCursor->Attribute("lv")));
    elmCursor = elmCursor->NextSiblingElement();
	this->setCannon3If(atoi(elmCursor->FirstChild()->Value()));
	this->setCannon3Energy(atoi(elmCursor->Attribute("lv")));
    elmCursor = elmCursor->NextSiblingElement();
	this->setCannon4If(atoi(elmCursor->FirstChild()->Value()));
	this->setCannon4Energy(atoi(elmCursor->Attribute("lv")));
    elmCursor = elmCursor->NextSiblingElement();
	this->setCannon5If(atoi(elmCursor->FirstChild()->Value()));
	this->setCannon5Energy(atoi(elmCursor->Attribute("lv")));
    ////当前装备炮的ID
	elmCursor = elmCursor->NextSiblingElement();
	this->setCannonID(atoi(elmCursor->FirstChild()->Value()));

	//签到的天数
	elmCursor = elmCursor->NextSiblingElement();
	this->setSignInNum(atoi(elmCursor->FirstChild()->Value()));
    //上次签到的时间
	elmCursor = elmCursor->NextSiblingElement();
	this->setSignInLastTime(atoi(elmCursor->FirstChild()->Value()));
	//每日领取的进度下标
	elmCursor = elmCursor->NextSiblingElement();
	this->setAwardIndex(atoi(elmCursor->FirstChild()->Value()));
    //每日领取的上次签到时间
	elmCursor = elmCursor->NextSiblingElement();
	this->setAwardLastTime(atoi(elmCursor->FirstChild()->Value()));
    //经验值
    elmCursor = elmCursor->NextSiblingElement();
	this->setExp_value(atoi(elmCursor->FirstChild()->Value()));
	//话费
	elmCursor = elmCursor->NextSiblingElement();
	this->setCurrentTelephoneFare(atoi(elmCursor->FirstChild()->Value()));
	//用户ID
	elmCursor = elmCursor->NextSiblingElement();
	this->setPlayerID(atoi(elmCursor->FirstChild()->Value()));
	//是否购买礼包
	elmCursor = elmCursor->NextSiblingElement();
	this->setBuyGift(atoi(elmCursor->FirstChild()->Value()));
	//首冲
	elmCursor = elmCursor->NextSiblingElement();
	this->setFirstShopping(atoi(elmCursor->FirstChild()->Value()));
	//金币峰值
	elmCursor = elmCursor->NextSiblingElement();
	this->setOverGoldPeak(atoi(elmCursor->FirstChild()->Value()));
	//补贴时间
	elmCursor = elmCursor->NextSiblingElement();
	this->setSubsidiesTime(atoi(elmCursor->FirstChild()->Value()));
	//补贴下标
	elmCursor = elmCursor->NextSiblingElement();
	this->setSubsidiesIndex(atoi(elmCursor->FirstChild()->Value()));

	
	//钻石
	elmCursor = elmCursor->NextSiblingElement();
	//this->setDiamonds(atoi(elmCursor->FirstChild()->Value()));
	m_pDiamonds = elmCursor->FirstChild()->Value();
	//充值金额
	elmCursor = elmCursor->NextSiblingElement();
	this->setRechargeAmount(atoi(elmCursor->FirstChild()->Value()));
	//技能次数
	elmCursor = elmCursor->NextSiblingElement();
	this->setSkillNum1(atoi(elmCursor->FirstChild()->Value()));
	elmCursor = elmCursor->NextSiblingElement();
	this->setSkillNum2(atoi(elmCursor->FirstChild()->Value()));
	elmCursor = elmCursor->NextSiblingElement();
	this->setSkillNum3(atoi(elmCursor->FirstChild()->Value()));
	elmCursor = elmCursor->NextSiblingElement();
	this->setSkillNum4(atoi(elmCursor->FirstChild()->Value()));
	elmCursor = elmCursor->NextSiblingElement();
	this->setSkillNum5(atoi(elmCursor->FirstChild()->Value()));
	//是否购买月卡
	elmCursor = elmCursor->NextSiblingElement();
	this->setBuyMonthCard(atoi(elmCursor->FirstChild()->Value()));
	//上次领取月卡奖励的时间
	elmCursor = elmCursor->NextSiblingElement();
	this->setGetMonthCardLastTime(atoi(elmCursor->FirstChild()->Value()));
	//领取月卡的次数
	elmCursor = elmCursor->NextSiblingElement();
	this->setNumOfReceivMonthCard(atoi(elmCursor->FirstChild()->Value()));
	//首次进入游戏
	elmCursor = elmCursor->NextSiblingElement();
	this->setFirstEnterGame(atoi(elmCursor->FirstChild()->Value()));
	


    //RoomOne
    TiXmlElement *elmUserRoomOne = elmUserDate->NextSiblingElement();
    elmCursor = elmUserRoomOne->FirstChildElement();
    while(elmCursor)
    {
        UserDataMission* data1 = UserDataMission::create(elmCursor);
        
        //加入到map中
        user_missions.insert(data1->getLevelNum(), data1);

        //下一个
        elmCursor = elmCursor->NextSiblingElement();
    }
    
	//游戏设置UserSetting
	 TiXmlElement *elmSettingMission = elmUserRoomOne->NextSiblingElement();
    elmCursor = elmSettingMission->FirstChildElement();
    this->setSound(atoi(elmCursor->FirstChild()->Value()));
    elmCursor = elmCursor->NextSiblingElement();
    this->setMusic(atoi(elmCursor->FirstChild()->Value()));
	elmCursor = elmCursor->NextSiblingElement();
	this->setSmooth(atoi(elmCursor->FirstChild()->Value()));
	//log("getSound = %d, getMusic = %d", this->getSound(), this->getMusic());

    delete xmlDoc;
}

void UserData::save()
{
    check();

	Data data = FileUtils::getInstance()->getDataFromFile(m_sUserDataPath.c_str());

	TiXmlDocument *xmlDoc = new TiXmlDocument();
	xmlDoc->LoadMemory((const char*)data.getBytes(), data.getSize());

    //获得数据的根元素
    TiXmlElement *elmRoot = xmlDoc->RootElement();
	//userdata版本
    elmRoot->SetAttribute("version", this->getDataVersion());

    //UserData
    TiXmlElement *elmUserData = elmRoot->FirstChildElement();

    //UserLevel 金币
    TiXmlElement *elmCursor = elmUserData->FirstChildElement();
    //elmCursor->FirstChild()->SetValue(getStringFromInt(this->getGoldNum()).c_str());
	elmCursor->FirstChild()->SetValue(m_pGoldNum.c_str());
	//炮的解锁状态
	elmCursor = elmCursor->NextSiblingElement();
	elmCursor->FirstChild()->SetValue(getStringFromInt(this->getCannon1If()).c_str());
	elmCursor->SetAttribute("lv", this->getCannon1Energy());
	elmCursor = elmCursor->NextSiblingElement();
	elmCursor->FirstChild()->SetValue(getStringFromInt(this->getCannon2If()).c_str());
	elmCursor->SetAttribute("lv", this->getCannon2Energy());
	elmCursor = elmCursor->NextSiblingElement();
	elmCursor->FirstChild()->SetValue(getStringFromInt(this->getCannon3If()).c_str());
	elmCursor->SetAttribute("lv", this->getCannon3Energy());
	elmCursor = elmCursor->NextSiblingElement();
	elmCursor->FirstChild()->SetValue(getStringFromInt(this->getCannon4If()).c_str());
	elmCursor->SetAttribute("lv", this->getCannon4Energy());
	elmCursor = elmCursor->NextSiblingElement();
	elmCursor->FirstChild()->SetValue(getStringFromInt(this->getCannon5If()).c_str());
	elmCursor->SetAttribute("lv", this->getCannon5Energy());
    //当前装备炮的ID
    elmCursor = elmCursor->NextSiblingElement();
	elmCursor->FirstChild()->SetValue(getStringFromInt(this->getCannonID()).c_str());
    //签到的天数
    elmCursor = elmCursor->NextSiblingElement();
	elmCursor->FirstChild()->SetValue(getStringFromInt(this->getSignInNum()).c_str());
    //上次签到的时间
    elmCursor = elmCursor->NextSiblingElement();
	elmCursor->FirstChild()->SetValue(getStringFromInt(this->getSignInLastTime()).c_str());
	//每日领取的进度下标
	elmCursor = elmCursor->NextSiblingElement();
	elmCursor->FirstChild()->SetValue(getStringFromInt(this->getAwardIndex()).c_str());
    //每日领取的上次签到时间
    elmCursor = elmCursor->NextSiblingElement();
	elmCursor->FirstChild()->SetValue(getStringFromInt(this->getAwardLastTime()).c_str());
	//经验值
    elmCursor = elmCursor->NextSiblingElement();
	elmCursor->FirstChild()->SetValue(getStringFromInt(this->getExp_value()).c_str());
	//话费
	elmCursor = elmCursor->NextSiblingElement();
	elmCursor->FirstChild()->SetValue(getStringFromInt(this->getCurrentTelephoneFare()).c_str());
	//用户ID
	elmCursor = elmCursor->NextSiblingElement();
	elmCursor->FirstChild()->SetValue(getStringFromInt(this->getPlayerID()).c_str());
	//是否购买礼包
	elmCursor = elmCursor->NextSiblingElement();
	elmCursor->FirstChild()->SetValue(getStringFromInt(this->getBuyGift()).c_str());
	//首冲
	elmCursor = elmCursor->NextSiblingElement();
	elmCursor->FirstChild()->SetValue(getStringFromInt(this->getFirstShopping()).c_str());
	//金币峰值
	elmCursor = elmCursor->NextSiblingElement();
	elmCursor->FirstChild()->SetValue(getStringFromInt(this->getOverGoldPeak()).c_str());
	//补贴时间
	elmCursor = elmCursor->NextSiblingElement();
	elmCursor->FirstChild()->SetValue(getStringFromInt(this->getSubsidiesTime()).c_str());
	//补贴下标
	elmCursor = elmCursor->NextSiblingElement();
	elmCursor->FirstChild()->SetValue(getStringFromInt(this->getSubsidiesIndex()).c_str());
	//钻石
	elmCursor = elmCursor->NextSiblingElement();
	//elmCursor->FirstChild()->SetValue(getStringFromInt(this->getDiamonds()).c_str());
	elmCursor->FirstChild()->SetValue(m_pDiamonds.c_str());
	//充值金额
	elmCursor = elmCursor->NextSiblingElement();
	elmCursor->FirstChild()->SetValue(getStringFromInt(this->getRechargeAmount()).c_str());
	//技能使用次数
	elmCursor = elmCursor->NextSiblingElement();
	elmCursor->FirstChild()->SetValue(getStringFromInt(this->getSkillNum1()).c_str());
	elmCursor = elmCursor->NextSiblingElement();
	elmCursor->FirstChild()->SetValue(getStringFromInt(this->getSkillNum2()).c_str());
	elmCursor = elmCursor->NextSiblingElement();
	elmCursor->FirstChild()->SetValue(getStringFromInt(this->getSkillNum3()).c_str());
	elmCursor = elmCursor->NextSiblingElement();
	elmCursor->FirstChild()->SetValue(getStringFromInt(this->getSkillNum4()).c_str());
	elmCursor = elmCursor->NextSiblingElement();
	elmCursor->FirstChild()->SetValue(getStringFromInt(this->getSkillNum5()).c_str());
	//是否购买月卡
	elmCursor = elmCursor->NextSiblingElement();
	elmCursor->FirstChild()->SetValue(getStringFromInt(this->getBuyMonthCard()).c_str());
	//上次领取月卡奖励的时间
	elmCursor = elmCursor->NextSiblingElement();
	elmCursor->FirstChild()->SetValue(getStringFromInt(this->getGetMonthCardLastTime()).c_str());
	//领取月卡的次数
	elmCursor = elmCursor->NextSiblingElement();
	elmCursor->FirstChild()->SetValue(getStringFromInt(this->getNumOfReceivMonthCard()).c_str());
	//首次进入游戏
	elmCursor = elmCursor->NextSiblingElement();
	elmCursor->FirstChild()->SetValue(getStringFromInt(this->getFirstEnterGame()).c_str());


    //RoomOne 第一房间关卡信息
    TiXmlElement *elmUserRoomOne = elmUserData->NextSiblingElement();
    elmCursor = elmUserRoomOne->FirstChildElement();
    while(elmCursor)
    {
        //获得数据
        int id = atoi(elmCursor->Attribute("LevelNum"));
        UserDataMission* data1 = getUserDataMissionByID(id);

        if(data1)
        {
            elmCursor->SetAttribute("FishID", data1->getFishID());
            elmCursor->SetAttribute("FishNum", data1->getFishNum());
			elmCursor->SetAttribute("FinishNum", data1->getFinishNum());
			elmCursor->SetAttribute("FishID1", data1->getFishID1());
			elmCursor->SetAttribute("FishNum1", data1->getFishNum1());
			elmCursor->SetAttribute("FinishNum1", data1->getFinishNum1());
            elmCursor->SetAttribute("MissionSitate", data1->getMissionSitate());
            elmCursor->SetAttribute("Unlock", data1->getUnLock() ? 1 : 0);
        }

        //下一个
        elmCursor = elmCursor->NextSiblingElement();
    }
    
    //UserStatus
    TiXmlElement *elmUserStatus = elmUserRoomOne->NextSiblingElement();
    elmCursor = elmUserStatus->FirstChildElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getSound()));
    elmCursor = elmCursor->NextSiblingElement();
    elmCursor->FirstChild()->SetValue(getStringFromInt(this->getMusic()));
	elmCursor = elmCursor->NextSiblingElement();
	elmCursor->FirstChild()->SetValue(getStringFromInt(this->getSmooth()));
   	//log("=======getSound = %d, getMusic = %d=======", this->getSound(), this->getMusic());


    xmlDoc->SaveFile(m_sUserDataPath);
	
    delete xmlDoc;
}

//加载包里的数据
void UserData::loadPackage()
{
	Data data = FileUtils::getInstance()->getDataFromFile("userdata.xml");

	TiXmlDocument *xmlDoc = new TiXmlDocument();
	xmlDoc->LoadMemory((const char*)data.getBytes(), data.getSize());

	//获得数据的根元素
	TiXmlElement *elmRoot = xmlDoc->RootElement();

	//版本号
	this->setDataVersion(atoi(elmRoot->Attribute("version")));

	//UserData
	TiXmlElement *elmUserDate = elmRoot->FirstChildElement();

	//UserLevel 金币
	TiXmlElement *elmCursor = elmUserDate->FirstChildElement();
	this->setGoldNum(atoi(elmCursor->FirstChild()->Value()));
	//炮 解锁情况
	elmCursor = elmCursor->NextSiblingElement();
	this->setCannon1If(atoi(elmCursor->FirstChild()->Value()));
	this->setCannon1Energy(atoi(elmCursor->Attribute("lv")));
	elmCursor = elmCursor->NextSiblingElement();
	this->setCannon2If(atoi(elmCursor->FirstChild()->Value()));
	this->setCannon2Energy(atoi(elmCursor->Attribute("lv")));
	elmCursor = elmCursor->NextSiblingElement();
	this->setCannon3If(atoi(elmCursor->FirstChild()->Value()));
	this->setCannon3Energy(atoi(elmCursor->Attribute("lv")));
	elmCursor = elmCursor->NextSiblingElement();
	this->setCannon4If(atoi(elmCursor->FirstChild()->Value()));
	this->setCannon4Energy(atoi(elmCursor->Attribute("lv")));
	elmCursor = elmCursor->NextSiblingElement();
	this->setCannon5If(atoi(elmCursor->FirstChild()->Value()));
	this->setCannon5Energy(atoi(elmCursor->Attribute("lv")));
	////当前装备炮的ID
	elmCursor = elmCursor->NextSiblingElement();
	this->setCannonID(atoi(elmCursor->FirstChild()->Value()));

	//签到的天数
	elmCursor = elmCursor->NextSiblingElement();
	this->setSignInNum(atoi(elmCursor->FirstChild()->Value()));
	//上次签到的时间
	elmCursor = elmCursor->NextSiblingElement();
	this->setSignInLastTime(atoi(elmCursor->FirstChild()->Value()));
	//每日领取的进度下标
	elmCursor = elmCursor->NextSiblingElement();
	this->setAwardIndex(atoi(elmCursor->FirstChild()->Value()));
	//每日领取的上次签到时间
	elmCursor = elmCursor->NextSiblingElement();
	this->setAwardLastTime(atoi(elmCursor->FirstChild()->Value()));
	//经验值
	elmCursor = elmCursor->NextSiblingElement();
	this->setExp_value(atoi(elmCursor->FirstChild()->Value()));
	//话费
	elmCursor = elmCursor->NextSiblingElement();
	this->setCurrentTelephoneFare(atoi(elmCursor->FirstChild()->Value()));
	//用户ID
	elmCursor = elmCursor->NextSiblingElement();
	this->setPlayerID(atoi(elmCursor->FirstChild()->Value()));
	//是否购买礼包
	elmCursor = elmCursor->NextSiblingElement();
	this->setBuyGift(atoi(elmCursor->FirstChild()->Value()));
	//首冲
	elmCursor = elmCursor->NextSiblingElement();
	this->setFirstShopping(atoi(elmCursor->FirstChild()->Value()));
	//金币峰值
	elmCursor = elmCursor->NextSiblingElement();
	this->setOverGoldPeak(atoi(elmCursor->FirstChild()->Value()));
	//补贴时间
	elmCursor = elmCursor->NextSiblingElement();
	this->setSubsidiesTime(atoi(elmCursor->FirstChild()->Value()));
	//补贴下标
	elmCursor = elmCursor->NextSiblingElement();
	this->setSubsidiesIndex(atoi(elmCursor->FirstChild()->Value()));
	//钻石
	elmCursor = elmCursor->NextSiblingElement();
	this->setDiamonds(atoi(elmCursor->FirstChild()->Value()));//充值金额
	elmCursor = elmCursor->NextSiblingElement();
	this->setRechargeAmount(atoi(elmCursor->FirstChild()->Value()));
	//技能次数
	elmCursor = elmCursor->NextSiblingElement();
	this->setSkillNum1(atoi(elmCursor->FirstChild()->Value()));
	elmCursor = elmCursor->NextSiblingElement();
	this->setSkillNum2(atoi(elmCursor->FirstChild()->Value()));
	elmCursor = elmCursor->NextSiblingElement();
	this->setSkillNum3(atoi(elmCursor->FirstChild()->Value()));
	elmCursor = elmCursor->NextSiblingElement();
	this->setSkillNum4(atoi(elmCursor->FirstChild()->Value()));
	elmCursor = elmCursor->NextSiblingElement();
	this->setSkillNum5(atoi(elmCursor->FirstChild()->Value()));
	//是否购买月卡
	elmCursor = elmCursor->NextSiblingElement();
	this->setBuyMonthCard(atoi(elmCursor->FirstChild()->Value()));
	//上次领取月卡奖励的时间
	elmCursor = elmCursor->NextSiblingElement();
	this->setGetMonthCardLastTime(atoi(elmCursor->FirstChild()->Value()));
	//领取月卡的次数
	elmCursor = elmCursor->NextSiblingElement();
	this->setNumOfReceivMonthCard(atoi(elmCursor->FirstChild()->Value()));

	//RoomOne
	TiXmlElement *elmUserRoomOne = elmUserDate->NextSiblingElement();
	elmCursor = elmUserRoomOne->FirstChildElement();
	while(elmCursor)
	{
		UserDataMission* data1 = UserDataMission::create(elmCursor);

		//加入到map中
		user_missions.insert(data1->getLevelNum(), data1);

		//下一个
		elmCursor = elmCursor->NextSiblingElement();
	}

	//游戏设置UserSetting
	TiXmlElement *elmSettingMission = elmUserRoomOne->NextSiblingElement();
	elmCursor = elmSettingMission->FirstChildElement();
	this->setSound(atoi(elmCursor->FirstChild()->Value()));
	elmCursor = elmCursor->NextSiblingElement();
	this->setMusic(atoi(elmCursor->FirstChild()->Value()));
	elmCursor = elmCursor->NextSiblingElement();
	this->setSmooth(atoi(elmCursor->FirstChild()->Value()));
	//log("getSound = %d, getMusic = %d", this->getSound(), this->getMusic());

	delete xmlDoc;
}
//钱是否够
bool UserData::isGoldEnough(int gold)
{
    if(this->getGoldNum() >= gold)
    {
        return true;
    }
    else
    {
        return false;
    }
}
//增加钱
void UserData::increaseGoldNum(int gold)
{
    //this->goldnum += gold;
	this->setGoldNum(this->getGoldNum() + gold);
}
//减少钱
void UserData::decreaseGoldNum(int gold)
{
    //this->goldnum -= gold;
	this->setGoldNum(this->getGoldNum() - gold);
}

//获得当前关卡的是否解锁，分数和等级
UserDataMission* UserData::getUserDataMissionByID(int id)
{
	if(id >= 10)
	{
		id = 10;
	}

    return user_missions.at(id);
}
//根据ID，设定关卡数据
void UserData::setUserDataMissionByID(int id, UserDataMission *&data)
{
	if(id >= 10)
	{
		id = 10;
	}

    UserDataMission* targetData = getUserDataMissionByID(id);

    if(targetData)
    {
        targetData->setLevelNum(data->getLevelNum());
        targetData->setFishID(data->getFishID());
        targetData->setFishNum(data->getFishNum());
		targetData->setFinishNum(data->getFinishNum());
		targetData->setFishID1(data->getFishID1());
		targetData->setFishNum1(data->getFishNum1());
		targetData->setFinishNum1(data->getFinishNum1());
        targetData->setMissionSitate(data->getMissionSitate());
        targetData->setUnLock(data->getUnLock());
    }
}
//下一关
void UserData::replaceNextMission()
{
    if(m_nSelectedMissionID < 10 - 1)
    {
        m_nSelectedMissionID ++;
    }
}
//获取当前解锁到第几关
int UserData::getCurrentMaxEnableStage()
{
	int i = 1;
	for ( ; i <= 10; i++)
	{
		UserDataMission* data = user_missions.at(i);

		bool enable = data->getUnLock();

		if(!enable)
		{
			return i-1;
		}
	}

	return 10;
}
//根据炮的ID设置威力
void UserData::setCannonEnergyByID(int id, int lv)
{
	if(id == 0)
		m_nCannon1Energy = lv;
	else if(id == 1)
		m_nCannon2Energy = lv;
	else if(id == 2)
		m_nCannon3Energy = lv;
	else if(id == 3)
		m_nCannon4Energy = lv;
	else if(id == 4)
		m_nCannon5Energy = lv;
}
//根据炮的ID获取威力
int UserData::getCannonEnergyByID(int id)
{
	int nEnergy = 1;

	if(id == 0)
		nEnergy = m_nCannon1Energy;
	else if(id == 1)
		nEnergy = m_nCannon2Energy;
	else if(id == 2)
		nEnergy = m_nCannon3Energy;
	else if(id == 3)
		nEnergy = m_nCannon4Energy;
	else if(id == 4)
		nEnergy = m_nCannon5Energy;

	return nEnergy;
}

//增加、减少道具数量
void UserData::increaseSkillNum(int id, int num)
{
	switch (id)
	{
	case 0:
		m_nSkillNum1 += num;
		break;
	case 1:
		m_nSkillNum2 += num;
		break;
	case 2:
		m_nSkillNum3 += num;
		break;
	case 3:
		m_nSkillNum4 += num;
		break;
	case 4:
		m_nSkillNum5 += num;
		break;
	default:
		break;
	}
}
void UserData::decreaseSkillNum(int id, int num)
{
	switch (id)
	{
	case SKILL_1:
		m_nSkillNum1 -= num;
		break;
	case SKILL_2:
		m_nSkillNum2 -= num;
		break;
	case SKILL_3:
		m_nSkillNum3 -= num;
		break;
	case SKILL_4:
		m_nSkillNum4 -= num;
		break;
	default:
		break;
	}
}

//增加、减少钻石数量
void UserData::increaseDiamonds(int num)
{
	//m_nDiamonds += num;
	this->setDiamonds(this->getDiamonds() + num);
}
void UserData::decreaseDiamonds(int num)
{
	//m_nDiamonds -= num;
	this->setDiamonds(this->getDiamonds() - num);
}

//获取VIP等级
int UserData::getCurVipLevel()
{
	m_nVipLevel = 0;
	if(m_nRechargeAmount < 10)
		m_nVipLevel = 0;
	else if(m_nRechargeAmount >= 10 && m_nRechargeAmount < 100)
		m_nVipLevel = 1;
	else if(m_nRechargeAmount >= 100 && m_nRechargeAmount < 500)
		m_nVipLevel = 2;
	else
		m_nVipLevel = 3;

	return m_nVipLevel;
}
//获取下一等级需要的充值
int UserData::getNextVIPLevelAmount()
{
	int nAmount = 0;
	int nCurVIP = this->getCurVipLevel();
	if(nCurVIP == 0)
		nAmount = 10;
	else if(nCurVIP == 1)
		nAmount = 100;
	else if(nCurVIP == 2)
		nAmount = 500;
	else
		nAmount = 0;

	return nAmount;
}
//获取当前等级需要的充值
int UserData::getCurVIPLevelAmount()
{
	int nAmount = 0;
	int nCurVIP = this->getCurVipLevel();
	if(nCurVIP == 0)
		nAmount = 0;
	else if(nCurVIP == 1)
		nAmount = 10;
	else if(nCurVIP == 2)
		nAmount = 100;
	else
		nAmount = 500;

	return nAmount;
}

unsigned int UserData::getUID()
{
	return ++m_nUID;
}

const char *m_strKey = "34cn3a7IdxKYv2Z9";
int UserData::getGoldNum()
{
	//获取的时候解密
	int temp = switchStringToInt(m_pGoldNum);

	return temp;
}
void UserData::setGoldNum(int num)
{
	m_pGoldNum = switchIntToString(num);
}

int UserData::getDiamonds()
{
	//获取的时候解密
	int temp = switchStringToInt(m_pDiamonds);

	return temp;
}
void UserData::setDiamonds(int num)
{
	m_pDiamonds = switchIntToString(num);
}




