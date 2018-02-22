#ifndef ITTrade_H
#define ITTrade_H

#include "X_DllExport.h"
#include "X_Define.h"

#ifdef	XAPIPLUGIN_EXPORTS
#define XAPIPLUGIN_EXPORTS_C  DLL_EXPORT_CLASS_DECL
#define XAPIPLUGIN_EXPORTS_A  DLL_EXPORT_C_DECL
#else
#define XAPIPLUGIN_EXPORTS_C  DLL_IMPORT_CLASS_DECL
#define XAPIPLUGIN_EXPORTS_A  DLL_IMPORT_C_DECL
#endif


class ITTradeSpi
{
public:
	virtual void rspUserLogin(bool successed, int errorID) = 0;
	virtual void rspUserInitialized(bool successed, int errorID) = 0;

	virtual void rtnTrade(const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume) = 0;
	virtual void rtnPositionTotal(const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume) = 0;
};

class ITTradeApi
{
public:
	virtual ~ITTradeApi() {}

	virtual void registerSpi(ITTradeSpi* spi) = 0;
	virtual void reqUserLogin(x_stuUserLogin& userLogin) = 0;

	virtual void reqPlaceOrder(const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume) = 0;
	virtual void reqCancelOrder() = 0;
};

XAPIPLUGIN_EXPORTS_A ITTradeApi* createTTradeApi();
XAPIPLUGIN_EXPORTS_A void destroyTTradeApi(ITTradeApi* target);

#endif // ITTrade_H
