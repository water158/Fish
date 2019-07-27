#include "HttpInstance.h"
#include "PublicLayer.h"
#include "CCStdC.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "UserChannel.h"

using namespace  rapidjson;

HttpInstance* HttpInstance::m_pSharedHttpInstance = NULL;

HttpInstance::HttpInstance()
{
}

HttpInstance::~HttpInstance()
{
	CC_SAFE_DELETE(m_pStore);
	CCLog(" HttpInstance is destroy");
}

HttpInstance* HttpInstance::getInstance()
{
	if(!m_pSharedHttpInstance)
	{
		m_pSharedHttpInstance = new HttpInstance();
		m_pSharedHttpInstance->init();
	}

	return m_pSharedHttpInstance;
}

bool HttpInstance::init()
{
	Layer::init();

	m_nRewardStatus = REWARD_STATUS_NONE;
	m_strCurOrderString = "";
	m_strDeviceMac = "";
	m_nMobileType = 0;
	m_nFakeCode = 0;
	m_strCurVersion = "";
	m_nPayType = PAY_TYPE_THRAN;
	m_bCouldPay = true;
	m_nCallBackIndex = 1;
	m_nPayWaitIndex = 0;
	m_bResumeByTime = false;
    std::stringstream ss;
    ss << (int)CURRENT_CHANNEL;
    //m_strChannelID = getUmengChannelString();
	m_strChannelID = "10020";
	m_pStore = new StoreManager("DiamondStoreLayer");

	this->schedule(schedule_selector(HttpInstance::updatePayTime), 1);

	return true;
}

void HttpInstance::updatePayTime(float dt)
{
	m_nPayWaitIndex ++;
	log("=+++++++++++++++++++++++++++++++++++++++++++++++++++++++");
	if(m_nPayWaitIndex == 90)
	{
		//m_bCouldPay = true;
		m_nPayWaitIndex = 0;
	}
}

//获取是否在游戏中
bool HttpInstance::getInGame()
{
	return m_pPublicLayer->getInGame();
}

//请求验证验证码是否正确
void HttpInstance::requestIdentifyingCode(std::string strCode)
{
	//创建对象
	cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
	//设置请求地址
	std::string strNet = "http://www.jiuaitang.com/fishBill.php?Code=" + strCode;
	request->setUrl(strNet.c_str());
	//连接类型为GET
	request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
	//请求完的回调函数
	request->setResponseCallback(this, callfuncND_selector(HttpInstance::returnCodeResult));
	//请求的数据
	request->setRequestData(strCode.c_str(),strlen(strCode.c_str()));
	//tag
	request->setTag("102");
	//发送请求
	cocos2d::network::HttpClient::getInstance()->send(request);
	//释放发送的数据
	request->release();
}

//返回验证码是否正确的结果
void HttpInstance::returnCodeResult(Node *node,void *data)
{
	//接收返回的消息
	cocos2d::network::HttpResponse* pResponse = (cocos2d::network::HttpResponse*)data;
	if (!pResponse)
	{
		return;
	}
	//获取请求的类型
	cocos2d::network::HttpRequest::Type nRequestType = pResponse->getHttpRequest()->getRequestType();
	log("request type %d", nRequestType);

	//请求的标记
	if (0 != strlen(pResponse->getHttpRequest()->getTag())) 
	{
		log("%s ------>oked", pResponse->getHttpRequest()->getTag());
	}

	//返回码statusCode
	int statusCode = pResponse->getResponseCode();
	log("response code: %d", statusCode);

	//不成功就返回
	if (!pResponse->isSucceed()) 
	{
		log("response failed");
		log("error buffer: %s", pResponse->getErrorBuffer());

		m_pPublicLayer->showPublicSureLayer("net_check");
		return;
	}

	//接收返回的信息
	std::vector<char> *pBuffer = pResponse->getResponseData();
	log("-----%s-------",pBuffer);

	char pChar[100] = "";
	int nSize = pBuffer->size() > 100 ? 100 : pBuffer->size();
	//字符串去空格
	for (int i = 0; i < nSize; i++)
	{
		char c = (*pBuffer)[i];
		if(c != 10)
		{
			pChar[strlen(pChar)] = (*pBuffer)[i];
		}
	}

	int nValue = getIntFromS(pChar);
	if(nValue > 0 && nValue < 14)
	{
		//在这里处理结果
		Dictionary* pDic = Dictionary::create();
		pDic->setObject(__String::createWithFormat("%d", nValue), "fake_code");
		pDic->setObject(__String::createWithFormat("%d", 0), "is_test");
		pDic->setObject(__String::createWithFormat("%d", 1), "result");
		m_pPublicLayer->callbackConfigure(this, pDic);
		m_pPublicLayer->showPublicSureLayer("key_success");
	}
	else
	{
		m_pPublicLayer->showPublicSureLayer("key_error");
	}
}


//请求付费（发送付费信息到服务器）
void HttpInstance::requestRecharge1(std::string strCode)
{
	MessageBox("", "get1");
	log("strCode requestRecharge1=== (%s)", strCode.c_str());
	//创建对象
	cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
	//设置请求地址
	std::string strNet = "http://jsby.dapai2.com:7000/getorder";
	//std::string strNet = "http://www.baidu.com";
	log("strNet == (%s)", strNet.c_str());
	request->setUrl(strNet.c_str());
	//连接类型为POST
	request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
	//请求完的回调函数
	request->setResponseCallback(this, callfuncND_selector(HttpInstance::returnRechargeResult1));
	//请求的数据
	request->setRequestData(strCode.c_str(),strlen(strCode.c_str()));
	//tag
	request->setTag("102");
	//发送请求
	cocos2d::network::HttpClient::getInstance()->send(request);
	//释放发送的数据
	request->release();
}
//返回请求付费的结果
void HttpInstance::returnRechargeResult1(Node *node,void *data)
{
	MessageBox("", "get2");
	log("returnRechargeResult1");
	//接收返回的消息
	cocos2d::network::HttpResponse* pResponse = (cocos2d::network::HttpResponse*)data;
	if (!pResponse)
	{
		return;
	}
	//获取请求的类型
	cocos2d::network::HttpRequest::Type nRequestType = pResponse->getHttpRequest()->getRequestType();
	log("request type %d", nRequestType);

	//请求的标记
	if (0 != strlen(pResponse->getHttpRequest()->getTag())) 
	{
		log("%s ------>oked", pResponse->getHttpRequest()->getTag());
	}

	//返回码statusCode
	int statusCode = pResponse->getResponseCode();
	log("response code: %d", statusCode);
	log("11111111111111111111");
	//不成功就返回
	if (!pResponse->isSucceed()) 
	{
		log("response failed");
		log("error buffer: %s", pResponse->getErrorBuffer());

		m_pPublicLayer->showPublicSureLayer("net_check");
		return;
	}
	log("22222222222222222222");
	//接收返回的信息
	std::vector<char> *pBuffer = pResponse->getResponseData();
	char pArr[100] = "";
	int nSize = pBuffer->size() > 100 ? 100 : pBuffer->size();
	for (int i = 0; i < nSize; i++)
	{
		char c = (*pBuffer)[i];
		if(c != 10)
		{
			int nLen = strlen(pArr);
			pArr[nLen] = (*pBuffer)[i];
		}
	}
	log("333333333333333333");
	log("%s", pArr);
	rapidjson::Document readdoc;
	readdoc.Parse<0>(pArr);

	int nResult = readdoc["result"].GetInt();
	if(nResult == 1)
	{
		std::string strOrder = readdoc["order"].GetString();
		int nChannel = readdoc["payChannel"].GetInt();
		m_nRewardStatus = REWARD_STATUS_WAIT;
		m_strCurOrderString = strOrder;
		m_pPublicLayer->recharge();
	}
	else
	{
		
	}
	
	log("4444444444444444444444444");

}

//请求付费结果（支付结果是否成功）
void HttpInstance::requestRecharge2(std::string strCode)
{
	//MessageBox("", "get3");
	log("returnRechargeResult2");
	//创建对象
	cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
	//设置请求地址
	std::string strNet = "http://jsby.dapai2.com:7000/checkorder";
	request->setUrl(strNet.c_str());
	//连接类型为GET
	request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
	//请求完的回调函数
	request->setResponseCallback(this, callfuncND_selector(HttpInstance::returnRechargeResult2));
	//请求的数据
	request->setRequestData(strCode.c_str(),strlen(strCode.c_str()));
	//tag
	request->setTag("103");
	//发送请求
	cocos2d::network::HttpClient::getInstance()->send(request);
	//释放发送的数据
	request->release();
}
//返回请求付费的结果
void HttpInstance::returnRechargeResult2(Node *node,void *data)
{
	//MessageBox("", "get4");
	//接收返回的消息
	cocos2d::network::HttpResponse* pResponse = (cocos2d::network::HttpResponse*)data;
	if (!pResponse)
	{
		return;
	}
	//获取请求的类型
	cocos2d::network::HttpRequest::Type nRequestType = pResponse->getHttpRequest()->getRequestType();
	log("request type %d", nRequestType);

	//请求的标记
	if (0 != strlen(pResponse->getHttpRequest()->getTag())) 
	{
		log("%s ------>oked", pResponse->getHttpRequest()->getTag());
	}

	//返回码statusCode
	int statusCode = pResponse->getResponseCode();
	log("response code: %d", statusCode);

	//不成功就返回
	if (!pResponse->isSucceed()) 
	{
		log("response failed");
		log("error buffer: %s", pResponse->getErrorBuffer());

		m_pPublicLayer->showPublicSureLayer("net_check");
		return;
	}

	//接收返回的信息
	std::vector<char> *pBuffer = pResponse->getResponseData();
	char pArr[100] = "";
	int nSize = pBuffer->size() > 100 ? 100 : pBuffer->size();
	for (int i = 0; i < nSize; i++)
	{
		char c = (*pBuffer)[i];
		if(c != 10)
		{
			int nLen = strlen(pArr);
			pArr[nLen] = (*pBuffer)[i];
		}
	}
	
	log("%s", pArr);
	rapidjson::Document readdoc;
	readdoc.Parse<0>(pArr);

	int nResult = readdoc["result"].GetInt();
	if(nResult == 1)
	{
		//std::string strOrder = readdoc["order"].GetString();
		int nStatus = readdoc["status"].GetInt();
		if(nStatus == 1)
		{
			log("---------m_nRewardStatus = REWARD_STATUS_COME---------");
			m_nRewardStatus = REWARD_STATUS_COME;
		}
		log("---nStatus-- ==== -----(%d)--", nStatus);
	}
	else
	{
		log("-nResult--error--");
	}
}
