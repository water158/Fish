//
//  FishMedol.cpp
//  Qianpaobuyu
//
//  Created by etgame on 15-5-16.
//
//

#include "FishMedol.h"
#include "ProductType.h"
#include "MyActionNew.h"
#include "BoomData.h"
#include "Manager.h"
#include "GlobalDelegate.h"
#include "FishBoom.h"

const int payTime[] = {4, 5, 24, 6, 6, 8, 8};
const int hPath[][6] = {
    {-300, 100, 940, 100},
    {-300, 200, 980, 200},
    {-300, 300, 980, 300},
    {-300, 400, 940, 400},
    {300, 540, 300, -100},
    {500, 540, 500, -100},
    {300, -100, 300, 400},
    {500, -100, 500, 400},
};
FishMedol::FishMedol():fm_recordtimer(0)
{
	//log("FishMedol::FishMedol()");
}
FishMedol::~FishMedol()
{
	//log("FishMedol::~FishMedol()");
}
FishMedol* FishMedol::createFishMedol()
{
    FishMedol* pRet = new FishMedol();
    if (pRet&&pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}
bool FishMedol::initFishMedol()
{
    size = Director::getInstance()->getWinSize();
    instance = Manager::getInstance();
    typeNum = rand()%7;
    totalTime = payTime[typeNum];

    this->schedule(schedule_selector(FishMedol::addFish), 0.5);
    
    return true;
}
void FishMedol::addFish(float dt)
{
	log("FishMedol::addFish-----1------------");
    if (instance->getIsStopBoom()) 
	{
		log("FishMedol::addFish-----2------------");
        return;
    }
	log("FishMedol::addFish-----3------------");
    fm_recordtimer++;
	
    switch (typeNum) 
	{
        case 0: //4s
        {
            for (int i = 0; i < 8; i++) 
			{
                ProductType* fish0 = ProductType::create();
                instance->getM_layer()->addChild(fish0);
                fish0->initProductType(0);
                fish0->setPosition(Point(size.width/2, size.height/2));
                fish0->setScale(0.1);
				fish0->setDelegateGame((GameDelegate*)(instance->getM_layer()));
                instance->m_fishVect.pushBack(fish0);
                MyActionNew* myAction0 = MyActionNew::create(3 + type0_hudie[i][0]*1, 120 + type0_hudie[i][1]*60, type0_hudie[i][2], type0_hudie[i][3], true, 1);
                ScaleTo* toBig = ScaleTo::create(0.1, 1.0);
                ScaleTo* toSmall = ScaleTo::create(0.1, 0.1);
                
                Sequence* seq1 = Sequence::create(toBig, DelayTime::create(2.8 + type0_hudie[i][0]), toSmall, nullptr);
                Spawn* spawn_type0 = Spawn::create(myAction0, seq1, nullptr);
                Sequence* seq_type0 = Sequence::create(spawn_type0, CallFuncN::create(CC_CALLBACK_1(FishMedol::removeType, this)), nullptr);
                fish0->runAction(seq_type0);
				fish0->setDelegateGame((GameDelegate*)(instance->getM_layer()));
            }
        }
            break;
        case 1:
        {
            //彩虹桥型
            for (int i = 0; i < 6; i++) 
			{
                ProductType* fish1 = ProductType::create();
                instance->getM_layer()->addChild(fish1);
                fish1->initProductType(8);
                if (i < 3) 
				{
                    fish1->setPosition(Point(-fish1->getContentSize().width/2, size.height-180) - Point(0, 120)*i);
                }
				else
				{
                    fish1->setPosition(Point(size.width - 180, -fish1->getContentSize().height/2) - Point(120, 0)*(i-3));
                }
                fish1->setScale(0.1);
                instance->m_fishVect.pushBack(fish1);
                fish1->setDelegateGame((GameDelegate*)(instance->getM_layer()));
                ScaleTo* toBig = ScaleTo::create(0.1, 1.0);
                ScaleTo* toSmall = ScaleTo::create(0.1, 0.1);
                Sequence* seq_fade = Sequence::create(toBig, DelayTime::create(2.8 + type1_caihongqiao[i][0]), toSmall, nullptr);
                
                MyActionNew* myAction1 = MyActionNew::create(3+type1_caihongqiao[i][0], 120+type1_caihongqiao[i][1], type1_caihongqiao[i][2], type1_caihongqiao[i][3], true, 1);
                Spawn* spaw_type1 = Spawn::create(seq_fade, myAction1, nullptr);
                Sequence* seq_type1 = Sequence::create(spaw_type1, CallFuncN::create(CC_CALLBACK_1(FishMedol::removeType, this)), nullptr);
                fish1->runAction(seq_type1);
                
                fish1->setDelegateGame((GameDelegate*)(instance->getM_layer()));
            }
        }
            break;
        case 2:
        {//蜗牛型
            for (int i = 0; i < 2; i++) 
			{
                ProductType* fish2 = ProductType::create();
                instance->getM_layer()->addChild(fish2);
                fish2->initProductType(12);
                fish2->setPosition(Vec2(size.width/2 - 80*sin(M_PI*(0.5 + i)), size.height+fish2->getContentSize().height/2));
                fish2->setScale(0.1);
                instance->m_fishVect.pushBack(fish2);
                MyActionNew* myAction2 = MyActionNew::create(6, 200, type2_woniu[i][2], type2_woniu[i][3], true, 1, true);
                Repeat* repeat = Repeat::create(myAction2, 4);
                ScaleTo* toBig = ScaleTo::create(0.1, 1.0);
                ScaleTo* toSmall = ScaleTo::create(0.1, 0.1);
                Sequence* seq_fade = Sequence::create(toBig, DelayTime::create(6*4 - 0.2), toSmall, nullptr);
                Spawn* spawn = Spawn::create(repeat,seq_fade, nullptr);
                Sequence* seq_type2 = Sequence::create(spawn, CallFuncN::create(CC_CALLBACK_1(FishMedol::removeType, this)), nullptr);
                fish2->runAction(seq_type2);
                fish2->setDelegateGame((GameDelegate*)(instance->getM_layer()));
                
            }
            
        }
            break;
        case 3:
        {//漩涡形
            for (int i = 1; i <= 4; i++) 
			{
                ProductType* fish3 = ProductType::create();
                instance->getM_layer()->addChild(fish3);
                fish3->initProductType(13);
                fish3->setPosition(Vec2(size.width/2, size.height/2));
                fish3->setScale(0.1);
                instance->m_fishVect.pushBack(fish3);
                ScaleTo* toBig = ScaleTo::create(0.1, 1.0);
                ScaleTo* toSmall = ScaleTo::create(0.1, 0.1);
                Sequence* seq_fade = Sequence::create(toBig, DelayTime::create(6 - 0.2), toSmall, nullptr);
                // MyActionNew* myAction3 = MyActionNew::create(6, 60, CC_RADIANS_TO_DEGREES((M_PI/4)*i), 360);
                MyActionNew* myAction3 = MyActionNew::create(6, 100, (360/8)*i, 360, true, 1);
                Spawn* spaw = Spawn::create(seq_fade, myAction3, nullptr);
                Sequence* seq_type3 = Sequence::create(spaw, CallFuncN::create(CC_CALLBACK_1(FishMedol::removeType, this)) , nullptr);
                fish3->runAction(seq_type3);
                fish3->setDelegateGame((GameDelegate*)(instance->getM_layer()));
            }
        }
        case 4:
        {
            //圆形
            for (int i = 0; i < 2; i++) 
			{
                ProductType* fish4 = ProductType::create();
                instance->getM_layer()->addChild(fish4);
                fish4->initProductType(13);
                fish4->setPosition(Vec2(size.width/2 - 60*(i+1), size.height/2));
                instance->m_fishVect.pushBack(fish4);
                
                MyActionNew* myAction4 = MyActionNew::create(6, 100+60*i, 0, 360, true, 1);
                ScaleTo* toBig = ScaleTo::create(0.1, 1.0);
                ScaleTo* toSmall = ScaleTo::create(0.1, 0.1);
                Sequence* seq_fade = Sequence::create(toBig, DelayTime::create(6 - 0.2), toSmall, nullptr);
                Spawn* spawn_type4 = Spawn::create(myAction4, seq_fade, nullptr);
                Sequence* seq_type4 = Sequence::create(spawn_type4, CallFuncN::create(CC_CALLBACK_1(FishMedol::removeType, this)) , nullptr);
                fish4->runAction(seq_type4);
				fish4->setDelegateGame((GameDelegate*)(instance->getM_layer()));
            }
            
            
        }
            break;
        case 5:
        {
            for (int i = 0; i < 2; i++) 
			{
                ProductType* fish5 = ProductType::createProductType();
                instance->getM_layer()->addChild(fish5);
                fish5->initProductType(15);
                fish5->setPosition(Vec2(fish5->getContentSize().width, size.height/2));
                fish5->setScale(0.1);
                instance->m_fishVect.pushBack(fish5);
                MyActionNew* myAction5 = MyActionNew::create(4, size.width / 4.0f, 180*i, 180*sin(M_PI*0.5+M_PI*i), true, 1);
                MyActionNew* myAction5_1 = MyActionNew::create(4, size.width / 4.0f, 180*(i+1), 180*sin(-M_PI*0.5 + M_PI*i), true, 1);
                Sequence* seq_type5 = Sequence::create( myAction5, myAction5_1, nullptr);
                ScaleTo* toBig = ScaleTo::create(0.1, 1.0);
                ScaleTo* toSmall = ScaleTo::create(0.1, 0.1);
                Sequence* seq_fade = Sequence::create(toBig, DelayTime::create(8 - 0.2), toSmall, nullptr);                Spawn* spawn_type5 = Spawn::create( seq_type5, seq_fade, nullptr);
                Sequence* seq = Sequence::create(spawn_type5, CallFuncN::create(CC_CALLBACK_1(FishMedol::removeType, this)), nullptr);
                fish5->runAction(seq);
				fish5->setDelegateGame((GameDelegate*)(instance->getM_layer()));
            }
        }
            break;
        case 6:
        {
			if(fm_recordtimer % 2 == 0)
			{
				for (int i = 0; i < 5; i++) 
				{
					ProductType* fish6 = ProductType::createProductType();
					instance->getM_layer()->addChild(fish6);
					int fishKind = i + 5;
					if(i > 2)
					{
						fishKind = 9 - i;
					}
					else if( i == 2)
					{
						fishKind = 24;
					}
					fish6->initProductType(fishKind);
					instance->m_fishVect.pushBack(fish6);

					fish6->runMove(Vec2(size.width + 80, 140 + 50 * i), Vec2(-size.width-80, 0));
					fish6->setRotation(90);

					fish6->setDelegateGame((GameDelegate*)(instance->getM_layer()));
				}
			}

        }
            break;
            
        default:
            break;
    }
    
    if (fm_recordtimer == 40) 
	{
		if(this->getDelegateGame())
			this->getDelegateGame()->stopFishBoom();
        this->removeFishMedol();
    }
}

void FishMedol::removeType(Node* ref)
{
    ProductType* fish_type = dynamic_cast<ProductType*>(ref);
    fish_type->removeAllThings();
	if (fish_type->getUID() == Manager::getInstance()->getCurLockUID()) 
	{
		fish_type->setLocked(false);
		fish_type->removeChildByTag(103);
		fish_type->getDelegateGame()->stopShooting();
	}
}
void FishMedol::removeFishMedol()
{
    this->removeFromParentAndCleanup(true);
}