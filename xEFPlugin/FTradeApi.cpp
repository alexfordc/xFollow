#include "FTradeApi.h"

#include <stdio.h>

IFTradeApi* createFTradeApi()
{
	return new CFTradeApi;
}

void destroyFTradeApi(IFTradeApi* target)
{
	delete target;
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

void CFTradeApi::registerSpi(IFTradeSpi* spi)
{
	m_callback = spi;
	m_spi.registerSpi(spi);
}

void CFTradeApi::reqUserLogin(x_stuUserLogin& userLogin)
{
	if (nullptr == m_api)
		m_api = CThostFtdcTraderApi::CreateFtdcTraderApi();

	m_spi.setUserInfo(userLogin.accountID, userLogin.password);
	m_spi.registerApi(m_api);

	char address[32] = {0};
	sprintf_s(address, "tcp://%s:%d", userLogin.ip1, userLogin.port1);
	m_api->RegisterFront(address);
	sprintf_s(address, "tcp://%s:%d", userLogin.ip2, userLogin.port2);
	m_api->RegisterFront(address);
	sprintf_s(address, "tcp://%s:%d", userLogin.ip3, userLogin.port3);
	m_api->RegisterFront(address);
	m_api->RegisterSpi(&m_spi);
	m_api->Init();
}

void CFTradeApi::reqPlaceOrder(const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume)
{
	m_spi.reqPlaceOrder(productID, instrumentID, isBuy, isOpen, hedgeFlag, volume);
}

void CFTradeApi::reqCancelOrder()
{
	m_spi.reqCancelOrder();
}
