#include "Store.h"
#include "HttpInstance.h"
#include "UserData.h"
#include "NDKHelper/NDKHelper.h"
#include "Manager.h"

StoreManager::StoreManager(string group_name)
: m_sGroupName("DefaultGroupName")
{
	int nSize = NDKHelper::selectorList.size();
	log("------NDKHelper::selectorList.size()------===---(%d)------", nSize);
	NDKHelper::selectorList.clear();
	//NDKHelper::RemoveSelectorsInGroup("DiamondStoreLayer");
	m_sGroupName = group_name;
	log("StoreManager::StoreManager Group Name is %s", group_name.c_str());
}

StoreManager::~StoreManager()
{
	log("StoreManager::~StoreManager %s", m_sGroupName.c_str());
    NDKHelper::RemoveSelectorsInGroup(m_sGroupName.c_str());
}

//调用JAVA内的支付函数
// callbackTag  用于在NDKHelper中
// fake_code    购买道具的编号
// target       回调的目标
// callback     回调的方法
void StoreManager::Purchase(string callbackTag, int fake_code, Node* target, SEL_CallFuncND callback) //购买道具的类型 由他调用 java
{
	HttpInstance::getInstance()->setCallBackIndex(HttpInstance::getInstance()->getCallBackIndex() + 1);
	std::string callback_tag = "Purchase" + getStringFromInt(HttpInstance::getInstance()->getCallBackIndex());
	
    //这里建立同支付接口之间的关系
    NDKHelper::AddSelector(m_sGroupName.c_str(),
                           callback_tag.c_str(),
                           callback,
                           target);
    
	int bFirst = UserData::getInstance()->getFirstShopping();
	int nVip = UserData::getInstance()->getCurVipLevel();
	
	int nPayWays = HttpInstance::getInstance()->getPayType();
	std::string order = HttpInstance::getInstance()->getCurOrderString();
	log("-----c++--order--==-(%s)-", order.c_str());

	std::string pOrder= HttpInstance::getInstance()->getCurOrderString().c_str();
	
    //参数
    __Dictionary* params = __Dictionary::create();
    //第二个参数是key 第一个参数是value
    params->setObject(__String::createWithFormat("%d", fake_code), "fake_code");
	params->setObject(__String::create(callback_tag.c_str()), "callback_tag");
    params->setObject(__String::createWithFormat("%d", bFirst), "first");
	params->setObject(__String::createWithFormat("%d", nVip), "vip");
	params->setObject(__String::createWithFormat("%d", nPayWays), "pay_way");
	params->setObject(__String::createWithFormat("%s", pOrder.c_str()), "order");
    
    //调用(方法名为第一个参数，这个方法名是JAVA里面的方法名)，并传递参数(第二个参数)
    SendMessageWithParams(string("ProcessPurchaseRequest"), params);
}


void StoreManager::RequestOrder(string words, int fake_code, Node* target, SEL_CallFuncND callback) //购买道具的类型 由他调用 java
{
	HttpInstance::getInstance()->setCallBackIndex(HttpInstance::getInstance()->getCallBackIndex() + 1);
	std::string callback_tag = "Purchase" + getStringFromInt(HttpInstance::getInstance()->getCallBackIndex());
	//这里建立同支付接口之间的关系
	NDKHelper::AddSelector(m_sGroupName.c_str(),
		callback_tag.c_str(),
		callback,
		target);


	//参数
	__Dictionary* params = __Dictionary::create();
	//第二个参数是key 第一个参数是value
	params->setObject(__String::createWithFormat("%d", fake_code), "fake_code");
	params->setObject(__String::create(callback_tag.c_str()), "callback_tag");
	params->setObject(__String::create(words.c_str()), "words");

	//调用(方法名为第一个参数，这个方法名是JAVA里面的方法名)，并传递参数(第二个参数)
	SendMessageWithParams(string("RequestOrder"), params);
}

//核对订单
void StoreManager::CheckOrder(string words, int fake_code, Node* target, SEL_CallFuncND callback)
{

}

//网络检查
void StoreManager::getDeviceInfo(string callbackTag, int fake_code, Node* target, SEL_CallFuncND callback)
{
	log("get getDeviceInfo in c++");
	//这里建立同支付接口之间的关系
	NDKHelper::AddSelector(m_sGroupName.c_str(),
		callbackTag.c_str(),
		callback,
		target);

	//参数
	__Dictionary* params = __Dictionary::create();
	//第二个参数是key 第一个参数是value
	params->setObject(__String::createWithFormat("%d", fake_code), "fake_code");
	params->setObject(__String::create(callbackTag), "callback_tag1");

	//调用(方法名为第一个参数，这个方法名是JAVA里面的方法名)，并传递参数(第二个参数)
	SendMessageWithParams(string("GetDeviceInfomation"), params);
}

//退出游戏
void StoreManager::exit(string callbackTag, int fake_code, Node* target, SEL_CallFuncND callback)
{
	//这里建立同支付接口之间的关系
	NDKHelper::AddSelector(m_sGroupName.c_str(),
		callbackTag.c_str(),
		callback,
		target);

	//参数
	__Dictionary* params = __Dictionary::create();
	//第二个参数是key 第一个参数是value
	params->setObject(__String::createWithFormat("%d", fake_code), "fake_code");
	params->setObject(__String::create(callbackTag), "callback_tag");

	//调用(方法名为第一个参数，这个方法名是JAVA里面的方法名)，并传递参数(第二个参数)
	SendMessageWithParams(string("dialog"), params);
}