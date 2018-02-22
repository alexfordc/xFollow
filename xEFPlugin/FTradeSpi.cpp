#include "FTradeSpi.h"


CFTradeSpi::CFTradeSpi()
	: m_callback(nullptr)
{

}

CFTradeSpi::~CFTradeSpi()
{

}

void CFTradeSpi::registerApi( CThostFtdcTraderApi* api )
{
	m_api = api;
}

void CFTradeSpi::registerSpi( IFTradeSpi* spi )
{
	m_callback = spi;
}

void CFTradeSpi::setUserInfo( std::string accountID, std::string password )
{
	m_accountID = accountID;
	m_password = password;
}

void CFTradeSpi::reqPlaceOrder( const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume )
{

}

void CFTradeSpi::reqCancelOrder()
{

}

//////////////////////////////////////////////////////////////////////////
void CFTradeSpi::OnFrontConnected()
{
	CThostFtdcReqUserLoginField field = {0};
	strncpy_s(field.UserID, m_accountID.c_str(), sizeof(field.UserID));
	strncpy_s(field.Password, m_password.c_str(), sizeof(field.Password));
	m_api->ReqUserLogin(&field, 0);
}

void CFTradeSpi::OnFrontDisconnected( int nReason )
{

}

void CFTradeSpi::OnRspUserLogin( CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	bool successed = false;
	int errorID = 0;
	if (pRspInfo != nullptr)
	{
		errorID = pRspInfo->ErrorID;
	}
	if (pRspUserLogin != nullptr && (pRspInfo == nullptr || pRspInfo->ErrorID == 0))
	{
		successed = true;
	}
	m_callback->rspUserLogin(successed, errorID);

	CThostFtdcSettlementInfoConfirmField field = {0};
	m_api->ReqSettlementInfoConfirm(&field, 0);
}

void CFTradeSpi::OnRspSettlementInfoConfirm( CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	bool successed = false;
	int errorID = 0;
	if (pRspInfo != nullptr)
	{
		errorID = pRspInfo->ErrorID;
	}
	if (pSettlementInfoConfirm != nullptr && (pRspInfo == nullptr || pRspInfo->ErrorID == 0))
	{
		successed = true;
	}
	m_callback->rspUserInitialized(successed, errorID);
}

void CFTradeSpi::OnRspQryOrder( CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{

}

void CFTradeSpi::OnRspQryTrade( CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{

}

void CFTradeSpi::OnRspQryInvestorPositionDetail( CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{

}

void CFTradeSpi::OnRtnInstrumentStatus( CThostFtdcInstrumentStatusField *pInstrumentStatus )
{

}

void CFTradeSpi::OnRtnOrder( CThostFtdcOrderField *pOrder )
{

}

void CFTradeSpi::OnRtnTrade( CThostFtdcTradeField *pTrade )
{

}

void CFTradeSpi::OnRspOrderInsert( CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{

}

void CFTradeSpi::OnRspOrderAction( CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{

}

void CFTradeSpi::OnErrRtnOrderInsert( CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo )
{

}

void CFTradeSpi::OnErrRtnOrderAction( CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo )
{

}
