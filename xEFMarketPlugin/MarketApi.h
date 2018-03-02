#ifndef MarketApi_H
#define MarketApi_H

#include "MarketSpi.h"

class CMarketApi : public IMarketApi
{
public:
	CMarketApi();
	virtual ~CMarketApi();

	virtual void registerSpi(IMarketSpi* spi);
	virtual void setSubscribeInstruments(std::list<std::string> instrumentIDs);
	virtual void reqUserLogin(x_stuMUserLogin& userLogin);

private:
	IMarketSpi*          m_callback;

	CThostFtdcMdApi*     m_api;
	CMarketSpi           m_spi;
};

#endif // MarketApi_H
