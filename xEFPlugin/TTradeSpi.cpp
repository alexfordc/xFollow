#include "TTradeSpi.h"


CTTradeSpi::CTTradeSpi()
	: m_callback(nullptr)
{

}

CTTradeSpi::~CTTradeSpi()
{

}

void CTTradeSpi::registerApi( CThostFtdcTraderApi* api )
{
	m_api = api;
}

void CTTradeSpi::registerSpi( ITTradeSpi* spi )
{
	m_callback = spi;
}

void CTTradeSpi::setUserInfo( std::string accountID, std::string password )
{
	m_accountID = accountID;
	m_password = password;
}

void CTTradeSpi::reqPlaceOrder( const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume )
{

}

void CTTradeSpi::reqCancelOrder()
{

}

//////////////////////////////////////////////////////////////////////////
void CTTradeSpi::OnFrontConnected()
{
	CThostFtdcReqUserLoginField field = {0};
	strncpy_s(field.UserID, m_accountID.c_str(), sizeof(field.UserID));
	strncpy_s(field.Password, m_password.c_str(), sizeof(field.Password));
	m_api->ReqUserLogin(&field, 0);
}

void CTTradeSpi::OnFrontDisconnected( int nReason )
{

}

void CTTradeSpi::OnRspUserLogin( CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
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

	m_callback->rspUserInitialized(successed, errorID);
}

void CTTradeSpi::OnRspQryOrder( CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{

}

void CTTradeSpi::OnRspQryTrade( CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{

}

void CTTradeSpi::OnRspQryInvestorPositionDetail( CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{

}

void CTTradeSpi::OnRtnInstrumentStatus( CThostFtdcInstrumentStatusField *pInstrumentStatus )
{

}

void CTTradeSpi::OnRtnOrder( CThostFtdcOrderField *pOrder )
{

}

void CTTradeSpi::OnRtnTrade( CThostFtdcTradeField *pTrade )
{

}

void CTTradeSpi::OnRspOrderInsert( CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{

}

void CTTradeSpi::OnRspOrderAction( CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{

}

void CTTradeSpi::OnErrRtnOrderInsert( CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo )
{

}

void CTTradeSpi::OnErrRtnOrderAction( CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo )
{

}
