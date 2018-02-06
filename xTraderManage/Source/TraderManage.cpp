#include "TraderManage.h"

#include <cassert>

#include "../Interface/ITrade.h"
#include "TradeSpi.h"

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

void CTraderManage::reqUserLogin( int id, int apiID, const char* ip, int port, const char* accountID, const char* password )
{
	HMODULE module = getApiModelByID(apiID);
	if (module == nullptr) return;

	// 
	typedef ITradeApi* (*CREATETRADEAPI)();
	CREATETRADEAPI func = (CREATETRADEAPI)GetProcAddress(module, "createTradeApi");

	ITradeApi* api = func();
	assert(api);
	m_apis[id] = api;
	CTradeSpi* spi = new CTradeSpi;
	spi->registerID(id);
	spi->registerSpi(m_spi);
	api->registerSpi(spi);
	api->reqUserLogin(ip, port, accountID, password);
}

void CTraderManage::reqPlaceOrder(int id, const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume)
{
	auto it = m_apis.find(id);
	if (it != m_apis.end()) {
		ITradeApi* api = (ITradeApi*)(it->second);
		api->reqPlaceOrder(productID, instrumentID, isBuy, isOpen, hedgeFlag, volume);
	} else {
		m_spi->rspPlaceOrder();
	}
}

void CTraderManage::reqCancelOrder(int id)
{
	auto it = m_apis.find(id);
	if (it != m_apis.end()) {
		ITradeApi* api = (ITradeApi*)(it->second);
		api->reqCancelOrder();
	} else {
		m_spi->rspCancelOrder();
	}
}
