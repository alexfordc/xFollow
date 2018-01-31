#ifndef TraderManage_H
#define TraderManage_H

#include "ITraderManage.h"

#include <map>
#include <string>

class CTraderManage : public ITraderManage
{
public:
	CTraderManage();
	~CTraderManage();
public:

private:
	HANDLE getApiModelByID(int apiID);
	std::map<int, std::string>          m_apiName;
	std::map<int, HANDLE>               m_apiModel;
	ITraderSpi*                         m_spi;

private:
	virtual void registerApi(const char* apiName, int apiID);
	virtual void registerSpi(ITraderSpi* spi);
	virtual void reqUserLogin(int apiID, const char* ip, int port, const char* accountID, const char* password);
};

#endif // TraderManage_H
