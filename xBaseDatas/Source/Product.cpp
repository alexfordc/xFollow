#include "Product.h"

#include <string.h>

CProduct::CProduct( char marketType, const char* productID, const char* productName, const char* exchangeID )
	: m_marketType(marketType)
{
	strncpy_s(m_productID, productID, sizeof(m_productID));
	strncpy_s(m_productName, productName, sizeof(m_productName));
	strncpy_s(m_exchangeID, exchangeID, sizeof(m_exchangeID));
}

CProduct::~CProduct()
{

}

char CProduct::marketType()
{
	return m_marketType;
}

const char* CProduct::productID()
{
	return m_productID;
}

const char* CProduct::productName()
{
	return m_productName;
}

const char* CProduct::exchangeID()
{
	return m_exchangeID;
}
