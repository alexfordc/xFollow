#include "TradeApi.h"


ITradeApi* createTradeApi()
{
	return new CTradeApi;
}

void destroyTradeApi(ITradeApi* target)
{
	delete target;
}

//////////////////////////////////////////////////////////////////////////
CTradeApi::CTradeApi()
	: m_spi(nullptr)
{

}

CTradeApi::~CTradeApi()
{

}

void CTradeApi::registerSpi(ITradeSpi* spi)
{
	m_spi = spi;
}

void CTradeApi::reqUserLogin(const char* ip, int port, const char* accountID, const char* password)
{
	m_spi->rspUserLogin(true, 0);
	m_spi->rspUserInitialized(true, 0);

	m_spi->rtnTrade("CL", "1803", true, true, '1', 5);
	m_spi->rtnPositionTotal("CL", "1803", true, '1', 5);
}

void CTradeApi::reqPlaceOrder(const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume)
{

}

void CTradeApi::reqCancelOrder()
{

}
