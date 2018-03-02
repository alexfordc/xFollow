#include "TTradeSpi.h"

#include "QueryTask.h"

#include <chrono>

CTTradeSpi::CTTradeSpi()
	: m_callback(nullptr)
	, m_api(nullptr)
	, m_successed(false)
	, m_errorID(0)
	, m_inited(false)

	, m_thread(nullptr)
{
	memset(&m_rspUserLogin, 0, sizeof(m_rspUserLogin));
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

void CTTradeSpi::init()
{
	m_hasReaction = false;
	if (nullptr != m_thread)
	{
		m_thread->join();
		delete m_thread;
	}
	m_thread = new std::thread(std::bind(&CTTradeSpi::run, this));
	m_api->Init();
}

int CTTradeSpi::run()
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
void CTTradeSpi::OnFrontConnected()
{
	m_hasReaction = true;
	CThostFtdcReqUserLoginField field = {0};
	strncpy_s(field.UserID, m_accountID.c_str(), sizeof(field.UserID));
	strncpy_s(field.Password, m_password.c_str(), sizeof(field.Password));
	CQueryTask::queryTask().login(m_api, field);
}

void CTTradeSpi::OnFrontDisconnected( int nReason )
{
	m_hasReaction = true;
	m_successed = false;
	m_errorID = 0;
	m_inited = false;
	m_tradeDetails.clear();
	m_positions.clear();
}

void CTTradeSpi::OnRspUserLogin( CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	if (pRspInfo != nullptr)
	{
		m_errorID = pRspInfo->ErrorID;
	}
	if (pRspUserLogin != nullptr && (pRspInfo == nullptr || pRspInfo->ErrorID == 0))
	{
		m_successed = true;
		m_rspUserLogin = *pRspUserLogin;

		CThostFtdcQryInvestorPositionDetailField field = {0};
		CQueryTask::queryTask().qryPositionDetail(m_api, field);
	}

	if (bIsLast)
		m_callback->rspUserLogin(m_successed, m_errorID);
}

void CTTradeSpi::OnRspQryOrder( CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	// 暂时目标账号查询委托没用
	if (bIsLast)
	{
		CThostFtdcQryTradeField field = {0};
		CQueryTask::queryTask().qryTrade(m_api, field);
	}
}

void CTTradeSpi::OnRspQryTrade( CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	if (nullptr != pTrade)
	{
		std::string key = pTrade->InstrumentID;
		key += "|";
		key += pTrade->Direction;
		key += "|";
		key += pTrade->HedgeFlag;
		std::string pkey = key;
		key += "|";
		key += pTrade->TradeID;
		auto it = m_tradeDetails.find(key);
		if (it == m_tradeDetails.end())
		{
			m_tradeDetails.insert(key);

			//
			auto itp = m_positions.find(pkey);
			if (itp == m_positions.end())
			{
				auto&& pp = m_positions[pkey];
				strncpy_s(pp.instrumentID, pTrade->InstrumentID, sizeof(pp.instrumentID));
				pp.isBuy = pTrade->Direction == THOST_FTDC_D_Buy;
				pp.hedgeFlag = pTrade->HedgeFlag;
				pp.volume = (pTrade->OffsetFlag == THOST_FTDC_OF_Open ? pTrade->Volume : -pTrade->Volume);
			}
			else
			{
				itp->second.volume += (pTrade->OffsetFlag == THOST_FTDC_OF_Open ? pTrade->Volume : -pTrade->Volume);
			}
		}
	}
	if (bIsLast)
	{
		m_callback->rspUserInitialized(m_successed, m_errorID);
		m_inited = true;
		m_successed = false;
		m_errorID = 0;

		for (auto& position : m_positions)
		{
			m_callback->rtnPositionTotal(position.second.instrumentID, position.second.instrumentID, 
				position.second.isBuy, position.second.hedgeFlag, position.second.volume);
		}
	}
}

void CTTradeSpi::OnRspQryInvestorPositionDetail( CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	if (nullptr != pInvestorPositionDetail)
	{
		if (strncmp(m_rspUserLogin.TradingDay, pInvestorPositionDetail->OpenDate, sizeof(m_rspUserLogin.TradingDay)) != 0)
		{ // 不是当前交易日的持仓明细认定为昨仓
			std::string key = pInvestorPositionDetail->InstrumentID;
			key += "|";
			key += pInvestorPositionDetail->Direction == THOST_FTDC_PD_Long ? THOST_FTDC_D_Buy : THOST_FTDC_D_Sell;
			key += "|";
			key += pInvestorPositionDetail->HedgeFlag;
			auto itp = m_positions.find(key);
			if (itp == m_positions.end())
			{
				auto&& pp = m_positions[key];
				strncpy_s(pp.instrumentID, pInvestorPositionDetail->InstrumentID, sizeof(pp.instrumentID));
				pp.isBuy = pInvestorPositionDetail->Direction == THOST_FTDC_PD_Long;
				pp.hedgeFlag = pInvestorPositionDetail->HedgeFlag;
				pp.volume = pInvestorPositionDetail->Volume;
			}
			else
			{
				itp->second.volume += pInvestorPositionDetail->Volume;
			}
		}
	}

	if (bIsLast)
	{
		CThostFtdcQryOrderField field = {0};
		CQueryTask::queryTask().qryOrder(m_api, field);
	}
}

void CTTradeSpi::OnRtnOrder( CThostFtdcOrderField *pOrder )
{

}

void CTTradeSpi::OnRtnTrade( CThostFtdcTradeField *pTrade )
{
	if (nullptr == pTrade) return;

	std::string key = pTrade->InstrumentID;
	key += "|";
	key += pTrade->Direction;
	key += "|";
	key += pTrade->HedgeFlag;
	std::string pkey = key;
	key += "|";
	key += pTrade->TradeID;

	int volume = 0;
	auto it = m_tradeDetails.find(key);
	if (it == m_tradeDetails.end())
	{
		m_tradeDetails.insert(key);

		//
		auto itp = m_positions.find(pkey);
		if (itp == m_positions.end())
		{
			auto&& pp = m_positions[pkey];
			strncpy_s(pp.instrumentID, pTrade->InstrumentID, sizeof(pp.instrumentID));
			pp.isBuy = pTrade->Direction == THOST_FTDC_D_Buy;
			pp.hedgeFlag = pTrade->HedgeFlag;
			pp.volume = (pTrade->OffsetFlag == THOST_FTDC_OF_Open ? pTrade->Volume : -pTrade->Volume);

			volume = pp.volume;
		}
		else
		{
			itp->second.volume += (pTrade->OffsetFlag == THOST_FTDC_OF_Open ? pTrade->Volume : -pTrade->Volume);

			volume = itp->second.volume;
		}
	}
	else
	{
		return;
	}

	if (m_inited)
	{
		m_callback->rtnTrade(pTrade->InstrumentID, pTrade->InstrumentID, pTrade->Direction == THOST_FTDC_D_Buy, 
			pTrade->OffsetFlag == THOST_FTDC_OF_Open, pTrade->HedgeFlag, pTrade->Volume);
		m_callback->rtnPositionTotal(pTrade->InstrumentID, pTrade->InstrumentID, 
			pTrade->Direction == THOST_FTDC_D_Buy, pTrade->HedgeFlag, volume);
	}
}
