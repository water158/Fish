//
//  Data.h
//  BuyuDemo
//
//  Created by etgame on 15-3-24.
//
//

#ifndef BuyuDemo_Data_h
#define BuyuDemo_Data_h
#include "cocos2d.h"
using namespace std;


//鱼的名称前缀和帧数
const string d_fishName[42][2] = {
    {"1_", "7"},
    {"28_", "8"},
    {"29_", "8"},
    {"2_", "6"},
    {"3_", "6"},
    {"4_", "6"},
    {"18_", "7"},
    {"31_", "7"},
    {"166_", "8"},
    {"6_", "6"},
    
    {"33_", "11"},
    {"7_", "10"},
    {"79_", "9"},
    {"8_", "6"},
    {"39_", "6"},
    {"62_", "10"},
    {"9_", "8"},
    {"103_", "6"},
    {"10_", "6"},
    {"11_", "5"},
    
    {"12_", "6"},
    {"13_", "10"},
    {"14_", "6"},
    {"17_", "6"},
    {"15_", "6"},
    {"42_", "11"},
    {"16_", "6"},
    {"43_", "11"},
    {"19_", "6"},
    {"20_", "7"},
    
    {"22_", "9"},
    {"51_", "9"},
    {"353_", "9"},//32
    
    {"54_", "10"},
    {"224_", "4"},
    {"23_", "8"},
    {"26_", "10"},
    {"252_", "8"},
    {"49_", "10"},
    {"244_", "11"},
    {"262_", "10"},
    
    {"task_anim_5_3_", "5"}
};

const string d_die_fishName[21][2] = {{"xiaoxiamiA_", "2"}, {"xiaolvA_", "3"},
     {"dandanyuA_", "3"}, {"hetunA_", "3"}, {"xiaohuangbanA_", "3"},
    {"redaiyuA_", "3"}, {"xiaochouyuA_", "3"}, {"shenlanyuA_", "4"},
    {"denglongyuA_", "4"}, {"sanjiaoyuA_", "4"}, {"hudieyuA_", "4"},
    {"xiaowuguiA_", "7"},{"jianyuA_", "3"}, {"bianfuyuA_", "4"},
    {"qunziyuA_", "4"},
    {"yinshaA_", "6"}, {"jinshaA_", "4"},{"haitunA_", "4"}, {"bazhuayuA_", "2"},
    {"lanshaA_", "3"},
    {"dajinshaA_", "3"}};

//鱼的价值 奖励
const int fishValue[42] = {
    2, 2, 2, 3, 4, 5, 5, 5, 6, 7,
    7, 8, 8, 9, 9, 9, 10, 10, 12, 15,
    18, 20, 25, 30, 35, 35, 40, 40, 120, 120,
    200, 200, 200, 300, 300, 400, 400, 500, 500, 600,
    600, 900};
//不同鱼的血量
//const int fishHp[41] = {10, 10, 10, 15, 15, 20, 20, 20, 25, 25, 25, 30, 30, 30,30, 30, 35, 35, 35, 35, 40, 40, 40, 40, 40, 45, 45, 45, 45, 50, 50, 50, 50, 55, 55, 55, 55, 55, 55, 55, 55};
const int fishHp[41] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
const int fishMin[21] = {45, 45, 45, 45, 35, 35, 25, 25, 20, 20, 20, 20, 15, 15, 10, 15, 10, 5, 5, 5};
const int fishMax[21] = {100, 100, 100, 100, 100, 100, 100, 100, 80,80, 70, 70,65, 65, 65,35, 60, 55,55, 55, 55};
//不同鱼死亡的金币数
const int fishCoin[41] = {1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 6, 6, 6,  6, 6, 6, 6, 6};
const int fish_EValue[41] = {
    2, 2, 2, 3, 4, 5, 5, 5, 6, 7,
    7, 8, 8, 9, 9, 9, 10, 10, 12, 15,
    18, 20, 25, 30, 35, 35, 40, 40, 120, 120,
    200, 200, 200, 300, 300, 400, 400, 500, 500, 600,
    600};

const Point bullet[5][4] = {{Point(8, -19), Point(0, -75), Point(-7, -19), Point(1, -8)}, {Point(8, -20), Point(0, -88), Point(-9, -20), Point(0, -9)}, {Point(9.5, -21), Point(1.5, -102), Point(-8.5, -22), Point(-0.5, -10)}, {Point(9.5, -21), Point(1.5, -102), Point(-8.5, -22), Point(0.5, -10)}, {Point(8.5, -60), Point(-0.5, -122), Point(-9.5, -62), Point(-0.5, -23)}};
const Point bullet_red[4] = {Point(7, 30.5), Point(0, -13.5), Point(-7, 30.5), Point(0, 41.5)};//锚点（0.5，0.5）
const Point bullet_blue[4] = {Point(8, 30.5), Point(0, -22.5), Point(-8, 30.5), Point(0, 40.5)};
const Point bullet_bronze[4] = {Point(8.5, 40), Point(0.5, -21), Point(-6.5, 39), Point(0.5, 49)};
const Point bullet_silver[4] = {Point(3.5, 43), Point(0.5, -28), Point(-3.5, 43), Point(0.5, 51)};
const Point bullet_golder[4] = {Point(4.5, 44.5), Point(-0.5, -33.5), Point(-4.5, 44.5), Point(-0.5, 51.5)};


//物理引擎标识
const int fishMask = 1 << 0;
const int bulletMask = 1 << 1;
const int groundMask = 1 << 2;




//炮台基座
const string battery_str[5] = {"battery_red.png", "battery_red.png", "battery_bronze.png", "battery_silver.png", "battery_golden.png"};
const string cannon_str[6] = {"cannon_red", "cannon_blue", "cannon_bronze", "cannon_silver", "cannon_golden", "cannon_power"};
//炮台显示面板
const string display_str[5] = {"display_red.png", "display_red.png", "display_bronze.png", "display_silver.png", "display_golden.png"};
const string contorl_str[5] = {"control_red.png", "control_red.png", "contorl_bronze.png",
    "control_silver.png", "control_golden.png"};
const string up_menu[5] = {"up_red.png", "up_red.png", "up_bronze.png", "up_sliver.png", "up_golden.png"};
const string down_menu[5] = {"down_red.png", "down_red.png", "down_bronze.png", "down_silver.png", "down_golden.png"};
//子弹名字
const string bullet_image[6] = {"bullet_red.png", "bullet_blue.png", "bullet_bronze.png", "bullet_silver.png", "bullet_golder.png", "bullet_power.png"};
const string net_image[6] = {"net_red.png", "net_blue.png", "net_bronze.png", "net_silver.png", "net_golden.png", "net_blue.png"};
const string flame_str[6] = {"fire1.png", "fire1.png", "fire2.png", "fire3.png", "fire4.png", "fire4.png"};

//进入房间的限制金币、子弹的初始威力、子弹的增率、子弹的最大威力、鱼的基础血量、奖励的基础金币
const int limitCoin[11] = {0, 30000, 300000, 800000, 500000, 500000, 500000, 500000, 500000, 500000, 500000 };
const int data_bulletPower[5] = {7, 40, 400, 4000, 6000};
const int uprate[5] = {1, 10, 100, 1000, 1000};
const int maxPower[5] = {10, 100, 1000, 10000, 20000};
const int minPower[5] = {5, 10, 100, 1000, 2000};
const int basisHP[5] = {1, 10, 100, 1000, 10000};
const int basisValue[5] = {1, 1, 1, 1, 1};
#define SCALE 0.75
#define START_TAG 11
#define LEVEL_TAG 12
#define BULLET_TAG 22
#define FISH_TAG 23

#define ENERGYCANNON 50
#define SCOPEFISH 41 //注：41不可变
#define SANYUAN 42
#define SIXI 43
#define YIWANGDAJIN 44
#define TONGLEIZHAIDAN 45
#define NSFISH 46
#define TELEPHONE_FARE_FISH 47
#define BOX_FISH 48

////加载精灵的zOrder
//鱼的zOrder按加载顺序确定
#define NET_zOrder 50
#define COIN_zOrder 290
#define CANNON_zOrder 200
#define WAITMENU_zOrder 200
#define MANUALCANNON_zOrder 200
#define PUBLIC_zOrder 300
#define BOOM_zOrder 250
#define COUNTDOWN_zOrder 400
#define Next_zOrder 400

enum
{
	ZORDER_FISH_SHILED = 0,    //影子
	ZORDER_FISH,               //鱼
	ZORDER_BULLET = 100,       //子弹
	ZORDER_NET,                //鱼网
	ZORDER_EFFECT,             //特效
	ZORDER_STOP = 190,         //技能（定屏）
	ZORDER_TIP,                //提示条
	ZORDER_CANNON = 200,       //炮台
	ZORDER_COIN,               //金币
	ZORDER_PUBLIC,             //弹出框
	ZORDER_COUNTDOWN,          //倒计时
	ZORDER_NEXT,               //下一关
};


const Point cannon_point[4] = {Point(213.5, 28), Point(640.5, 28), Point(213.5, 452), Point(640.5, 452)};

const int d_bulletPower[4][10] = {{1, 2,3,4, 5, 6, 7, 8, 9, 10},
{10, 20, 30, 40, 50, 60, 70, 80, 90, 100},
{100, 200, 300, 400, 500, 600, 700, 800, 900, 1000},
{1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000}};

const int d_bulletPowerNew[4][10] = {
{10, 20, 30, 40, 50},
{60, 70, 80, 90, 100},
{100, 200, 300, 400, 500, 600, 700, 800, 900, 1000},
{1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000}};

const int m_arrCannonFlameNum[4][10] = 
{
	{1, 1, 1, 2, 2, 2, 3, 3, 3, 3},
	{1, 1, 1, 2, 2, 2, 3, 3, 3, 3},
	{1, 1, 1, 2, 2, 2, 3, 3, 3, 3},
	{1, 1, 1, 2, 2, 2, 3, 3, 3, 3}
};

const int m_arrCannonFlameNumNew[4][10] = 
{
	{1, 1, 2, 2, 3},
	{1, 1, 2, 2, 3},
	{1, 1, 1, 2, 2, 2, 3, 3, 3, 3},
	{1, 1, 1, 2, 2, 2, 3, 3, 3, 3}
};


const int f_bulletPower[16] = {1, 5, 10, 20, 50, 100, 150, 200, 300, 400, 500, 600, 700, 800, 900, 1000};

//友盟统计项
enum
{
	YM_SINGLE = 1,      // 点击单人场的数量
	YM_FOUR_PLAYER,            //点击多人场的次数
	YM_ENTER_SINGLE,      //进入单人场的数量
	YM_ENTER_FOUR_PLAYER,         //进入多人场的次数
	YM_FREEGOLD, //免费金币
	YM_SIGN, //签到
	YM_ONLINE_GOLD, //上线活动
	YM_CONFIGURE, //设置
	YM_RECHARGE_GOLD, //充值金币
	YM_RECHARGE_DIAMOND, //充值钻石
	YM_MONTCH_CARD, //月卡
	YM_VIP, //VIP特权
	YM_EXCHANGE_GOLD, //兑换金币
	YM_SERVICE, //客服
	YM_HELP, //帮助
	YM_SPECIAL_PLAY, //特殊玩法
	YM_FISH_RATE, //鱼的倍率
	YM_EXCHANGE_TELEPHONEFARE, //兑换话费
	YM_SMALL_GAME, //小游戏
	YM_MISSION_1, //第一关
	YM_MISSION_2,
	YM_MISSION_3,
	YM_MISSION_4,
	YM_MISSION_5,
	YM_MISSION_6,
	YM_MISSION_7,
	YM_MISSION_8,
	YM_MISSION_9,
	YM_MISSION_10,
	YM_SING_ENTER_GAME,//单人进入游戏
	YM_USE_SKILL_1,//五个技能的使用
	YM_USE_SKILL_2,
	YM_USE_SKILL_3,
	YM_USE_SKILL_4,
	YM_USE_SKILL_5,
	YM_SWITCH_GUN,   //换炮
	YM_UNLOCK_GUN_1,   //点击具体的炮
	YM_UNLOCK_GUN_2,
	YM_UNLOCK_GUN_3,
	YM_UNLOCK_GUN_4 = 40,
	YM_SING_RETURN,  //返回
	YM_SING_MENU,  //单人场的菜单
	YM_MANY_1,  //多人场的房间
	YM_MANY_2,
	YM_MANY_3,
	YM_MANY_4,
	YM_BUY_GOLD_6,  //点击商店的次数
	YM_BUY_GOLD_15,
	YM_BUY_GOLD_30,
	YM_BUY_GOLD_50,
	YM_BUY_GOLD_108,
	YM_BUY_GOLD_328,
	YM_BUY_GOLD_01,
	YM_BUY_DIAMOND_6,
	YM_BUY_DIAMOND_15,
	YM_BUY_DIAMOND_30,
	YM_BUY_DIAMOND_50,
	YM_BUY_DIAMOND_108,
	YM_BUY_DIAMOND_328,
	YM_GIFT_1,  //新手礼包
	YM_GIFT_2,  //特惠礼包
	YM_GIFT_3,  //破产礼包
	YM_GIFT_4,  //贵族礼包
	YM_SUCCESS_BUY_GOLD_6 = 64,  //成功购买的次数
	YM_SUCCESS_BUY_GOLD_15,
	YM_SUCCESS_BUY_GOLD_30,
	YM_SUCCESS_BUY_GOLD_50,
	YM_SUCCESS_BUY_GOLD_108,
	YM_SUCCESS_BUY_GOLD_328,
	YM_SUCCESS_BUY_GOLD_01,
	YM_SUCCESS_BUY_DIAMOND_6,
	YM_SUCCESS_BUY_DIAMOND_15,
	YM_SUCCESS_BUY_DIAMOND_30,
	YM_SUCCESS_BUY_DIAMOND_50,
	YM_SUCCESS_BUY_DIAMOND_108,
	YM_SUCCESS_BUY_DIAMOND_328,
	YM_SUCCESS_GIFT_1,  //新手礼包
	YM_SUCCESS_GIFT_2,  //特惠礼包
	YM_SUCCESS_GIFT_3,  //破产礼包
	YM_SUCCESS_GIFT_4,  //贵族礼包
};

//付费方式
enum
{
	PAY_TYPE_NONE,
	PAY_TYPE_QF,
	PAY_TYPE_THRAN,
	PAY_TYPE_SKY,
    PAY_TYPE_APPSTORE,
    PAY_TYPE_XY
};

//消费物品代号
enum
{
	BUY_GOLD_6 = 1,
	BUY_GOLD_15,
	BUY_GOLD_30,
	BUY_GOLD_50,
	BUY_GOLD_108,
	BUY_GOLD_328,
	BUY_GOLD_01,
	BUY_DIAMOND_6,
	BUY_DIAMOND_15,
	BUY_DIAMOND_30,
	BUY_DIAMOND_50,
	BUY_DIAMOND_108,
	BUY_DIAMOND_328,
	BUY_GIFT_1,       //新手礼包
	BUY_GIFT_2,		  //特惠礼包
	BUY_GIFT_3,       //破产礼包
	BUY_GIFT_4,		  //贵族礼包
};

//主菜单的页面
enum
{
	MENU_MAIN = 0,
	MENU_SINGLE,
	MENU_MANY,
};

//获得的道具类型
enum
{
	SKILL_1,
	SKILL_2,
	SKILL_3,
	SKILL_4,
	DIAMOND,
};

//获得的道具类型
enum
{
	REWARD_STATUS_NONE,
	REWARD_STATUS_WAIT,
	REWARD_STATUS_COME,
    REWARD_STATUS_VALIDATING
};

//运营商类型
enum
{
	MOBILE_TYPE_NONE,
	MOBILE_TYPE_YIDONG,
	MOBILE_TYPE_LIANTONG,
	MOBILE_TYPE_DIANXIN,
};

enum 
{
	QingNingZhuShou     = 10001,
	WanDouJia           = 10002,
	WiFiWanNengYaoShi   = 10003,
	SouYing             = 10004,
	YouKu               = 10005,
	FengWang            = 10006,
	ChengLongXinXi      = 10007,
	ZhangXingLiYi       = 10008,
	NanJingYouTong      = 10009,
	XuhangWangLuo       = 10010,
	C4399               = 10011,
	ZongHengTianYa      = 10012,
	FengHuaHeRui        = 10013,
	HuoCheZhanWiFi      = 10014,
	QiFan               = 10015,
	WanJiangPing        = 10016,
	MuMaYi              = 10017,
	IOS                 = 10018,
	XY                  = 10019,
	SouGouShouJiZhuShu  = 10020,
	JiFengShiChang      = 10021,
	MuZhiWan            = 10022,
	AiQiYi              = 10023,
	PPZhuShou           = 10024,
    GooglePlay          = 10025,
};

#endif
