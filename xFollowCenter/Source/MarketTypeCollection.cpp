#include "MarketTypeCollection.h"


CMarketTypeCollection* CMarketTypeCollection::s_instance = nullptr;

CMarketTypeCollection& CMarketTypeCollection::marketTypeCollection()
{
	if (nullptr == s_instance)
	{
		s_instance = new CMarketTypeCollection;
	}
	return *s_instance;
}

CMarketTypeCollection::CMarketTypeCollection()
{

}

CMarketTypeCollection::~CMarketTypeCollection()
{
	clear();
}

void CMarketTypeCollection::setData( int api_ID, char marketType, char type )
{
	m_marketTypes[api_ID] = std::make_pair(marketType, type);
}

char CMarketTypeCollection::marketType( int api_ID )
{
	auto it = m_marketTypes.find(api_ID);
	return (it == m_marketTypes.end()) ? '\0' : it->second.first;
}

void CMarketTypeCollection::clear()
{
	m_marketTypes.clear();
}
