#include "FTradeSpi.h"

#include "QueryTask.h"

#include <chrono>

CFTradeSpi::CFTradeSpi()
	: m_callback(nullptr)
	, m_api(nullptr)
	, m_successed(false)
	, m_errorID(0)
	, m_inited(false)
	, m_orderReference(0)

	, m_thread(nullptr)
{
	memset(&m_rspUserLogin, 0, sizeof(m_rspUserLogin));
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

void CFTradeSpi::setUserInfo( std::string brokerID, std::string accountID, std::string password )
{
	m_brokerID = brokerID;
	m_accountID = accountID;
	m_password = password;
}

void CFTradeSpi::init()
{
	m_hasReaction = false;
	if (nullptr != m_thread)
	{
		m_thread->join();
		delete m_thread;
	}
	m_thread = new std::thread(std::bind(&CFTradeSpi::run, this));
	m_api->Init();
}

void CFTradeSpi::reqPlaceOrder( int orderIndex, const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume, double price )
{
	CThostFtdcInputOrderField field = {0};
	strncpy(field.BrokerID, m_brokerID.c_str(), sizeof(field.BrokerID));
	//使用UserID的部分解析出来分账号
	strncpy(field.UserID, m_accountID.c_str(), sizeof(field.UserID));
	strncpy(field.InvestorID, m_accountID.c_str(), sizeof(field.InvestorID));
	strncpy(field.InstrumentID, instrumentID, sizeof(field.InstrumentID));
	sprintf(field.OrderRef, "%012d", ++m_orderReference);
	char key[37] = {0};
	sprintf_s(key, "%012d%012d%012d", m_rspUserLogin.SessionID, m_rspUserLogin.FrontID, m_orderReference);
	m_orders[key] = orderIndex;
	m_orderIndexs[orderIndex] = key;

	field.Direction = isBuy ? THOST_FTDC_D_Buy : THOST_FTDC_D_Sell;
	field.CombOffsetFlag[0] = isOpen ? THOST_FTDC_OF_Open : THOST_FTDC_OF_Close;
	field.CombHedgeFlag[0] = hedgeFlag;
	field.VolumeTotalOriginal = volume;
	field.TimeCondition = THOST_FTDC_TC_GFD;
	field.VolumeCondition = THOST_FTDC_VC_AV;
	field.MinVolume = 1;                                                                             // 最小成交量1;
	field.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;                                           // 强平原因,非强平;
	field.IsAutoSuspend = 1;                                                                         // 自动挂起标志: 否;
	field.UserForceClose = 0;                                                                        // 用户强评标志 否;
	field.ContingentCondition = THOST_FTDC_CC_Immediately;                                           // 触发条件:立即;
// 	if (orderField.priceType == THOST_FTDC_OPT_AnyPrice)
// 	{
// 		field.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
// 	}
// 	else if (orderField.priceType == THOST_FTDC_OPT_LimitPrice)
	{
		field.LimitPrice = price;
		field.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	}
	m_api->ReqOrderInsert(&field, 0);
}

void CFTradeSpi::reqCancelOrder(int orderIndex)
{
	auto it = m_orderIndexs.find(orderIndex);
	if (it != m_orderIndexs.end())
	{
		CThostFtdcInputOrderActionField field = {0};
		strncpy(field.BrokerID, m_brokerID.c_str(), sizeof(field.BrokerID));

		strncpy(field.UserID, m_accountID.c_str(), sizeof(field.UserID));
		strncpy(field.InvestorID, m_accountID.c_str(), sizeof(field.InvestorID));
		field.ActionFlag = THOST_FTDC_AF_Delete;
		field.SessionID = std::stoi(it->second.substr(0, 12));
		field.FrontID = std::stoi(it->second.substr(12, 12));
		strncpy_s(field.OrderRef, it->second.substr(24, 12).c_str(), sizeof(field.OrderRef));

		m_api->ReqOrderAction(&field, 0);
	}
}

int CFTradeSpi::run()
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
void CFTradeSpi::OnFrontConnected()
{
	m_hasReaction = true;
	CThostFtdcReqUserLoginField field = {0};
	strncpy_s(field.UserID, m_accountID.c_str(), sizeof(field.UserID));
	strncpy_s(field.Password, m_password.c_str(), sizeof(field.Password));
	CQueryTask::queryTask().login(m_api, field);
}

void CFTradeSpi::OnFrontDisconnected( int nReason )
{
	m_hasReaction = true;
	m_successed = false;
	m_errorID = 0;
	m_inited = false;
	m_tradeDetails.clear();
	m_positions.clear();
}

void CFTradeSpi::OnRspUserLogin( CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	if (pRspInfo != nullptr)
	{
		m_errorID = pRspInfo->ErrorID;
	}
	if (pRspUserLogin != nullptr && (pRspInfo == nullptr || pRspInfo->ErrorID == 0))
	{
		m_successed = true;
		m_rspUserLogin = *pRspUserLogin;

		CThostFtdcSettlementInfoConfirmField field = {0};
		m_api->ReqSettlementInfoConfirm(&field, 0);
	}

	if (bIsLast)
		m_callback->rspUserLogin(m_successed, m_errorID);
}

void CFTradeSpi::OnRspSettlementInfoConfirm( CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	if (bIsLast)
	{
		CThostFtdcQryInvestorPositionDetailField field = {0};
		CQueryTask::queryTask().qryPositionDetail(m_api, field);
	}
}

void CFTradeSpi::OnRspQryOrder( CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	if (bIsLast)
	{
		CThostFtdcQryTradeField field = {0};
		CQueryTask::queryTask().qryTrade(m_api, field);
	}
}

void CFTradeSpi::OnRspQryTrade( CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
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

void CFTradeSpi::OnRspQryInvestorPositionDetail( CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
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

void CFTradeSpi::OnRtnInstrumentStatus( CThostFtdcInstrumentStatusField *pInstrumentStatus )
{

}

void CFTradeSpi::OnRtnOrder( CThostFtdcOrderField *pOrder )
{
	if (nullptr == pOrder) return;

	char key[37] = {0};
	sprintf_s(key, "%012d%012d%s", pOrder->SessionID, pOrder->FrontID, pOrder->OrderRef);
	auto it = m_orders.find(key);
	if (it == m_orders.end()) return;

	char status = '\0';
	int volume = 0;
	switch (pOrder->OrderStatus)
	{
	case THOST_FTDC_OST_AllTraded:
		{
			status = OST_AllTraded;
			volume = pOrder->VolumeTraded;
		}
		break;
	case THOST_FTDC_OST_PartTradedQueueing:
		{
			status = OST_PartTradedQueueing;
			volume = pOrder->VolumeTraded;
		}
		break;
	case THOST_FTDC_OST_PartTradedNotQueueing:
		{
			status = OST_NotQueueing;
			volume = pOrder->VolumeTraded;
		}
		break;
	case THOST_FTDC_OST_NoTradeQueueing:
		{
			status = OST_NoTradeQueueing;
		}
		break;
	case THOST_FTDC_OST_NoTradeNotQueueing:
		{
			status = OST_NotQueueing;
		}
		break;
	case THOST_FTDC_OST_Canceled:
		{
			status = OST_Canceled;
		}
		break;
	case THOST_FTDC_OST_Unknown:
		return; // @note 暂时未知单不处理
		{
			status = OST_NoTradeQueueing;
		}
		break;
	case THOST_FTDC_OST_NotTouched:
		{
			status = OST_NoTradeQueueing;
		}
		break;
	case THOST_FTDC_OST_Touched:
		{
			status = OST_NoTradeQueueing;
		}
		break;
	default:
		return;
	}
	m_callback->rtnOrder(it->second, status, volume);
}

void CFTradeSpi::OnRtnTrade( CThostFtdcTradeField *pTrade )
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
		m_callback->rtnPositionTotal(pTrade->InstrumentID, pTrade->InstrumentID, 
			pTrade->Direction == THOST_FTDC_D_Buy, pTrade->HedgeFlag, volume);
	}
}

void CFTradeSpi::OnRspOrderInsert( CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	if (pRspInfo != nullptr && pRspInfo->ErrorID != 0)
	{
		char key[37] = {0};
		sprintf_s(key, "%012d%012d%s", m_rspUserLogin.SessionID, m_rspUserLogin.FrontID, pInputOrder->OrderRef);
		auto it = m_orders.find(key);
		if (it == m_orders.end()) return;

		m_callback->rtnOrder(it->second, OST_Failed, 0);
	}
}

void CFTradeSpi::OnRspOrderAction( CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	// 撤单失败不处理
}

void CFTradeSpi::OnErrRtnOrderInsert( CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo )
{
	if (pRspInfo != nullptr && pRspInfo->ErrorID != 0)
	{
		char key[37] = {0};
		sprintf_s(key, "%012d%012d%s", m_rspUserLogin.SessionID, m_rspUserLogin.FrontID, pInputOrder->OrderRef);
		auto it = m_orders.find(key);
		if (it == m_orders.end()) return;

		m_callback->rtnOrder(it->second, OST_Failed, 0);
	}
}

void CFTradeSpi::OnErrRtnOrderAction( CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo )
{
	// 撤单失败不处理
}
