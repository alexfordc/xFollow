#include "MarketApi.h"

#include <stdio.h>

IMarketApi* createMarketApi()
{
	return new CMarketApi;
}

void destroyMarketApi( IMarketApi* api )
{
	delete api;
}
//////////////////////////////////////////////////////////////////////////
CMarketApi::CMarketApi()
	: m_callback(nullptr)
	, m_api(nullptr)
{

}

CMarketApi::~CMarketApi()
{

}

void CMarketApi::registerSpi( IMarketSpi* spi )
{
	m_callback = spi;
	m_spi.registerSpi(spi);
}

void CMarketApi::setSubscribeInstruments( std::list<std::string> instrumentIDs )
{
	m_spi.setSubscribeInstruments(instrumentIDs);
}

void CMarketApi::reqUserLogin( x_stuMUserLogin& userLogin )
{
	if (nullptr == m_api)
		m_api = CThostFtdcMdApi::CreateFtdcMdApi();

	m_spi.setUserInfo(userLogin.marketType, userLogin.brokerID, userLogin.accountID, userLogin.password);
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
