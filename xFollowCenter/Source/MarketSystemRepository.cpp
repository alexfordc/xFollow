#include "MarketSystemRepository.h"

#include "MarketSystem.h"

CMarketSystemRepository* CMarketSystemRepository::s_instance = nullptr;

CMarketSystemRepository& CMarketSystemRepository::marketSystemRepository()
{
	if (nullptr == s_instance)
	{
		s_instance = new CMarketSystemRepository;
	}
	return *s_instance;
}

CMarketSystem* CMarketSystemRepository::createMarketSystem( int id, std::string name, int api_ID, std::string accountID, std::string password, 
														std::string ip1, int port1, std::string ip2, int port2, std::string ip3, int port3, char status )
{
	CMarketSystem* marketSystem = nullptr;
	auto it = m_marketSystems.find(id);
	if (it == m_marketSystems.end())
	{
		marketSystem = new CMarketSystem(id);
		m_marketSystems[id] = marketSystem;
	}
	else
	{
		marketSystem = it->second;
	}
	marketSystem->setData(name, api_ID, accountID, password, ip1, port1, ip2, port2, ip3, port3);
	marketSystem->setStatus(status);
	return marketSystem;
}

CMarketSystem* CMarketSystemRepository::getMarketSystem( int id )
{
	CMarketSystem* marketSystem = nullptr;
	auto it = m_marketSystems.find(id);
	if (it != m_marketSystems.end())
	{
		marketSystem = it->second;
	}
	return marketSystem;
}

std::list<CMarketSystem*> CMarketSystemRepository::getMarketSystems()
{
	std::list<CMarketSystem*> marketSystems;
	for (auto& m : m_marketSystems)
	{
		marketSystems.push_back(m.second);
	}
	return marketSystems;
}

void CMarketSystemRepository::clear()
{
	for (auto& m : m_marketSystems)
	{
		if (m.second != nullptr)
			delete m.second;
	}
	m_marketSystems.clear();
}
