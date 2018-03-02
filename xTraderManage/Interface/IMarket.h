#ifndef IMarket_H
#define IMarket_H

#include "X_DllExport.h"
#include "X_Define.h"

#ifdef	XAPIPLUGIN_EXPORTS
#define XAPIPLUGIN_EXPORTS_C  DLL_EXPORT_CLASS_DECL
#define XAPIPLUGIN_EXPORTS_A  DLL_EXPORT_C_DECL
#else
#define XAPIPLUGIN_EXPORTS_C  DLL_IMPORT_CLASS_DECL
#define XAPIPLUGIN_EXPORTS_A  DLL_IMPORT_C_DECL
#endif

class IMarketSpi
{
public:
	virtual void rspUserLogin(bool successed, int errorID) = 0;

	virtual void rtnMarketData(char marketType, const char* instrumentID, double lastPrice) = 0;
};

class XAPIPLUGIN_EXPORTS_C IMarketApi
{
public:
	virtual ~IMarketApi() {}

	virtual void registerSpi(IMarketSpi* spi) = 0;
	virtual void setSubscribeInstruments(std::list<std::string> instrumentIDs) = 0;
	virtual void reqUserLogin(x_stuMUserLogin& userLogin) = 0;
};

XAPIPLUGIN_EXPORTS_A IMarketApi* createMarketApi();
XAPIPLUGIN_EXPORTS_A void destroyMarketApi(IMarketApi* api);

#endif // IMarket_H
