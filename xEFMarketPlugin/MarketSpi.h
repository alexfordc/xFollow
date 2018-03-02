#ifndef MarketSpi_H
#define MarketSpi_H

#include <atomic>
#include <string>
#include <thread>
#include "./Include/ThostFtdcMdApi.h"
#include "../xTraderManage/Interface/IMarket.h"

class CMarketSpi : public CThostFtdcMdSpi
{
public:
	CMarketSpi();
	virtual ~CMarketSpi();

	void registerApi(CThostFtdcMdApi* api);
	void setSubscribeInstruments(std::list<std::string> instrumentIDs);
	void registerSpi(IMarketSpi* spi);
	void setUserInfo(char marketType, std::string brokerID, std::string accountID, std::string password);
	void init();

private:
	IMarketSpi*          m_callback;
	CThostFtdcMdApi*     m_api;

	std::list<std::string>  m_instrumentIDs;

	char                 m_marketType;
	std::string          m_brokerID;
	std::string          m_accountID;
	std::string          m_password;

	int  run();
	std::atomic<bool>    m_hasReaction;
	std::thread*         m_thread;

	CThostFtdcRspUserLoginField m_rspUserLogin;

	bool                 m_successed;
	int                  m_errorID;

	bool                 m_inited;

private:
	virtual void OnFrontConnected();
	virtual void OnFrontDisconnected(int nReason);
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
};

#endif // MarketSpi_H
