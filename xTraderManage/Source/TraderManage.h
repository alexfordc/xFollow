#ifndef TraderManage_H
#define TraderManage_H

#include "../Interface/ITraderManage.h"

#include <atomic>
#include <list>
#include <map>
#include <string>

class IMarketApi;
class CMarketSpi;

class IFTradeApi;
class ITTradeApi;
class CFTradeSpi;
class CTTradeSpi;

class CTraderManage : public ITraderManage
{
public:
	CTraderManage();
	~CTraderManage();
public:
	void clear();

private:
	HMODULE getApiModelByID(int apiID);
	std::map<int, std::string>          m_apiName;
	std::map<int, HMODULE>              m_apiModel;
	std::map<int, std::pair<bool, void*>> m_apis;

	typedef void (*DESTROYMARKETAPI)(IMarketApi* api);
	std::map<DESTROYMARKETAPI, IMarketApi*> m_marketApis;
	std::list<CMarketSpi*>              m_marketSpis;

	typedef void (*DESTROYFTRADEAPI)(IFTradeApi* api);
	typedef void (*DESTROYTTRADEAPI)(ITTradeApi* api);

	std::map<DESTROYFTRADEAPI, IFTradeApi*> m_fTradeApis;
	std::map<DESTROYTTRADEAPI, ITTradeApi*> m_tTradeApis;
	std::list<CFTradeSpi*>              m_fTradeSpis;
	std::list<CTTradeSpi*>              m_tTradeSpis;
	ITraderManageSpi*                   m_spi;

private:
	virtual void registerApi(const char* apiName, int apiID);
	virtual void registerSpi(ITraderManageSpi* spi);
	virtual void reqUserLogin(x_stuUserLogin& userLogin);
	virtual void reqUserLogin(x_stuMUserLogin& userLogin);

	virtual void reqPlaceOrder(int id, int orderIndex, const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume, double price);
	virtual void reqCancelOrder(int id, int orderIndex);
};

#endif // TraderManage_H
