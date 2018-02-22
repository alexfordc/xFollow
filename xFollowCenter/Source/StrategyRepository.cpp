#include "StrategyRepository.h"

#include "Strategy.h"

CStrategyRepository* CStrategyRepository::s_strategyRepository = nullptr;

CStrategyRepository& CStrategyRepository::strategyRepository()
{
	if (nullptr == s_strategyRepository)
	{
		s_strategyRepository = new CStrategyRepository;
	}
	return *s_strategyRepository;
}

CStrategy* CStrategyRepository::createStrategy( int id, std::string strategyType, std::string strategyData )
{
	CStrategy* strategy = nullptr;
	auto it = m_strategys.find(id);
	if (it == m_strategys.end())
	{
		strategy = new CStrategy(id);
		m_strategys[id] = strategy;
	}
	else
	{
		strategy = it->second;
	}
	if (nullptr != strategy)
		strategy->setData(strategyType, strategyData);
	return strategy;
}

CStrategy* CStrategyRepository::getStrategy( int id )
{
	CStrategy* strategy = nullptr;
	auto it = m_strategys.find(id);
	if (it != m_strategys.end())
	{
		strategy = it->second;
	}
	return strategy;
}
