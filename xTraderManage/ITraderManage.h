#ifndef ITraderManage_H
#define ITraderManage_H

#include "../Include/X_DllExport.h"
#include "ITraderSpi.h"

class EXPORTS_CLASS ITraderManage
{
public:
	static ITraderManage* createTraderManage();

	virtual void registerApi(const char* apiName, int apiID) = 0;
	virtual void registerSpi(ITraderSpi* spi) = 0;
	virtual void reqUserLogin(int apiID, const char* ip, int port, const char* accountID, const char* password) = 0;

};

#endif // ITraderManage_H
