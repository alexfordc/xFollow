#include "Strategy.h"


CStrategy::CStrategy( int id )
	: m_id(id)
{

}

CStrategy::~CStrategy()
{

}

std::string CStrategy::getStrategy()
{
	return m_strategyData;
}

void CStrategy::setData( std::string strategyType, std::string strategyData )
{
	m_strategyType = strategyType;
	m_strategyData = m_strategyData;
}
