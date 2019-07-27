//
//  Manager.cpp
//  Qianpaobuyu
//
//  Created by etgame on 15-5-14.
//
//

#include "Manager.h"
#include "Data.h"
#include "ManualCannon.h"

#include "extensions/cocos-ext.h"
#include "ui/UIButton.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace cocostudio;

const Point bullet_one[4] = {Point(-7, 20.5), Point(-5, 31.5), Point(4, 31.5), Point(7, 20.5)};
const Point bullet_two[4] = {Point(-21, 55.5), Point(-21, 71.5), Point(21, 71.5), Point(21, 55.5)};
const Point bullet_three[4] = {Point(-25, 20.5), Point(-25, 31.5), Point(25, 31.5), Point(25, 20.5)};


bool Manager::init()
{
    roomNum = 2;
    one_levelNum = 0;
	m_nBulletNum = 0;
    bodiesDic = __Dictionary::create();
    //bodiesDic->retain();
    m_bulletPower = 20;
    m_BulletNum = 2;
    m_bEnterLoading = false;
    isCoinChange = false;
    isStopBoom = false;
	m_bEnterGame = true;
	isLockFish = true;
	m_bInSmallGame = false;
	m_bOpenFromVIP = false;
	m_bNeedOpenVIP = false;
	m_nTipID = 0;
	m_nFishID = 0;
	m_nPoolGoldNum = 0;
	m_nManyPoolGoldNum = 0;
	m_nTaskFishID1 = 0;
	m_nTaskFishID2 = 0;
	m_nTaskFishID3 = 0;
	m_bReleaseBoxFish = false;
	m_bTaskOver = false;
	m_nCreateBossIndex = 0;
	m_bInternet = true;
	m_bFirstEnter = true;
	m_nPayWays = 0;
	m_bMMPay = false;
	m_bPopGift = true;
	m_bRefreshGun = false;
	m_bOpenFreeGoldFromSign = true;

    return true;
}
Manager* Manager::instance = nullptr;
Manager* Manager::getInstance()
{
    if (instance == NULL) {
        instance = new Manager();
        instance->init();
    }
    
    return instance;
}
void Manager::clean()
{
    bodiesDic->release();
    
}
PhysicsBody* Manager::loadBulletBodies(int kind)
{
	PhysicsBody* pBulletBody = NULL;
    switch (kind) 
	{
        case 1:
            pBulletBody = PhysicsBody::createPolygon(bullet_one, 4);
            break;
        case 2:
            pBulletBody = PhysicsBody::createPolygon(bullet_two, 4);
            break;
        case 3:
            pBulletBody = PhysicsBody::createPolygon(bullet_three, 4);
            break;
        default:
            break;
    }
    pBulletBody->setCategoryBitmask(bulletMask);
    pBulletBody->setCollisionBitmask(fishMask);
    pBulletBody->setContactTestBitmask(fishMask);
    pBulletBody->setGravityEnable(false);
    pBulletBody->setTag(100);
    
	pBulletBody->retain();
	return pBulletBody;
}
PhysicsBody* Manager::loadFishBodies(int kind)
{
	PhysicsBody* pFishBody = NULL;
    if (kind < 41) 
	{
        pFishBody = PhysicsBody::create(); 
        //    const std::string testFilePath = FileUtils::getInstance()->fullPathForFilename("FishTotal.plist");
        //    auto plistDic = __Dictionary::createWithContentsOfFile(testFilePath.c_str());
        //     bodiesDic = dynamic_cast<__Dictionary*>(plistDic->objectForKey("bodies"));
        
        string fishname = d_fishName[kind][0];
        string fishFullName = fishname + "01";
        //log("fishFullName = %s", fishFullName.c_str());
        auto bazhuyuDic = dynamic_cast<__Dictionary*>(bodiesDic->objectForKey(fishFullName));
        
        auto fixturesArr = dynamic_cast<__Array*>(bazhuyuDic->objectForKey("fixtures"));
        auto item0Dic = dynamic_cast<__Dictionary*>(fixturesArr->getObjectAtIndex(0));
        auto polygonsArr = dynamic_cast<__Array*>(item0Dic->objectForKey("polygons"));
        auto dic_num = polygonsArr->count();
        //log("dic_num = %ld", dic_num);
        
        
        
        
        for (int i = 0; i < dic_num; i++) 
		{
            auto item0Arr = dynamic_cast<__Array*>(polygonsArr->getObjectAtIndex(i));
            auto item_num = item0Arr->count();
            
            Point myPoint[8];
            for (int j = 0; j < item_num; j++) {
                auto itemsStr = dynamic_cast<__String*>(item0Arr->getObjectAtIndex(j));
                //log("itemsStr = %s", itemsStr->getCString());
                __Array* str_arr1 = itemsStr->componentsSeparatedByString("{ ");
                auto str_split1 = dynamic_cast<__String*>(str_arr1->getObjectAtIndex(0));
                auto str_arr2 = str_split1->componentsSeparatedByString(" }");
                auto str_split2 = dynamic_cast<__String*>(str_arr2->getObjectAtIndex(0));
                auto str_arr = str_split2->componentsSeparatedByString(",");
                
                
                //log("%s", dynamic_cast<__String*>(str_arr->getObjectAtIndex(k))->getCString());
                auto first_num = dynamic_cast<__String*>(str_arr->getObjectAtIndex(0));
                auto second_num = dynamic_cast<__String*>(str_arr->getObjectAtIndex(1));
                Point bodyPoint;
                bodyPoint = Point(first_num->floatValue(), second_num->floatValue());
                
                myPoint[j] = bodyPoint;
                
            }
            pFishBody->addShape(PhysicsShapePolygon::create(myPoint, (int)item_num));
            
        }
    }
    if (kind == ENERGYCANNON) 
	{
        pFishBody = PhysicsBody::createBox(Size(85, 113));
    }
	if (kind == TELEPHONE_FARE_FISH) 
	{
		pFishBody = PhysicsBody::createBox(Size(113, 113));
	}
	if (kind == BOX_FISH) 
	{
		pFishBody = PhysicsBody::createBox(Size(113, 113));
	}
    if (kind == SCOPEFISH) 
	{
        pFishBody = PhysicsBody::createCircle(50);
    }
    pFishBody->setRotationEnable(false);
    pFishBody->getShape(0)->setMass(100000000);
    pFishBody->setCategoryBitmask(fishMask);
    pFishBody->setCollisionBitmask(bulletMask);
    pFishBody->setContactTestBitmask(bulletMask);
    pFishBody->setGravityEnable(false);
    pFishBody->setTag(200);

	pFishBody->retain();
	return pFishBody;
}

void Manager::destroyInstance()
{
    if (instance != NULL) {
		log("delete instance;");
        //delete instance;
		CC_SAFE_DELETE(instance);
        //instance = NULL;
    }
}

//ÇåÀí»º´æ
void Manager::cleanBuffer()
{
	SceneReader::getInstance()->destroyInstance();
	GUIReader::getInstance()->purgeGUIReader();
	if(!Manager::getInstance()->getInSmallGame())
		SpriteFrameCache::getInstance()->destroyInstance();
	TextureCache::getInstance()->destroyInstance();
	AnimationCache::getInstance()->destroyInstance();		
	ActionManagerEx::getInstance()->destroyInstance();
}