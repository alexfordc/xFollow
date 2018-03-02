#include "Instrument.h"

#include <string.h>

CInstrument::CInstrument( char marketType, const char* instrumentID, const char* instrumentName, const char* productID, const char* exchangeID )
	: m_marketType(marketType)

	, m_priceTick(0)
	, m_lastPrice(0)
{
	strncpy_s(m_instrumentID, instrumentID, sizeof(m_instrumentID));
	strncpy_s(m_instrumentName, instrumentName, sizeof(m_instrumentName));
	strncpy_s(m_productID, productID, sizeof(m_productID));
	strncpy_s(m_exchangeID, exchangeID, sizeof(m_exchangeID));
}

CInstrument::~CInstrument()
{

}

void CInstrument::setData( double priceTick )
{
	m_priceTick = priceTick;
}

char CInstrument::marketType()
{
	return m_marketType;
}

const char* CInstrument::instrumentID()
{
	return m_instrumentID;
}

const char* CInstrument::instrumentName()
{
	return m_instrumentName;
}

const char* CInstrument::productID()
{
	return m_productID;
}

const char* CInstrument::exchangeID()
{
	return m_exchangeID;
}

double CInstrument::priceTick()
{
	return m_priceTick;
}

void CInstrument::lastPrice( double value )
{
	m_lastPrice = value;
}

double CInstrument::lastPrice()
{
	return m_lastPrice;
}

double CInstrument::bestPrice(bool isBuy)
{
	return m_lastPrice + (isBuy ? 40 * m_priceTick : -40 * m_priceTick);
}
