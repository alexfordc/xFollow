#ifndef ITraderManage_H
#define ITraderManage_H

#include "X_DllExport.h"
#include "ITraderSpi.h"

#ifdef	XTRADERMANAGE_EXPORTS
#define XTRADERMANAGE_EXPORTS_C  DLL_EXPORT_CLASS_DECL
#define XTRADERMANAGE_EXPORTS_A  DLL_EXPORT_C_DECL
#else
#define XTRADERMANAGE_EXPORTS_C  DLL_IMPORT_CLASS_DECL
#define XTRADERMANAGE_EXPORTS_A  DLL_IMPORT_C_DECL
#endif

class XTRADERMANAGE_EXPORTS_C ITraderManage
{
public:
	static ITraderManage* createTraderManage();
	static void           destroyTraderManage(ITraderManage* target);

	virtual void registerApi(const char* apiName, int apiID) = 0;
	virtual void registerSpi(ITraderSpi* spi) = 0;
	virtual void reqUserLogin(int apiID, const char* ip, int port, const char* accountID, const char* password) = 0;

};

#endif // ITraderManage_H
