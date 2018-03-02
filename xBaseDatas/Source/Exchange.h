#ifndef Exchange_H
#define Exchange_H

#include "../Interface/IBaseDataRepository.h"

class CExchange : public IExchange
{
public:
	CExchange(char marketType, const char* exchangeID, const char* exchangeName);
	virtual ~CExchange();

	virtual char marketType();
	virtual const char* exchangeID();
	virtual const char* exchangeName();

private:
	char m_marketType;
	char m_exchangeID[32];
	char m_exchangeName[32];
};

#endif // Exchange_H
