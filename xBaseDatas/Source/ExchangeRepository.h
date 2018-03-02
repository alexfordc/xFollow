#ifndef ExchangeRepository_H
#define ExchangeRepository_H

#include <map>
#include <string>

#include "../Interface/IBaseDataRepository.h"

class CExchangeRepository : public IExchangeRepository
{
public:
	static CExchangeRepository& exchangeRepository();
	virtual ~CExchangeRepository();

	virtual void setExchange(char marketType, const char* exchangeID, const char* exchangeName);
	virtual IExchange* getExchange(char marketType, const char* exchangeID);

	virtual void clear();
private:
	CExchangeRepository();

	static CExchangeRepository*            s_instance;

	// marketType+'|'+exchangeID <==> CExchange
	std::map<std::string, IExchange*>      m_exchanges;
};

#endif // ExchangeRepository_H
