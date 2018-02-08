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

CTradeSystem* CTradeSystemRepository::createTradeSystem( int id, std::string name, 
														std::string ip1, int port1, std::string ip2, int port2, std::string ip3, int port3, char status )
{
	CTradeSystem* tradeSystem = nullptr;
	auto it = m_tradeSystems.find(id);
	if (it == m_tradeSystems.end())
	{
		tradeSystem = new CTradeSystem(id);
	}
	else
	{
		tradeSystem = it->second;
	}
	tradeSystem->setData(name, ip1, port1, ip2, port2, ip3, port3);
	tradeSystem->setStatus(status);
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
