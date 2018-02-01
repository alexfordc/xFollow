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
