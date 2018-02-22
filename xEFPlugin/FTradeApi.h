#ifndef FTradeApi_H
#define FTradeApi_H

#include "../xTraderManage/Interface/IFTrade.h"

#include "FTradeSpi.h"

class CFTradeApi : public IFTradeApi
{
public:
	CFTradeApi();
	virtual ~CFTradeApi();

private:
	IFTradeSpi*          m_callback;

	CFTradeSpi           m_spi;
	CThostFtdcTraderApi* m_api;

private:
	virtual void registerSpi(IFTradeSpi* spi);
	virtual void reqUserLogin(x_stuUserLogin& userLogin);

	virtual void reqPlaceOrder(const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume);
	virtual void reqCancelOrder();
};

#endif // FTradeApi_H
