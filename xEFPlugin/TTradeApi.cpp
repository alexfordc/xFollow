#include "TTradeApi.h"

#include <stdio.h>

ITTradeApi* createTTradeApi()
{
	return new CTTradeApi;
}

void destroyTTradeApi(ITTradeApi* target)
{
	delete target;
}

//////////////////////////////////////////////////////////////////////////
CTTradeApi::CTTradeApi()
	: m_callback(nullptr)
	, m_api(nullptr)
{

}

CTTradeApi::~CTTradeApi()
{

}

void CTTradeApi::registerSpi(ITTradeSpi* spi)
{
	m_callback = spi;
	m_spi.registerSpi(spi);
}

void CTTradeApi::reqUserLogin(x_stuUserLogin& userLogin)
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

void CTTradeApi::reqPlaceOrder(const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume)
{
	m_spi.reqPlaceOrder(productID, instrumentID, isBuy, isOpen, hedgeFlag, volume);
}

void CTTradeApi::reqCancelOrder()
{
	m_spi.reqCancelOrder();
}
