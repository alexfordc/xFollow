#ifndef Strategy_H
#define Strategy_H

#include "../Interface/IStrategy.h"

class CStrategy : public IStrategy
{
public:
	CStrategy(int id);
	virtual ~CStrategy();

public:
	virtual bool isInstrumentValid(const char* productID, const char* instrumentID);
	virtual std::string getStrategy();

public:
	void setData(std::string strategyType, std::string strategyData);

private:
	int              m_id;
	std::string      m_strategyType;
	std::string      m_strategyData;
};

#endif // Strategy_H
