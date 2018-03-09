#ifndef ITraderManage_H
#define ITraderManage_H

#include "X_DllExport.h"
#include "X_Define.h"
#include "ITraderManageSpi.h"

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
	virtual ~ITraderManage() {};

	virtual void registerApi(const char* apiName, int apiID) = 0;
	virtual void registerSpi(ITraderManageSpi* spi) = 0;
	virtual void reqUserLogin(x_stuUserLogin& userLogin) = 0;
	virtual void reqUserLogin(x_stuMUserLogin& userLogin) = 0;

	virtual void reqPlaceOrder(int id, int orderIndex, const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume, double price) = 0;
	virtual void reqCancelOrder(int id, int orderIndex) = 0;
};

#endif // ITraderManage_H
