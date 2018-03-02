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
	clear();
}

void CTraderManage::clear()
{
	m_apiName.clear();

	for (auto& apiModel : m_apiModel)
	{
		if (apiModel.second != nullptr)
			FreeLibrary(apiModel.second);
	}
	m_apiModel.clear();

	for (auto& spi : m_marketSpis)
	{
		delete spi;
	}
	m_marketSpis.clear();
	for (auto& api : m_marketApis)
	{
		api.first(api.second);
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
		api.first(api.second);
	}
	m_fTradeApis.clear();
	for (auto& api : m_tTradeApis)
	{
		api.first(api.second);
	}
	m_tTradeApis.clear();

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
	HMODULE module = getApiModelByID(userLogin.apiID);
	if (module == nullptr) return;

	// 
	if (userLogin.isFollow)
	{
		typedef IFTradeApi* (*CREATETRADEAPI)();
		CREATETRADEAPI func = (CREATETRADEAPI)GetProcAddress(module, "createFTradeApi");

		IFTradeApi* api = func();
		assert(api);
		DESTROYFTRADEAPI desf = (DESTROYFTRADEAPI)GetProcAddress(module, "destroyFTradeApi");
		m_fTradeApis[desf] = api;
		m_apis[userLogin.id] = std::make_pair(true, api);

		CFTradeSpi* spi = new CFTradeSpi;
		m_fTradeSpis.push_back(spi);
		spi->registerID(userLogin.id);
		spi->registerSpi(m_spi);
		api->registerSpi(spi);
		api->reqUserLogin(userLogin);
	}
	else
	{
		typedef ITTradeApi* (*CREATETRADEAPI)();
		CREATETRADEAPI func = (CREATETRADEAPI)GetProcAddress(module, "createTTradeApi");

		ITTradeApi* api = func();
		assert(api);
		DESTROYTTRADEAPI desf = (DESTROYTTRADEAPI)GetProcAddress(module, "destroyTTradeApi");
		m_tTradeApis[desf] = api;
		m_apis[userLogin.id] = std::make_pair(false, api);

		CTTradeSpi* spi = new CTTradeSpi;
		m_tTradeSpis.push_back(spi);
		spi->registerID(userLogin.id);
		spi->registerSpi(m_spi);
		api->registerSpi(spi);
		api->reqUserLogin(userLogin);
	}
}

void CTraderManage::reqUserLogin( x_stuMUserLogin& userLogin )
{
	HMODULE module = getApiModelByID(userLogin.apiID);
	if (module == nullptr) return;

	// 
	typedef IMarketApi* (*CREATEMarketAPI)();
	CREATEMarketAPI func = (CREATEMarketAPI)GetProcAddress(module, "createMarketApi");

	IMarketApi* api = func();
	assert(api);
	DESTROYMARKETAPI desf = (DESTROYMARKETAPI)GetProcAddress(module, "destroyMarketApi");
	m_marketApis[desf] = api;

	CMarketSpi* spi = new CMarketSpi;
	m_marketSpis.push_back(spi);
	spi->setMarketType(userLogin.marketType);
	spi->registerSpi(m_spi);
	api->registerSpi(spi);
	api->setSubscribeInstruments(userLogin.instruments);
	api->reqUserLogin(userLogin);
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
