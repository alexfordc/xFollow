#include "TradeSpi.h"


CTradeSpi::CTradeSpi()
	: m_id(0)
{

}

CTradeSpi::~CTradeSpi()
{

}

void CTradeSpi::registerID(int id)
{
	m_id = id;
}

void CTradeSpi::registerSpi(ITraderManageSpi* spi)
{
	m_spi = spi;
}

//////////////////////////////////////////////////////////////////////////
void CTradeSpi::rspUserLogin(bool successed, int errorID)
{
	m_spi->rspUserLogin(m_id, successed, errorID);
}

void CTradeSpi::rspUserInitialized(bool successed, int errorID)
{
	m_spi->rspUserInitialized(m_id, successed, errorID);
}

void CTradeSpi::rtnTrade( const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume )
{
	m_spi->rtnTrade(m_id, instrumentID, isBuy, isOpen, hedgeFlag, volume);
}

void CTradeSpi::rtnPositionTotal( const char* instrumentID, bool isBuy, char hedgeFlag, int volume )
{
	m_spi->rtnPositionTotal(m_id, instrumentID, isBuy, hedgeFlag, volume);
}
