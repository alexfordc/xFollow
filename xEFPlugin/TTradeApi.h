#ifndef TTradeApi_H
#define TTradeApi_H

#include "../xTraderManage/Interface/ITTrade.h"

#include "TTradeSpi.h"

class CTTradeApi : public ITTradeApi
{
public:
	CTTradeApi();
	virtual ~CTTradeApi();

private:
	ITTradeSpi*          m_callback;

	CTTradeSpi           m_spi;
	CThostFtdcTraderApi* m_api;

private:
	virtual void registerSpi(ITTradeSpi* spi);
	virtual void reqUserLogin(x_stuUserLogin& userLogin);
};

#endif // TTradeApi_H
