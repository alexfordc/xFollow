#include "TTradeApi.h"

#include <stdio.h>

ITTradeApi* createTTradeApi()
{
	return new CTTradeApi;
}

void destroyTTradeApi(ITTradeApi* target)
{
	CTTradeApi* api = dynamic_cast<CTTradeApi*>(target);
	assert(api);
	api->clear();
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


void CTTradeApi::clear()
{
	m_api->Release();
	m_api->Join();
}

void CTTradeApi::registerSpi(ITTradeSpi* spi)
{
	m_callback = spi;
	m_spi.registerSpi(spi);
}

void CTTradeApi::reqUserLogin(x_stuUserLogin& userLogin)
{
	if (m_spi.isInited())
	{
		m_spi.sendInitedInfo();
		return;
	}

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
	m_spi.init();
}
