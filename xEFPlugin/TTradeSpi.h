#ifndef TTradeSpi_H
#define TTradeSpi_H

#include "Include/ThostFtdcTraderApi.h"
#include "../xTraderManage/Interface/ITTrade.h"

#include <atomic>
#include <map>
#include <set>
#include <string>
#include <thread>

class CTTradeSpi : public CThostFtdcTraderSpi
{
public:
	CTTradeSpi();
	~CTTradeSpi();

public:
	void registerApi(CThostFtdcTraderApi* api);
	void registerSpi(ITTradeSpi* spi);
	void setUserInfo(std::string accountID, std::string password);
	void init();

private:
	ITTradeSpi*          m_callback;
	CThostFtdcTraderApi* m_api;

	std::string          m_accountID;
	std::string          m_password;

	int  run();
	std::atomic<bool>    m_hasReaction;
	std::thread*         m_thread;

	CThostFtdcRspUserLoginField m_rspUserLogin;

	bool                 m_successed;
	int                  m_errorID;

	bool                 m_inited;

	bool                 m_wait2rtnP;

	// instrumentID + | + direction + | + hedgeFlag + | + tradeID
	std::set<std::string> m_tradeDetails;
	// instrumentID + | + direction + | + hedgeFlag
	struct stuPosition
	{
		char instrumentID[32];
		bool isBuy;
		char hedgeFlag;
		int volume;
	};
	std::map<std::string, stuPosition> m_positions;

private:
	virtual void OnFrontConnected();
	virtual void OnFrontDisconnected(int nReason);
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);

};

#endif // TTradeSpi_H
