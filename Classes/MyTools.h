#ifndef _TOOLS_H_
#define _TOOLS_H_

#include "cocos2d.h"
USING_NS_CC;
#include "Data.h"

//两个点的位置，指定速度，就能计算出平均速度需要的时间
float AverageSpeed(Vec2 begin_point ,Vec2 end_point,float speed);

//两个点，需要旋转的角度
float PointRotatePoint(Vec2 begin_point ,Vec2 end_point ,bool isUp=true);

float calcTwoPointLength(Vec2 p_begin, Vec2 p_end);

float calcTwoPointAngle(Vec2 begin_point, Vec2 end_point);

//精灵遮罩
Sprite* ccspmaskedSpriteWithSprite(cocos2d::Sprite* textureSprite, cocos2d::Sprite* maskSprite);

//圆形与点的碰撞
bool isRoundContainsPoint(Node *aimnode,cocos2d::Point aimpoint);
//圆形与圆形的碰撞
bool isRoundContainsNode(Node *aimroundnode,Node *aimnode);

//用于创建无法修改的包内文件副本（创建用户存储文件）
void copyData(const char* pFileURL,const char* pFileName);
//用于判断本地文件是否存在
bool isFileExist(const char* pFileName);

//随机范围值
int getRandom(int begain_num,int end_num);  //包括两端部分

//char *转为 int  float
int getIntFromS(const char *temp_string);
float getFloatFromS(const char *temp_string);

//int float转为string
std::string getStringFromInt(int temp_int);
std::string getStringFromFloat(float temp_float);

float calcRadius(float x, float y);
float calcAngleDegrees(float x, float y);

float calcAngleFromOriginToTarget(Vec2 origin, Vec2 target);



//将int加入到指定字符串中
//std::string getFormatString(std::string str_aim,int n_format);

cocos2d::Point getRandomPositionByOffset(Vec2 anchor_point, float offset_len_x, float offset_len_y);

//范围随机点
Vec2 getRandomPositionByBound(Vec2 anchor_point, float min_x, float min_y, float max_x, float max_y);


bool compareString(const char* src, const char* dst);

//获得一个点相对于另一个点的对称点
Vec2 getMappingPoint(Vec2 pCenter, Vec2 pOne, float angle);

//获取系统当前时间
long GetTime();
tm* GetTime1();

//根据图片和指定的的色相和饱和度，创建新的图片
Sprite* getGrayLightWithSprite(Sprite* old_sprite, int is_plus, int saturation);
void SwapRGB(int *a, int *b);
void CheckRGB(int *Value);
void AssignRGB(int *R, int *G, int *B, int intR, int intG, int intB);
void SetBright(int *R, int *G, int *B, int bValue);
void SetHueAndSaturation(int *R, int *G, int *B, int hValue, int sValue);

//shader
void graySprite(Node* sprite);

//加密
std::string switchIntToString(int num);
int switchStringToInt(std::string p);


#define CHANNEL_NAME(__channel__) \
case __channel__: \
	return #__channel__

static int CURRENT_CHANNEL = GooglePlay;

// 获取友盟渠道名
std::string getUmengChannelString();


#endif
