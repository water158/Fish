//
//  FishPath.cpp
//  Helloworld
//
//  Created by etgame on 15-1-30.
//
//

#include "FishPath.h"
#define TOTALPATH 17
#define SPRITE_OFFSET 0

int commonNum = 2;
const int pathArray[][6]={
    //   0-x    1-y  2-x  3-y  4-x  5-y  6   7
    {-200*commonNum,100*commonNum,240*commonNum,320*commonNum,560*commonNum,240*commonNum},//0左到右（偏上）--·
    {-200*commonNum,-100*commonNum,240*commonNum,320*commonNum,560*commonNum,120*commonNum},//1左到右（偏下）
    {-100*commonNum, -50*commonNum,240*commonNum,320*commonNum,560*commonNum,-100*commonNum},//2左下到右下
    {-100*commonNum,330*commonNum, -20*commonNum,-100*commonNum,550*commonNum,380*commonNum},//3左上到右上
    {  50*commonNum,-100*commonNum, 30*commonNum,350*commonNum,500*commonNum,350*commonNum},//4左下到右上
    { 600*commonNum,100*commonNum,300*commonNum,100*commonNum,-100*commonNum,300*commonNum},//5右到左（偏上）
    { 550*commonNum,300*commonNum,300*commonNum, -50*commonNum,-150*commonNum,160*commonNum},//6右上到左
    { 600*commonNum,240*commonNum, -20*commonNum,350*commonNum,-150*commonNum,-100*commonNum},//7右到左偏下
    { 550*commonNum,-100*commonNum,450*commonNum,350*commonNum,-100*commonNum,350*commonNum},//8右下到左上
    { 400*commonNum,400*commonNum,150*commonNum,420*commonNum,100*commonNum,-100*commonNum},//9上到下偏左1
    { 300*commonNum,400*commonNum,600*commonNum,100*commonNum, 50*commonNum,-100*commonNum},//10上到下偏右2
    {  50*commonNum,400*commonNum,600*commonNum,150*commonNum,250*commonNum,-100*commonNum},//11上到下偏右1
    { 300*commonNum,550*commonNum,-100*commonNum,100*commonNum,100*commonNum,-100*commonNum},//12上到下偏左2
    {  25*commonNum,-100*commonNum,350*commonNum,200*commonNum,100*commonNum,400*commonNum},//13下到上
    { 200*commonNum,-100*commonNum,-200*commonNum,240*commonNum,350*commonNum,400*commonNum},//14下到上
    { 400*commonNum,-100*commonNum,500*commonNum,200*commonNum,200*commonNum,400*commonNum},//15下到上
    { 450*commonNum,-100*commonNum,-100*commonNum,200*commonNum,260*commonNum,400*commonNum},//16下到上
};

//贝塞尔曲线 两个控制点 曲线
const int fish1OfPath[][8] = {
    {-200*commonNum, 100*commonNum, 120*commonNum, 300*commonNum, 340*commonNum, -300*commonNum, 600*commonNum, 400*commonNum}, //0 从左到右
    {-200*commonNum, 60*commonNum, 120*commonNum, 360*commonNum, 340*commonNum, -80*commonNum, 540*commonNum, 300*commonNum}, //1 上 直游
    {540*commonNum, 300*commonNum, 380*commonNum, 0*commonNum, 200*commonNum, 400*commonNum, -80*commonNum, 100*commonNum}, //2 右到左上
    {500*commonNum, -150*commonNum, 400*commonNum, -200*commonNum, 280*commonNum, 400*commonNum, -100*commonNum, 120*commonNum},//3 右下 左上
    {60*commonNum, -150*commonNum, 160*commonNum, 200*commonNum, 300*commonNum, 20*commonNum, 500*commonNum, 300*commonNum},  //4 左下右上
    {540*commonNum, -110*commonNum, 300*commonNum, -100*commonNum, 340*commonNum, 360*commonNum, 80*commonNum, 300*commonNum},//5 右下 上左
    {400*commonNum, -130*commonNum, 480*commonNum, 160*commonNum, 0*commonNum, 200*commonNum, 0*commonNum, -100*commonNum}, //6 上右 下左
    {-200*commonNum, 300*commonNum, 0*commonNum, 50*commonNum, 400*commonNum, 200*commonNum, 400*commonNum, -80*commonNum}, //7 上左 下中
    {220*commonNum, -100*commonNum, 0*commonNum, 140*commonNum, 480*commonNum, 300*commonNum, 300*commonNum, 400*commonNum}, //8 下左 上右
    {-200*commonNum, 40*commonNum, 100*commonNum, -10*commonNum, 120*commonNum, 300*commonNum, 400*commonNum, 300*commonNum}, //9 左下 上左
    {-200*commonNum, 300*commonNum, 20*commonNum, 100*commonNum, 350*commonNum, 300*commonNum, 200*commonNum, -200*commonNum}, //10  左上 下中
    {-200*commonNum, 100*commonNum, 120*commonNum, 200*commonNum, 380*commonNum, -80*commonNum, 560*commonNum, 200*commonNum}, //11 左到右 下
    {540*commonNum, 240*commonNum, 400*commonNum, 40*commonNum, 100*commonNum, 320*commonNum, -80*commonNum, 100*commonNum}, //12 右到左上
    {380*commonNum, -180*commonNum, 100*commonNum, 100*commonNum, 480*commonNum, 200*commonNum, 240*commonNum, 300*commonNum}, //13 下中 上中
    {100*commonNum, -220*commonNum, 300*commonNum, 140*commonNum, 0*commonNum, 200*commonNum, 100*commonNum, 340*commonNum}, //14 下中 上中
    {160*commonNum, -180*commonNum, 300*commonNum, 0*commonNum, 300*commonNum, 200*commonNum, 500*commonNum, 220*commonNum}, //15 下中 右中
    {300*commonNum, -180*commonNum, 80*commonNum, -100*commonNum, 200*commonNum, 300*commonNum, -200*commonNum, 220*commonNum}, //16 下中 左中
    
};

//样条曲线 路线
const int splineOfPath[][14] = {
    {-100*commonNum, 100*commonNum, 150*commonNum, 210*commonNum,  280*commonNum, 200*commonNum, 350*commonNum, 160*commonNum, 350*commonNum, 40*commonNum, 80*commonNum, 100*commonNum, -100*commonNum, -120*commonNum},
    {-100*commonNum, 300*commonNum, 150*commonNum, 80*commonNum, 350*commonNum, 40*commonNum, 400*commonNum, 120*commonNum, 350*commonNum, 200*commonNum, 180*commonNum, 180*commonNum, -100*commonNum, 20*commonNum},
    {-100*commonNum, 100*commonNum, 100*commonNum, 50*commonNum, 380*commonNum, 80*commonNum, 340* commonNum, 180*commonNum, 140*commonNum, 220*commonNum, 80*commonNum, 80*commonNum,   600*commonNum, 10*commonNum},
    
    {540*commonNum, 10*commonNum, 320*commonNum, 80*commonNum, 140*commonNum, 60*commonNum, 60*commonNum, 100*commonNum, 100*commonNum, 200*commonNum, 280*commonNum, 180*commonNum, 600*commonNum, 20*commonNum},
    {540*commonNum, 200*commonNum, 300*commonNum, 40*commonNum, 180*commonNum, 100*commonNum, 60*commonNum, 60*commonNum, 20*commonNum, 160*commonNum,  160*commonNum, 200*commonNum,  600*commonNum, 20*commonNum},
    {540*commonNum, 100*commonNum, 400*commonNum, 50*commonNum, 60*commonNum, 60*commonNum, 60*commonNum, 180*commonNum, 100*commonNum, 220*commonNum, 200*commonNum, 120*commonNum, 600*commonNum, 20*commonNum}
    
};
//三元四喜路线
const int groupSplinePath[][6] = {
    {-80*commonNum, 100*commonNum, 200*commonNum, 150*commonNum, 480*commonNum, 120*commonNum},
    {500*commonNum, 80*commonNum, 200*commonNum, 120*commonNum, -80*commonNum, 100*commonNum},
};

//const int pathArray[][8]={
//    {-200, 100, 240, 320, 560, 240},//0左到右（偏上）
//    {-200,-100, 240, 320, 560, 120},//1左到右（偏下）
//    {-100, -50, 240, 320, 560,-100},//2左下到右下
//    {-100, 330, -20,-100, 550, 380},//3左上到右上
//    {  50,-100,  30, 350, 500, 350},//4左下到右上
//    {  50, 400, 600, 150, 250,-100},//5上到下偏右1
//    {  25,-100, 350, 200, 100, 400},//6下到上
//    { 600, 100, 300, 100,-100, 300},//7右到左（偏上）
//    { 550, 300, 300, -50,-150, 160},//8右上到左
//    { 600, 240, -20, 350,-150,-100},//9右到左偏下
//    { 550,-100, 450, 350,-100, 350},//10右下到左上
//    { 400, 400, 150, 420, 100,-100},//11上到下偏左1
//    
//    { 300, 400, 600, 100,  50,-100},//12上到下偏右2
//    { 300, 550,-100, 100, 100,-100},//13上到下偏左2
//    
//    { 200,-100,-200, 240, 350, 400},//14下到上
//    { 400,-100, 500, 200, 200, 400},//15下到上
//    { 450,-100,-100, 200, 260, 400},//16下到上
//};


FishPath::FishPath()
{
}
FishPath::~FishPath()
{
//    splineArray->release();
//    groupArr->release();
	//CC_SAFE_RELEASE(splineArray);
	//CC_SAFE_RELEASE(groupArr);
}
void FishPath::addPath()
{
    time = rand()%10 + 18;
    int i = rand()%TOTALPATH;//随机选择17条路中的一条
    
    startPoint = Point(pathArray[i][0], pathArray[i][1]);
    controlPoint1 = Point(pathArray[i][2], pathArray[i][3]);
    controlPoint2 = Point::ZERO;
    endPoint = Point(pathArray[i][4], pathArray[i][5]);
    
    //CCUserDefault::sharedUserDefault()->setIntegerForKey("PATHNUM", i);
}
void FishPath::fishPah()
{
    time = rand() % 30 + 5;
    int i = rand() % 17;
    startPoint = Point(fish1OfPath[i][0], fish1OfPath[i][1]);
    controlPoint1 = Point(fish1OfPath[i][2], fish1OfPath[i][3]);
    controlPoint2 = Point(fish1OfPath[i][4], fish1OfPath[i][5]);
    endPoint = Point(fish1OfPath[i][6], fish1OfPath[i][7]);
}
void FishPath::splinePath()
{
    splineArray = CCPointArray::create(7);
   // splineArray->retain();
    splineNum = rand() % 6;
    splineArray->addControlPoint(Point(splineOfPath[splineNum][0], splineOfPath[splineNum][1]) + (rand()%3-1)*Point(30, 30));
    splineArray->addControlPoint(Point(splineOfPath[splineNum][2], splineOfPath[splineNum][3]) + (rand()%3-1)*Point(30, 30));
    splineArray->addControlPoint(Point(splineOfPath[splineNum][4], splineOfPath[splineNum][5]) + (rand()%3-1)*Point(30, 30));
    splineArray->addControlPoint(Point(splineOfPath[splineNum][6], splineOfPath[splineNum][7]) + (rand()%3-1)*Point(30, 30));
    splineArray->addControlPoint(Point(splineOfPath[splineNum][8], splineOfPath[splineNum][9]) + (rand()%3-1)*Point(30, 30));
    splineArray->addControlPoint(Point(splineOfPath[splineNum][10], splineOfPath[splineNum][11]) + (rand()%3-1)*Point(30, 30));
    splineArray->addControlPoint(Point(splineOfPath[splineNum][12], splineOfPath[splineNum][13]) + (rand()%3-1)*Point(30, 30));
    
}
void FishPath::choseGroupPath()
{
    groupArr = PointArray::create(2);
    //groupArr->retain();
    int choseOne = rand()%2;
    groupArr->addControlPoint(Vec2(groupSplinePath[choseOne][0], groupSplinePath[choseOne][1]));
    groupArr->addControlPoint(Vec2(groupSplinePath[choseOne][2], groupSplinePath[choseOne][3]));
    groupArr->addControlPoint(Vec2(groupSplinePath[choseOne][4], groupSplinePath[choseOne][5]));
}