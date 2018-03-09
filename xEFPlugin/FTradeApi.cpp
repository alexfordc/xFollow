#include "FTradeApi.h"

#include <stdio.h>

IFTradeApi* createFTradeApi()
{
	return new CFTradeApi;
}

void destroyFTradeApi(IFTradeApi* target)
{
	CFTradeApi* api = dynamic_cast<CFTradeApi*>(target);
	assert(api);
	api->clear();
	delete api;
}

//////////////////////////////////////////////////////////////////////////
CFTradeApi::CFTradeApi()
	: m_callback(nullptr)
	, m_api(nullptr)
{

}

CFTradeApi::~CFTradeApi()
{

}


void CFTradeApi::clear()
{
	m_api->Release();
	m_api->Join();
}

void CFTradeApi::registerSpi(IFTradeSpi* spi)
{
	m_callback = spi;
	m_spi.registerSpi(spi);
}

void CFTradeApi::reqUserLogin(x_stuUserLogin& userLogin)
{
	if (m_spi.isInited())
	{
		m_spi.sendInitedInfo();
		return;
	}

	if (nullptr == m_api)
		m_api = CThostFtdcTraderApi::CreateFtdcTraderApi();

	m_spi.setUserInfo(userLogin.brokerID, userLogin.accountID, userLogin.password);
	m_spi.registerApi(m_api);

	char address[32] = {0};
	if (userLogin.ip1[0] != '\0' && userLogin.port1 != 0)
	{
		sprintf_s(address, "tcp://%s:%d", userLogin.ip1, userLogin.port1);
		m_api->RegisterFront(address);
	}
	if (userLogin.ip2[0] != '\0' && userLogin.port2 != 0)
	{
		sprintf_s(address, "tcp://%s:%d", userLogin.ip2, userLogin.port2);
		m_api->RegisterFront(address);
	}
	if (userLogin.ip3[0] != '\0' && userLogin.port3 != 0)
	{
		sprintf_s(address, "tcp://%s:%d", userLogin.ip3, userLogin.port3);
		m_api->RegisterFront(address);
	}
	m_api->RegisterSpi(&m_spi);
	m_spi.init();
}

void CFTradeApi::reqPlaceOrder(int orderIndex, const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume, double price)
{
	m_spi.reqPlaceOrder(orderIndex, productID, instrumentID, isBuy, isOpen, hedgeFlag, volume, price);
}

void CFTradeApi::reqCancelOrder(int orderIndex)
{
	m_spi.reqCancelOrder(orderIndex);
}
