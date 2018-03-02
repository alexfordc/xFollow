#ifndef MarketSpi_H
#define MarketSpi_H

#include "../Interface/IMarket.h"
#include "../Interface/ITraderManageSpi.h"

class CMarketSpi : public IMarketSpi
{
public:
	CMarketSpi();
	virtual ~CMarketSpi();

public:
	void setMarketType(char type);
	void registerSpi(ITraderManageSpi* spi);

private:
	char              m_marketType;
	ITraderManageSpi* m_spi;

private:
	virtual void rspUserLogin(bool successed, int errorID);

	virtual void rtnMarketData(char marketType, const char* instrumentID, double lastPrice);
};

#endif // MarketSpi_H
