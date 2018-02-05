#ifndef ITrade_H
#define ITrade_H

#include "X_DllExport.h"

#ifdef	XCTPPLUGIN_EXPORTS
#define XCTPPLUGIN_EXPORTS_C  DLL_EXPORT_CLASS_DECL
#define XCTPPLUGIN_EXPORTS_A  DLL_EXPORT_C_DECL
#else
#define XCTPPLUGIN_EXPORTS_C  DLL_IMPORT_CLASS_DECL
#define XCTPPLUGIN_EXPORTS_A  DLL_IMPORT_C_DECL
#endif


class ITradeSpi
{
public:
	virtual void rspUserLogin(bool successed, int errorID) = 0;
	virtual void rspUserInitialized(bool successed, int errorID) = 0;

	virtual void rtnTrade(const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume) = 0;
	virtual void rtnPositionTotal(const char* instrumentID, bool isBuy, char hedgeFlag, int volume) = 0;
};

class ITradeApi
{
public:
	virtual ~ITradeApi() {}

	virtual void registerSpi(ITradeSpi* spi) = 0;
	virtual void reqUserLogin(const char* ip, int port, const char* accountID, const char* password) = 0;

	virtual void reqPlaceOrder(const char* instrumentID, char direction, char offerset, char hedgeFlag, int volume) = 0;
	virtual void reqCancelOrder() = 0;
};

XCTPPLUGIN_EXPORTS_A ITradeApi* createTradeApi();
XCTPPLUGIN_EXPORTS_A void destroyTradeApi(ITradeApi* target);

#endif // ITrade_H
