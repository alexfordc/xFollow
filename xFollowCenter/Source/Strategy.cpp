#include "Strategy.h"


CStrategy::CStrategy( int id )
	: m_id(id)
{

}

CStrategy::~CStrategy()
{

}

bool CStrategy::isInstrumentValid(const char* productID, const char* instrumentID)
{
/*
	auto it = m_authProducts.find(productID);
	if (it == m_authProducts.end())
	{
		std::string insID = productID;
		insID += instrumentID;
		it = m_authProducts.find(insID);
		if (it == m_authProducts.end())
		{
			return; // ²»¸ú
		}
	}
*/
	return true;
}

std::string CStrategy::getStrategy()
{
	return m_strategyData;
}

std::string CStrategy::strategyType()
{
	return m_strategyType;
}

void CStrategy::setData( std::string strategyType, std::string strategyData )
{
	m_strategyType = strategyType;
	m_strategyData = m_strategyData;
}
