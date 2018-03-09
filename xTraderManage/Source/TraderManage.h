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
	virtual ~CTraderManage();
public:
	void clear();

private:
	HMODULE getApiModelByID(int apiID);
	std::map<int, std::string>          m_apiName;
	std::map<int, HMODULE>              m_apiModel;
	std::map<int, std::pair<bool, void*>> m_apis;

	std::map<IMarketApi*, HMODULE>      m_marketApis;
	std::list<CMarketSpi*>              m_marketSpis;

	std::map<IFTradeApi*, HMODULE>      m_fTradeApis;
	std::map<ITTradeApi*, HMODULE>      m_tTradeApis;
	std::list<CFTradeSpi*>              m_fTradeSpis;
	std::list<CTTradeSpi*>              m_tTradeSpis;
	ITraderManageSpi*                   m_spi;
	struct stuApiInfo
	{
		int type;
		union unApi
		{
			IMarketApi* mapi;
			IFTradeApi* fapi;
			ITTradeApi* tapi;
		} api;
		union unSpi
		{
			CMarketSpi* mspi;
			CFTradeSpi* fspi;
			CTTradeSpi* tspi;
		} spi;
	};
	std::map<std::string, stuApiInfo>  m_apiInfos;

private:
	virtual void registerApi(const char* apiName, int apiID);
	virtual void registerSpi(ITraderManageSpi* spi);
	virtual void reqUserLogin(x_stuUserLogin& userLogin);
	virtual void reqUserLogin(x_stuMUserLogin& userLogin);

	virtual void reqPlaceOrder(int id, int orderIndex, const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume, double price);
	virtual void reqCancelOrder(int id, int orderIndex);
};

#endif // TraderManage_H
