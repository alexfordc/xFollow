#include "TraderManage.h"

#include <cassert>

#include "../Interface/IMarket.h"
#include "MarketSpi.h"

#include "../Interface/IFTrade.h"
#include "../Interface/ITTrade.h"
#include "FTradeSpi.h"
#include "TTradeSpi.h"

ITraderManage* ITraderManage::createTraderManage()
{
	return new CTraderManage;
}

void ITraderManage::destroyTraderManage( ITraderManage* target )
{
	delete target;
}

//////////////////////////////////////////////////////////////////////////
CTraderManage::CTraderManage()
	: m_spi(nullptr)
{

}

CTraderManage::~CTraderManage()
{
// 	clear();
}

void CTraderManage::clear()
{
	for (auto& spi : m_marketSpis)
	{
		delete spi;
	}
	m_marketSpis.clear();
	for (auto& api : m_marketApis)
	{
		typedef void (*DESTROYMARKETAPI)(IMarketApi* api);
		DESTROYMARKETAPI desf = (DESTROYMARKETAPI)GetProcAddress(api.second, "destroyMarketApi");
		desf(api.first);
	}
	m_marketApis.clear();

	for (auto& spi : m_fTradeSpis)
	{
		delete spi;
	}
	m_fTradeSpis.clear();
	for (auto& spi : m_tTradeSpis)
	{
		delete spi;
	}
	m_tTradeSpis.clear();

	for (auto& api : m_fTradeApis)
	{
		typedef void (*DESTROYFTRADEAPI)(IFTradeApi* api);
		DESTROYFTRADEAPI desf = (DESTROYFTRADEAPI)GetProcAddress(api.second, "destroyFTradeApi");
		desf(api.first);
	}
	m_fTradeApis.clear();
	for (auto& api : m_tTradeApis)
	{
		typedef void (*DESTROYTTRADEAPI)(ITTradeApi* api);
		DESTROYTTRADEAPI desf = (DESTROYTTRADEAPI)GetProcAddress(api.second, "destroyTTradeApi");
		desf(api.first);
	}
	m_tTradeApis.clear();

	m_apiInfos.clear();
	m_apiName.clear();

	for (auto& apiModel : m_apiModel)
	{
		if (apiModel.second != nullptr)
			FreeLibrary(apiModel.second);
	}
	m_apiModel.clear();
	m_spi = nullptr;
}

HMODULE CTraderManage::getApiModelByID( int apiID )
{
	auto it = m_apiModel.find(apiID);
	if (it == m_apiModel.end()) {
		auto itName = m_apiName.find(apiID);
		if (itName == m_apiName.end()) {
			return nullptr;
		}
		else {
			std::string dllName = itName->second;
			HMODULE module = LoadLibrary(dllName.c_str());
			if (module == nullptr) {
				return nullptr;
			}
			m_apiModel[apiID] = module;

			return module;
		}
	}
	else {
		return it->second;
	}
}

//////////////////////////////////////////////////////////////////////////
void CTraderManage::registerApi( const char* apiName, int apiID )
{
	m_apiName[apiID] = apiName;
}

void CTraderManage::registerSpi(ITraderManageSpi* spi )
{
	m_spi = spi;
}

void CTraderManage::reqUserLogin( x_stuUserLogin& userLogin )
{
	std::string key = std::to_string(userLogin.apiID);
	key += "|";
	key += userLogin.accountID;
	auto it = m_apiInfos.find(key);
	if (it != m_apiInfos.end())
	{
		stuApiInfo& apiInfo = it->second;
		if (apiInfo.type == 0)
		{
			// error
		}
		else if (apiInfo.type == 1)
		{
			apiInfo.spi.fspi->registerID(userLogin.id);
			apiInfo.api.fapi->reqUserLogin(userLogin);
		}
		else if (apiInfo.type == 2)
		{
			apiInfo.spi.tspi->registerID(userLogin.id);
			apiInfo.api.tapi->reqUserLogin(userLogin);
		}
		return;
	}

	HMODULE module = getApiModelByID(userLogin.apiID);
	if (module == nullptr) return;

	// 
	if (userLogin.isFollow)
	{
		typedef IFTradeApi* (*CREATETRADEAPI)();
		CREATETRADEAPI func = (CREATETRADEAPI)GetProcAddress(module, "createFTradeApi");

		IFTradeApi* api = func();
		assert(api);
		m_fTradeApis[api] = module;
		m_apis[userLogin.id] = std::make_pair(true, api);

		CFTradeSpi* spi = new CFTradeSpi;
		m_fTradeSpis.push_back(spi);
		spi->registerID(userLogin.id);
		spi->registerSpi(m_spi);
		api->registerSpi(spi);
		api->reqUserLogin(userLogin);

		stuApiInfo& apiInfo = m_apiInfos[key];
		apiInfo.type = 1;
		apiInfo.api.fapi = api;
		apiInfo.spi.fspi = spi;
	}
	else
	{
		typedef ITTradeApi* (*CREATETRADEAPI)();
		CREATETRADEAPI func = (CREATETRADEAPI)GetProcAddress(module, "createTTradeApi");

		ITTradeApi* api = func();
		assert(api);
		m_tTradeApis[api] = module;
		m_apis[userLogin.id] = std::make_pair(false, api);

		CTTradeSpi* spi = new CTTradeSpi;
		m_tTradeSpis.push_back(spi);
		spi->registerID(userLogin.id);
		spi->registerSpi(m_spi);
		api->registerSpi(spi);
		api->reqUserLogin(userLogin);

		stuApiInfo& apiInfo = m_apiInfos[key];
		apiInfo.type = 2;
		apiInfo.api.tapi = api;
		apiInfo.spi.tspi = spi;
	}
}

void CTraderManage::reqUserLogin( x_stuMUserLogin& userLogin )
{
	std::string key = std::to_string(userLogin.apiID);
	key += "|";
	key += userLogin.accountID;
	auto it = m_apiInfos.find(key);
	if (it != m_apiInfos.end())
	{
		stuApiInfo& apiInfo = it->second;
		if (apiInfo.type == 0)
		{
			apiInfo.api.mapi->reqUserLogin(userLogin);
		}
		else if (apiInfo.type == 1)
		{
			// error
		}
		else if (apiInfo.type == 2)
		{
			// error
		}
		return;
	}

	HMODULE module = getApiModelByID(userLogin.apiID);
	if (module == nullptr) return;

	// 
	typedef IMarketApi* (*CREATEMarketAPI)();
	CREATEMarketAPI func = (CREATEMarketAPI)GetProcAddress(module, "createMarketApi");

	IMarketApi* api = func();
	assert(api);
	m_marketApis[api] = module;

	CMarketSpi* spi = new CMarketSpi;
	m_marketSpis.push_back(spi);
	spi->setMarketType(userLogin.marketType);
	spi->registerSpi(m_spi);
	api->registerSpi(spi);
	api->setSubscribeInstruments(userLogin.instruments);
	api->reqUserLogin(userLogin);

	stuApiInfo& apiInfo = m_apiInfos[key];
	apiInfo.type = 0;
	apiInfo.api.mapi = api;
	apiInfo.spi.mspi = spi;
}

void CTraderManage::reqPlaceOrder(int id, int orderIndex, const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume, double price)
{
	auto it = m_apis.find(id);
	if (it != m_apis.end()) {
		if (it->second.first) {
			IFTradeApi* api = (IFTradeApi*)(it->second.second);
			api->reqPlaceOrder(orderIndex, productID, instrumentID, isBuy, isOpen, hedgeFlag, volume, price);
		}
	} else {
		m_spi->rtnOrder(orderIndex, OST_Failed, 0);
	}
}

void CTraderManage::reqCancelOrder(int id, int orderIndex)
{
	auto it = m_apis.find(id);
	if (it != m_apis.end()) {
		if (it->second.first) {
			IFTradeApi* api = (IFTradeApi*)(it->second.second);
			api->reqCancelOrder(orderIndex);
		}
	} else {
		m_spi->rtnOrder(orderIndex, OST_Failed, 0);
	}
}
