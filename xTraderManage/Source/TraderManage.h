#ifndef TraderManage_H
#define TraderManage_H

#include "../Interface/ITraderManage.h"

#include <atomic>
#include <map>
#include <string>

class CTraderManage : public ITraderManage
{
public:
	CTraderManage();
	~CTraderManage();
public:

private:
	HMODULE getApiModelByID(int apiID);
	std::map<int, std::string>          m_apiName;
	std::map<int, HMODULE>              m_apiModel;
	std::map<int, std::pair<bool, void*>> m_apis;
	ITraderManageSpi*                   m_spi;

private:
	virtual void registerApi(const char* apiName, int apiID);
	virtual void registerSpi(ITraderManageSpi* spi);
	virtual void reqUserLogin(x_stuUserLogin& userLogin);

	virtual void reqPlaceOrder(int id, const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume);
	virtual void reqCancelOrder(int id);
};

#endif // TraderManage_H
