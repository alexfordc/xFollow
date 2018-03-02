#ifndef FTradeSpi_H
#define FTradeSpi_H

#include "Include/ThostFtdcTraderApi.h"
#include "../xTraderManage/Interface/IFTrade.h"

#include <atomic>
#include <map>
#include <set>
#include <string>
#include <thread>

class CFTradeSpi : public CThostFtdcTraderSpi
{
public:
	CFTradeSpi();
	~CFTradeSpi();

public:
	void registerApi(CThostFtdcTraderApi* api);
	void registerSpi(IFTradeSpi* spi);
	void setUserInfo(std::string brokerID, std::string accountID, std::string password);
	void init();
	void reqPlaceOrder(int orderIndex, const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume, double price);
	void reqCancelOrder(int orderIndex);

private:
	IFTradeSpi*          m_callback;
	CThostFtdcTraderApi* m_api;

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

	// instrumentID + | + direction + | + hedgeFlag + | + tradeID
	std::set<std::string> m_tradeDetails;
	// instrumentID + | + direction + | + hedgeFlag
	struct stuPosition
	{
		char instrumentID[32];
		bool isBuy;
		char hedgeFlag;
		int  volume;
	};
	std::map<std::string, stuPosition> m_positions;

	int                   m_orderReference;
	std::map<std::string, int> m_orders;
	std::map<int, std::string> m_orderIndexs;

private:
	virtual void OnFrontConnected();
	virtual void OnFrontDisconnected(int nReason);
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus);

	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);
	virtual void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo);

};

#endif // FTradeSpi_H
