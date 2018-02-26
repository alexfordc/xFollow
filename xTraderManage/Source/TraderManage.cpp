#include "TraderManage.h"

#include <cassert>

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
		m_apis[userLogin.id] = std::make_pair(true, api);

		CFTradeSpi* spi = new CFTradeSpi;
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
		m_apis[userLogin.id] = std::make_pair(false, api);

		CTTradeSpi* spi = new CTTradeSpi;
		spi->registerID(userLogin.id);
		spi->registerSpi(m_spi);
		api->registerSpi(spi);
		api->reqUserLogin(userLogin);
	}
}

void CTraderManage::reqPlaceOrder(int id, int orderIndex, const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume)
{
	auto it = m_apis.find(id);
	if (it != m_apis.end()) {
		if (it->second.first) {
			IFTradeApi* api = (IFTradeApi*)(it->second.second);
			api->reqPlaceOrder(orderIndex, productID, instrumentID, isBuy, isOpen, hedgeFlag, volume, 0);
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
