#include "MarketSpi.h"


CMarketSpi::CMarketSpi()
	: m_marketType('\0')
	, m_spi(nullptr)
{

}

CMarketSpi::~CMarketSpi()
{

}

void CMarketSpi::setMarketType( char type )
{
	m_marketType = type;
}

void CMarketSpi::registerSpi( ITraderManageSpi* spi )
{
	m_spi = spi;
}

void CMarketSpi::rspUserLogin( bool successed, int errorID )
{
	m_spi->rspMUserLogin(m_marketType, successed, errorID);
}

void CMarketSpi::rtnMarketData( char marketType, const char* instrumentID, double lastPrice )
{
	m_spi->rtnMarketData(marketType, instrumentID, lastPrice);
}
