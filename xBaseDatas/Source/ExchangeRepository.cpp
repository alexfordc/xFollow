#include "ExchangeRepository.h"

#include "Exchange.h"

IExchangeRepository& IExchangeRepository::exchangeRepository()
{
	return CExchangeRepository::exchangeRepository();
}
//////////////////////////////////////////////////////////////////////////
CExchangeRepository* CExchangeRepository::s_instance = nullptr;

CExchangeRepository& CExchangeRepository::exchangeRepository()
{
	if (s_instance == nullptr)
	{
		s_instance = new CExchangeRepository;
	}
	return *s_instance;
}

CExchangeRepository::CExchangeRepository()
{

}

CExchangeRepository::~CExchangeRepository()
{

}

void CExchangeRepository::setExchange( char marketType, const char* exchangeID, const char* exchangeName )
{
	std::string key = &marketType;
	key += "|";
	key += exchangeID;
	auto it = m_exchanges.find(key);
	if (it == m_exchanges.end())
	{
		IExchange* exchange = new CExchange(marketType, exchangeID, exchangeName);
		m_exchanges[key] = exchange;
	}
}

IExchange* CExchangeRepository::getExchange( char marketType, const char* exchangeID )
{
	std::string key = &marketType;
	key += "|";
	key += exchangeID;
	auto it = m_exchanges.find(key);
	return it == m_exchanges.end() ? nullptr : it->second;
}

void CExchangeRepository::clear()
{
	for (auto& e : m_exchanges)
	{
		if (e.second != nullptr)
			delete e.second;
	}
	m_exchanges.clear();
}
