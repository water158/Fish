#ifndef _HTTPLAYER_H_
#define _HTTPLAYER_H_

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "network/HttpClient.h"
#include "MyTools.h"
#include "Store.h"

using namespace cocos2d;
USING_NS_CC_EXT;
using namespace cocos2d::network;

//刷新付费请求的次数
#define TOTAL_REQUEST_NUM 60
//刷新付费请求的时间间隔
#define REQUEST_OFFEST_TIME 1800

class PublicLayer;
class HttpInstance : public Layer
{
public:
	~HttpInstance();

	static HttpInstance* getInstance();
	
	//请求验证验证码是否正确
	void requestIdentifyingCode(std::string strCode);
	//返回验证码是否正确的结果
	void returnCodeResult(Node *node,void *data);

	//请求付费（发送付费信息到服务器）
	void requestRecharge1(std::string strCode);
	//返回请求付费的结果
	void returnRechargeResult1(Node *node,void *data);

	//请求付费结果（支付结果是否成功）
	void requestRecharge2(std::string strCode);
	//返回请求付费的结果
	void returnRechargeResult2(Node *node,void *data);

	//获取是否在游戏中
	bool getInGame();

	void updatePayTime(float dt);

private:
	HttpInstance();

	static HttpInstance* m_pSharedHttpInstance;

	bool init();


public:
	CC_SYNTHESIZE(PublicLayer*, m_pPublicLayer, PublicLayer);

	//发放奖励的状态
	CC_SYNTHESIZE(int, m_nRewardStatus, RewardStatus);
	//当前的订单号
	CC_SYNTHESIZE(std::string, m_strCurOrderString, CurOrderString);
	//当前设备的mac地址
	CC_SYNTHESIZE(std::string, m_strDeviceMac, DeviceMac);
	//当前版本
	CC_SYNTHESIZE(std::string, m_strCurVersion, CurVersion);
	//当前设备的运营商
	CC_SYNTHESIZE(int, m_nMobileType, MobileType);
	//当前支付的物品ID
	CC_SYNTHESIZE(int, m_nFakeCode, FakeCode);
	//当前支付类型
	CC_SYNTHESIZE(int, m_nPayType, PayType);
	//是否可以进行支付
	CC_SYNTHESIZE(bool, m_bCouldPay, CouldPay);
	CC_SYNTHESIZE(StoreManager*, m_pStore, Store);

	//回调下标
	CC_SYNTHESIZE(int, m_nCallBackIndex, CallBackIndex);
	//支付等待时间下标
	CC_SYNTHESIZE(int, m_nPayWaitIndex, PayWaitIndex);
	//是否按时间恢复
	CC_SYNTHESIZE(bool, m_bResumeByTime, ResumeByTime);

	//渠道号
	CC_SYNTHESIZE(std::string, m_strChannelID, ChannelID);
    // appstore 支付凭证
    CC_SYNTHESIZE(std::string, m_strTransactionReceipt, TransactionReceipt);
};


#endif