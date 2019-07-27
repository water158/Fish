#ifndef _Fire_Game_Store_
#define _Fire_Game_Store_

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class StoreManager
{
    
public:
	StoreManager(string group_name);
	virtual ~StoreManager();
    
    //调用JAVA内的支付函数
    // callbackTag  用于在NDKHelper中
    // fake_code    购买道具的编号
    // target       回调的目标
    // callback     回调的方法
    void Purchase(string callbackTag, int fake_code, Node* target, SEL_CallFuncND callback);

	//请求订单号
	void RequestOrder(string words, int fake_code, Node* target, SEL_CallFuncND callback);

	//核对订单
	void CheckOrder(string words, int fake_code, Node* target, SEL_CallFuncND callback);

	//网络检查
	void getDeviceInfo(string callbackTag, int fake_code, Node* target, SEL_CallFuncND callback);

	//退出游戏
	void exit(string callbackTag, int fake_code, Node* target, SEL_CallFuncND callback);
    
private:
    
    string m_sGroupName;
};

#endif /* defined(__plane__Store__) */