#include "Exchange.h"

#include <string.h>

CExchange::CExchange( char marketType, const char* exchangeID, const char* exchangeName )
	: m_marketType(marketType)
{
	strncpy_s(m_exchangeID, exchangeID, sizeof(m_exchangeID));
	strncpy_s(m_exchangeName, exchangeName, sizeof(m_exchangeName));
}

CExchange::~CExchange()
{

}

char CExchange::marketType()
{
	return m_marketType;
}

const char* CExchange::exchangeID()
{
	return m_exchangeID;
}

const char* CExchange::exchangeName()
{
	return m_exchangeName;
}
