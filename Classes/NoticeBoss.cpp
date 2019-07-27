//
//  NoticeBoss.cpp
//  Qianpaobuyu
//
//  Created by etgame on 15-6-9.
//
//

#include "NoticeBoss.h"
#include "FishBase.h"
#include "Manager.h"
#include "MyTools.h"
#include "Data.h"
#include "SpellNumber.h"
NoticeBoss::NoticeBoss()
{}
NoticeBoss::~NoticeBoss()
{}
NoticeBoss* NoticeBoss::createNoticeBoss(int _kind, int level)
{
    NoticeBoss* pRet = new NoticeBoss();
    if (pRet&&pRet->initNoticeBoss( _kind, level)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}
bool NoticeBoss::initNoticeBoss(int _kind, int level)
{
    
    size = Director::getInstance()->getWinSize();
    int pos = 0;
    FishBase* bossSp = FishBase::create();
    this->addChild(bossSp);
    bossSp->setIsGray(false);
    bossSp->isSetbody = false;
	//»°·ÑÓã
	if(level == TELEPHONE_FARE_FISH)
		bossSp->initFishBase(TELEPHONE_FARE_FISH);
	else
		bossSp->initFishBase(32 + level);
    bossSp->setPosition(-230, 0);
    bossSp->setScale(0.6);
    bossSp->setRotation(180);
	if(level == 4)
		bossSp->setRotation(270);
	else if(level == 6 || level == 8)
		bossSp->setRotation(-90);

	if(level == TELEPHONE_FARE_FISH)
		bossSp->setRotation(0);
	
    Sprite* notice_bg = Sprite::create("notice_bg.png");
    notice_bg->setPosition(Vec2(0, 0));
    notice_bg->setScale(1.5f);
    this->addChild(notice_bg, -1);
    __String* notice_str = __String::createWithFormat("n_name_%d.png", 3 - _kind);
    Sprite* notice_sp1 = Sprite::create(notice_str->getCString());
    pos = -100;
    notice_sp1->setPosition(Vec2(pos, 0));
    this->addChild(notice_sp1);
    Sprite* notice_sp2 = Sprite::create("n_dis_1.png");
    pos = notice_sp1->getContentSize().width - 100;
    notice_sp2->setPosition(Vec2(pos, 0));
    this->addChild(notice_sp2);
    Sprite* notice_sp3 = Sprite::create("n_dis_3.png");
    pos = notice_sp1->getContentSize().width+notice_sp2->getContentSize().width - 40;
    notice_sp3->setPosition(Vec2(pos, 0));
    this->addChild(notice_sp3);
    Sprite* notice_sp4 = Sprite::create("n_dis_4.png");
    pos = notice_sp1->getContentSize().width+notice_sp2->getContentSize().width+notice_sp3->getContentSize().width - 60;
    notice_sp4->setPosition(Vec2(pos, 0));
    this->addChild(notice_sp4);

//    Node* num_node = Node::create();
//    int num_count = bossSp->value;
//    
//    int i = 0;
//    std::string str = getStringFromInt(num_count);
//    int numLong = strlen(str.c_str());
//    float yello_width = 0;
//    while (num_count) {
//        __String* num_str = __String::createWithFormat("yellow_%d.png", num_count%10);
//        Sprite* num_sp = Sprite::createWithSpriteFrameName(num_str->getCString());
//        num_sp->setPosition(Point(-12*i, 0));
//        i++;
//        num_node->addChild(num_sp, 1);
//        
//        num_count /= 10;
//        yello_width = num_sp->getContentSize().width;
//    }
//    
//    pos = notice_sp1->getContentSize().width - 40 + (numLong - 1)*yello_width/2;
//    num_node->setPosition(pos , 0);
//    this->addChild(num_node, 2);
    
	int nTimes = bossSp->value;
	if(level == 6)
	{
		nTimes = 100;
	}
	log("----------nTimes------------------------");
    SpellNumber* spell = SpellNumber::createSpellNumber(nTimes);
    spell->setPosition(notice_sp1->getContentSize().width - 20, 0);
    this->addChild(spell);
    
    FadeTo* n_fade = FadeTo::create(1, 0);
    DelayTime* n_delay = DelayTime::create(5);
    Sequence* n_seq = Sequence::create(n_delay, n_fade, CallFunc::create(CC_CALLBACK_0(NoticeBoss::removeNotice, this)), nullptr);
    this->runAction(n_seq);
    this->setScale(SCALE);
    return true;
}
void NoticeBoss::removeNotice()
{
    this->removeFromParent();
}