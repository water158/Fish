#include "TelephoneFareFish.h"
#include "FishPath.h"
#include "Manager.h"
#include "PixCell.h"
#include "Coin.h"
#include "Data.h"
#include "MyXmlParser.h"

TelephoneFareFish::TelephoneFareFish()
{
	log("TelephoneFareFish::TelephoneFareFish()");
}
TelephoneFareFish::~TelephoneFareFish()
{
    log("TelephoneFareFish::~TelephoneFareFish()");
}

TelephoneFareFish* TelephoneFareFish::createTelephoneFareFish()
{
    TelephoneFareFish* pRet = new TelephoneFareFish();
    if (pRet && pRet->init()) 
	{
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}
bool TelephoneFareFish::initTelephoneFareFish(int _kind)
{
    isSetbody = true;
    size = Director::getInstance()->getWinSize();
    FishPath* path = new FishPath();
    path->fishPah();
    this->initFishBase(_kind);
    fb_startPoint = path->startPoint;
    fb_endPoint = path->endPoint;
    fb_controlPoint1 = path->controlPoint1;
    fb_controlPoint2 = path->controlPoint2;

	CC_SAFE_DELETE(path);


	m_nFishTeleploneFare = rand() % 10 + 1;
	int nPre = m_nFishTeleploneFare / 10;
	int nLast = m_nFishTeleploneFare % 10;

	//显示钱数
	auto pXmlParser = XMLParser::parseWithFile("ui/loading.xml");
	__String* info = pXmlParser->getString("fare_card");
	std::string strName = __String::createWithFormat(info->getCString(), nPre, nLast)->getCString();

	LabelTTF* pLabel = LabelTTF::create(strName, "Arial", 30);
	pLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
	pLabel->setPosition(Vec2(this->getContentSize().width / 2.0f, this->getContentSize().height / 3.0f * 2.0f));
	this->addChild(pLabel);
	pLabel->setColor(Color3B(255, 255, 0));

    return true;
}
void TelephoneFareFish::moveWithBezier()
{
    int xChange = rand()%200 - 150;
    int yChange = rand()%200 - 100;
    float sx = fb_startPoint.x + xChange;//起点x
    float sy = fb_startPoint.y + yChange;//起点y
    Point sp = Point(sx, sy);//起点位置
    
    
    int ex = fb_endPoint.x + rand()%100 - 40;
    int ey = fb_endPoint.y + rand()%100 - 100;
    Point ep = Point(ex, ey);//终点坐标
    
    int c1x = fb_controlPoint1.x + xChange;//控制点x
    int c1y = fb_controlPoint1.y + yChange;
    Point c1p = Point(c1x, c1y);
    
    int c2x = fb_controlPoint2.x + xChange;
    int c2y = fb_controlPoint2.y + yChange;
    Point c2p = Point(c2x, c2y);
    
    this->setPosition(sp);//精灵的初始位置
    
    
    //动作设置 贝塞尔曲线
    ccBezierConfig bezier;
    if (fb_controlPoint2.equals(Point::ZERO)) 
	{
        bezier.controlPoint_1 = sp;
        bezier.controlPoint_2 = c1p;
        bezier.endPosition = ep;
    }
	else
	{
        bezier.controlPoint_1 = c1p;
        bezier.controlPoint_2 = c2p;
        bezier.endPosition = ep;
    }
    BezierTo* actionMove = BezierTo::create(40, bezier);
    //动作执行完 精灵删除 、、、注、、、易造成二次删除
    CallFunc* callFunc = CallFunc::create(CC_CALLBACK_0(FishBase::removeAllThings, this));
    
    Sequence* sequence = CCSequence::create(actionMove, callFunc, nullptr);
    moveSpeed = Speed::create(sequence, 1.0f);
    moveSpeed->retain();
    this->runAction(moveSpeed);
    
	this->enterSpeedUp();

    this->scheduleUpdate();
}
void TelephoneFareFish::update(float dt)
{
    //sub = this->getPosition() - oPoint;
    //float angle = sub.getAngle();
    //float degress = CC_RADIANS_TO_DEGREES(-1* angle)-90;
    ////this->setRotation(degress);
    //
    //oPoint = this->getPosition();
    
    if (isCatch)
	{
        this->setIsCatch(false);
        this->theSpecialEffects();
        this->catchAnimation(fb_Kind);
    }
}

