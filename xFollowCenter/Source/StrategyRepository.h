#ifndef StrategyRepository_H
#define StrategyRepository_H

#include <map>
#include <string>

class CStrategy;

class CStrategyRepository
{
public:
	static CStrategyRepository& strategyRepository();

	CStrategy* createStrategy(int id, std::string strategyType, std::string strategyData);
	CStrategy* getStrategy(int id);

	void clear();

private:
	static CStrategyRepository*      s_strategyRepository;

	std::map<int, CStrategy*>        m_strategys;
};

#endif // StrategyRepository_H
