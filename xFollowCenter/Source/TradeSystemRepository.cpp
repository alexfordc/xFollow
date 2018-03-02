#include "TradeSystemRepository.h"

#include "TradeSystem.h"

CTradeSystemRepository* CTradeSystemRepository::s_instance = nullptr;

CTradeSystemRepository& CTradeSystemRepository::tradeSystemRepository()
{
	if (nullptr == s_instance)
	{
		s_instance = new CTradeSystemRepository;
	}
	return *s_instance;
}

CTradeSystem* CTradeSystemRepository::createTradeSystem( int id, std::string name, int api_ID, 
														std::string ip1, int port1, std::string ip2, int port2, std::string ip3, int port3, char status, int org_ID )
{
	CTradeSystem* tradeSystem = nullptr;
	auto it = m_tradeSystems.find(id);
	if (it == m_tradeSystems.end())
	{
		tradeSystem = new CTradeSystem(id);
		m_tradeSystems[id] = tradeSystem;
	}
	else
	{
		tradeSystem = it->second;
	}
	tradeSystem->setData(name, api_ID, ip1, port1, ip2, port2, ip3, port3);
	tradeSystem->setStatus(status);
	tradeSystem->setOrgID(org_ID);
	return tradeSystem;
}

CTradeSystem* CTradeSystemRepository::getTradeSystem( int id )
{
	CTradeSystem* tradeSystem = nullptr;
	auto it = m_tradeSystems.find(id);
	if (it != m_tradeSystems.end())
	{
		tradeSystem = it->second;
	}
	return tradeSystem;
}

void CTradeSystemRepository::clear()
{
	for (auto& t : m_tradeSystems)
	{
		if (t.second != nullptr)
			delete t.second;
	}
	m_tradeSystems.clear();
}
