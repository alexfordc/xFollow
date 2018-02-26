#include "FTradeSpi.h"


CFTradeSpi::CFTradeSpi()
	: m_id(0)
{

}

CFTradeSpi::~CFTradeSpi()
{

}

void CFTradeSpi::registerID(int id)
{
	m_id = id;
}

void CFTradeSpi::registerSpi(ITraderManageSpi* spi)
{
	m_spi = spi;
}

//////////////////////////////////////////////////////////////////////////
void CFTradeSpi::rspUserLogin(bool successed, int errorID)
{
	m_spi->rspUserLogin(m_id, successed, errorID);
}

void CFTradeSpi::rspUserInitialized(bool successed, int errorID)
{
	m_spi->rspUserInitialized(m_id, successed, errorID);
}


void CFTradeSpi::rtnOrder( int orderIndex, char orderStatus, int volume )
{
	m_spi->rtnOrder(orderIndex, orderStatus, volume);
}

void CFTradeSpi::rtnPositionTotal( const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume )
{
	m_spi->rtnPositionTotal(m_id, productID, instrumentID, isBuy, hedgeFlag, volume);
}
