#ifndef IFTrade_H
#define IFTrade_H

#include "X_DllExport.h"
#include "X_Define.h"

#ifdef	XAPIPLUGIN_EXPORTS
#define XAPIPLUGIN_EXPORTS_C  DLL_EXPORT_CLASS_DECL
#define XAPIPLUGIN_EXPORTS_A  DLL_EXPORT_C_DECL
#else
#define XAPIPLUGIN_EXPORTS_C  DLL_IMPORT_CLASS_DECL
#define XAPIPLUGIN_EXPORTS_A  DLL_IMPORT_C_DECL
#endif


class IFTradeSpi
{
public:
	virtual void rspUserLogin(bool successed, int errorID) = 0;
	virtual void rspUserInitialized(bool successed, int errorID) = 0;

	virtual void rtnOrder(int orderIndex, char orderStatus, int volume) = 0;
	virtual void rtnPositionTotal(const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume) = 0;
};

class IFTradeApi
{
public:
	virtual ~IFTradeApi() {}

	virtual void registerSpi(IFTradeSpi* spi) = 0;
	virtual void reqUserLogin(x_stuUserLogin& userLogin) = 0;

	virtual void reqPlaceOrder(int orderIndex, const char* productID, const char* instrumentID, bool isBuy, bool isOpen, 
		char hedgeFlag, int volume, double price) = 0;
	virtual void reqCancelOrder(int orderIndex) = 0;
};

XAPIPLUGIN_EXPORTS_A IFTradeApi* createFTradeApi();
XAPIPLUGIN_EXPORTS_A void destroyFTradeApi(IFTradeApi* target);

#endif // IFTrade_H
