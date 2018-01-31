#include "TraderManage.h"


ITraderManage* ITraderManage::createTraderManage()
{
	return new CTraderManage;
}
//////////////////////////////////////////////////////////////////////////
CTraderManage::CTraderManage()
	: m_spi(nullptr)
{

}

CTraderManage::~CTraderManage()
{

}

HANDLE CTraderManage::getApiModelByID( int apiID )
{
	auto it = m_apiModel.find(apiID);
	if (it == m_apiModel.end()) {
		auto itName = m_apiName.find(apiID);
		if (itName == m_apiName.end()) {
			return nullptr;
		}
		else {
			std::string dllName = itName->second;
			HANDLE module = LoadLibrary(dllName.c_str());
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

void CTraderManage::registerApi( const char* apiName, int apiID )
{
	m_apiName[apiID] = apiName;
}

void CTraderManage::registerSpi( ITraderSpi* spi )
{
	m_spi = spi;
}

void CTraderManage::reqUserLogin( int apiID, const char* ip, int port, const char* accountID, const char* password )
{
	HANDLE module = getApiModelByID(apiID);
	if (module == nullptr) return;
}
