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

	virtual void reqPlaceOrder(const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume);
	virtual void reqCancelOrder();
};

#endif // TTradeApi_H
