#include "TTradeSpi.h"


CTTradeSpi::CTTradeSpi()
	: m_id(0)
{

}

CTTradeSpi::~CTTradeSpi()
{

}

void CTTradeSpi::registerID(int id)
{
	m_id = id;
}

void CTTradeSpi::registerSpi(ITraderManageSpi* spi)
{
	m_spi = spi;
}

//////////////////////////////////////////////////////////////////////////
void CTTradeSpi::rspUserLogin(bool successed, int errorID)
{
	m_spi->rspUserLogin(m_id, successed, errorID);
}

void CTTradeSpi::rspUserInitialized(bool successed, int errorID)
{
	m_spi->rspUserInitialized(m_id, successed, errorID);
}

void CTTradeSpi::rtnTrade( const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume )
{
	m_spi->rtnTrade(m_id, productID, instrumentID, isBuy, isOpen, hedgeFlag, volume);
}

void CTTradeSpi::rtnPositionTotal( const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume )
{
	m_spi->rtnPositionTotal(m_id, productID, instrumentID, isBuy, hedgeFlag, volume);
}
