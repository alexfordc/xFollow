#include "MarketSpi.h"

#include <chrono>

CMarketSpi::CMarketSpi()
	: m_callback(nullptr)
	, m_api(nullptr)
	, m_marketType('\0')
	, m_successed(false)
	, m_errorID(0)
	, m_inited(false)

	, m_thread(nullptr)
{
	memset(&m_rspUserLogin, 0, sizeof(m_rspUserLogin));
}

CMarketSpi::~CMarketSpi()
{

}

void CMarketSpi::registerApi( CThostFtdcMdApi* api )
{
	m_api = api;
}

void CMarketSpi::setSubscribeInstruments( std::list<std::string> instrumentIDs )
{
	m_instrumentIDs.insert(m_instrumentIDs.begin(), instrumentIDs.begin(), instrumentIDs.end());
}

void CMarketSpi::registerSpi( IMarketSpi* spi )
{
	m_callback = spi;
}

void CMarketSpi::setUserInfo( char marketType, std::string brokerID, std::string accountID, std::string password )
{
	m_marketType = marketType;
	m_brokerID = brokerID;
	m_accountID = accountID;
	m_password = password;
}

void CMarketSpi::init()
{
	m_hasReaction = false;
	if (nullptr != m_thread)
	{
		m_thread->join();
		delete m_thread;
	}
	m_thread = new std::thread(std::bind(&CMarketSpi::run, this));
	m_api->Init();
}

int CMarketSpi::run()
{
	auto f_now = std::chrono::system_clock::now();
	while (true)
	{
		if (m_hasReaction)
		{
			return 0;
		}
		std::this_thread::sleep_for(std::chrono::microseconds(10));
		auto now = std::chrono::system_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - f_now);
		if (duration.count() > 60)
			break;
	}
	m_callback->rspUserLogin(false, 0);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
void CMarketSpi::OnFrontConnected()
{
	m_hasReaction = true;
	CThostFtdcReqUserLoginField field = {0};
	strncpy_s(field.UserID, m_accountID.c_str(), sizeof(field.UserID));
	strncpy_s(field.Password, m_password.c_str(), sizeof(field.Password));
	m_api->ReqUserLogin(&field, 0);
}

void CMarketSpi::OnFrontDisconnected( int nReason )
{
	m_hasReaction = true;
	m_successed = false;
	m_errorID = 0;
	m_inited = false;
}

void CMarketSpi::OnRspUserLogin( CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	if (pRspInfo != nullptr)
	{
		m_errorID = pRspInfo->ErrorID;
	}
	if (pRspUserLogin != nullptr && (pRspInfo == nullptr || pRspInfo->ErrorID == 0))
	{
		m_successed = true;
		m_rspUserLogin = *pRspUserLogin;
	}

	if (bIsLast)
	{
		if (m_successed)
		{
			char** instrumentID = nullptr;
			for (auto& i : m_instrumentIDs)
			{
				char* ii = (const_cast<char*>(i.c_str()));
				instrumentID = &ii;
				m_api->SubscribeMarketData(instrumentID, 1);
			}
		}
		m_callback->rspUserLogin(m_successed, m_errorID);
	}
}

void CMarketSpi::OnRtnDepthMarketData( CThostFtdcDepthMarketDataField *pDepthMarketData )
{
	if (pDepthMarketData == NULL) return;

	if(pDepthMarketData->ClosePrice==DBL_MAX)
		pDepthMarketData->ClosePrice=0;
	if(pDepthMarketData->SettlementPrice==DBL_MAX)
		pDepthMarketData->SettlementPrice=0;
	if(pDepthMarketData->PreDelta==DBL_MAX)
		pDepthMarketData->PreDelta=0;
	if(pDepthMarketData->CurrDelta==DBL_MAX)
		pDepthMarketData->CurrDelta=0;
	if(pDepthMarketData->BidPrice1==DBL_MAX)
		pDepthMarketData->BidPrice1=0;
	if(pDepthMarketData->BidPrice2==DBL_MAX)
		pDepthMarketData->BidPrice2=0;
	if(pDepthMarketData->BidPrice3==DBL_MAX)
		pDepthMarketData->BidPrice3=0;
	if(pDepthMarketData->BidPrice4==DBL_MAX)
		pDepthMarketData->BidPrice4=0;
	if(pDepthMarketData->BidPrice5==DBL_MAX)
		pDepthMarketData->BidPrice5=0;
	if(pDepthMarketData->AskPrice1==DBL_MAX)
		pDepthMarketData->AskPrice1=0;
	if(pDepthMarketData->AskPrice2==DBL_MAX)
		pDepthMarketData->AskPrice2=0;
	if(pDepthMarketData->AskPrice3==DBL_MAX)
		pDepthMarketData->AskPrice3=0;
	if(pDepthMarketData->AskPrice4==DBL_MAX)
		pDepthMarketData->AskPrice4=0;
	if(pDepthMarketData->AskPrice5==DBL_MAX)
		pDepthMarketData->AskPrice5=0;
	if(pDepthMarketData->OpenInterest==DBL_MAX)
		pDepthMarketData->OpenInterest=0;
	if(pDepthMarketData->PreOpenInterest==DBL_MAX)
		pDepthMarketData->PreOpenInterest=0;
	if(pDepthMarketData->LastPrice==DBL_MAX)
		pDepthMarketData->LastPrice=0;
	if(pDepthMarketData->PreSettlementPrice==DBL_MAX)
		pDepthMarketData->PreSettlementPrice=0;
	if(pDepthMarketData->OpenPrice==DBL_MAX)
		pDepthMarketData->OpenPrice=0;
	if(pDepthMarketData->HighestPrice==DBL_MAX)
		pDepthMarketData->HighestPrice=0;
	if(pDepthMarketData->LowestPrice==DBL_MAX)
		pDepthMarketData->LowestPrice=0;
	if(pDepthMarketData->Turnover==DBL_MAX)
		pDepthMarketData->Turnover=0;
	if(pDepthMarketData->UpperLimitPrice==DBL_MAX)
		pDepthMarketData->UpperLimitPrice=0;
	if(pDepthMarketData->LowerLimitPrice==DBL_MAX)
		pDepthMarketData->LowerLimitPrice=0;
	if(pDepthMarketData->LowerLimitPrice==DBL_MAX)
		pDepthMarketData->LowerLimitPrice=0;
	if(pDepthMarketData->AveragePrice==DBL_MAX)
		pDepthMarketData->AveragePrice=0;
	if(pDepthMarketData->PreClosePrice==DBL_MAX)
		pDepthMarketData->PreClosePrice=0;

	m_callback->rtnMarketData(m_marketType, pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice);
}
