#ifndef TradeApi_H
#define TradeApi_H

#include "../xTraderManage/Interface/ITrade.h"

class CTradeApi : public ITradeApi
{
public:
	CTradeApi();
	virtual ~CTradeApi();

private:
	ITradeSpi*       m_spi;

private:
	virtual void registerSpi(ITradeSpi* spi);
	virtual void reqUserLogin(const char* ip, int port, const char* accountID, const char* password);

	virtual void reqPlaceOrder(const char* instrumentID, char direction, char offerset, char hedgeFlag, int volume);
	virtual void reqCancelOrder();
};

#endif // TradeApi_H
