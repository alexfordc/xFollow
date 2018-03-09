#include "Relation.h"

#include <cassert>

#include "../Include/X_MyLog.h"
#include "../xBaseDatas/Interface/IBaseDataRepository.h"
#include "../xFollowCenter/Source/DataDefine.h"


IRelation* createRelation(int id)
{
	return new CRelation(id);
}

void destroyRelation( IRelation* relation )
{
	delete relation;
}

//////////////////////////////////////////////////////////////////////////
CRelation::CRelation(int id)
	: m_id(id)
	, m_spi(nullptr)
	, m_marketType('\0')
	, m_orgStatus('0')
	, m_status('0')
	, m_strategy(nullptr)
	, m_groupID(0)
	, m_strategyID(0)
	, m_rate(0)

	, m_orderIndex(0)
{

}

CRelation::~CRelation()
{

}

int CRelation::id()
{
	return m_id;
}

void CRelation::addFollowUser(int id)
{
	m_followUserPositionTotals[id];
}

void CRelation::addTargetUser(int id)
{
	m_targetUserPositionTotals[id];
}

void CRelation::removeFollowUser(int id)
{
	m_followUserPositionTotals.erase(id);
}

void CRelation::removeTargetUser(int id)
{
	m_targetUserPositionTotals.erase(id);
}

void CRelation::setStrategy(IStrategy* strategy)
{
	m_strategy = strategy;
	m_dataStrategy.parser(strategy->getStrategy());
}

void CRelation::registerSpi(IStrategyResultSpi* spi)
{
	m_spi = spi;
}

void CRelation::setAuthProductID( std::string authProductID )
{
	m_dataStrategy.setAuthProductID(authProductID);
}

void CRelation::setRate( double rate )
{
	m_rate = rate;
}

void CRelation::setMarketType( char marketType )
{
	m_marketType = marketType;
}

void CRelation::setOrgStatus( char status )
{
	m_orgStatus = status;
}

void CRelation::setStatus(char status)
{
	m_status = status;
}

void CRelation::rtnFollowPositionTotal( int id, const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume )
{
	auto it = m_followUserPositionTotals.find(id);
	if (it != m_followUserPositionTotals.end())
	{ // find userid
		std::string key = productID;
		key += instrumentID;
		key += "|";
		key += isBuy ? "1" : "0";
		key += "|";
		key += hedgeFlag;
		auto& positionTotal = it->second;
		positionTotal[key] = volume;
	}
	else
	{ // not find user
		FOLLOW_LOG_ERROR("[策略模块] 此策略组中没有 跟随账号ID为%d的账号", id);
	}
}

void CRelation::rtnOrder( int orderIndex, char orderStatus, int volume )
{

}

void CRelation::rtnTrade( int id, const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume )
{
	auto it = m_targetUserPositionTotals.find(id);
	if (it != m_targetUserPositionTotals.end())
	{ // find userid
		std::string key = productID;
		key += instrumentID;
		key += "|";
		key += isBuy ? "1" : "0";
		key += "|";
		key += hedgeFlag;
		int deltaVolume = ((isOpen ? 1: -1) * volume);
		auto& positionTotal = it->second;
		auto itp = positionTotal.find(key);
		if (itp != positionTotal.end())
		{
			itp->second += deltaVolume;
		}
		else
		{
			if (!isOpen) return; // 没持仓不可能平仓...

			positionTotal[key] = volume;
		}

		auto gptIt = m_groupPositionTotal.find(key);
		if (gptIt != m_groupPositionTotal.end())
		{
			stuGroupPositionTotal& gpt = gptIt->second;
			gpt.volume += deltaVolume;
		}
		else
		{
			stuGroupPositionTotal& gpt = m_groupPositionTotal[key];
			gpt.instrumentID;
			gpt.isBuy = isBuy;
			gpt.hedgeFlag = hedgeFlag;
			gpt.volume = deltaVolume;
		}
	}
	else
	{ // not find user
		FOLLOW_LOG_ERROR("[策略模块] 此策略组中没有 目标账号ID为%d的账号", id);
		return;
	}

	if (m_orgStatus == '0' || m_status == '0') return; // 非启动状态

	IInstrument* instrument = IInstrumentRepository::instrumentRepository().getInstrument(m_marketType, instrumentID);
	if (nullptr == instrument) return;

// 	if (m_strategyType == STRATEGY_TRADE)
	{
		if (!m_dataStrategy.isInstrumentValid(productID, instrumentID))
		{
			return; // 不在授权范围内,不跟
		}

		//
		bool is2Buy = m_dataStrategy.isSameDirection() ? isBuy : !isBuy;
		int tVolume = int(m_rate * volume);
		int orderIndex = 0;
		for (auto& fUserPP : m_followUserPositionTotals)
		{
			orderIndex = ++m_orderIndex;
			FOLLOW_LOG_TRACE("[策略模块] 序号 %d ID为%d的账号 成交跟单 %s 比率 %0.3f 合约 %s%s %s%s%d手", orderIndex, id, m_dataStrategy.isSameDirection() ? "同向" : "反向", 
				m_rate, productID, instrumentID, isBuy ? "买" : "卖", isOpen ? "开" : "平", tVolume);
			if (tVolume == 0)
			{
				FOLLOW_LOG_TRACE("[策略模块] ID为%d的账号 成交跟单 转换数量为零不跟单", id);
			}
			else
			{
				m_spi->reqPlaceOrder(fUserPP.first, m_id, orderIndex, productID, instrumentID, is2Buy, isOpen, hedgeFlag, tVolume, instrument->bestPrice(is2Buy));
			}
		}
	}
}

void CRelation::rtnTargetPositionTotal(int id, const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume)
{
	auto it = m_targetUserPositionTotals.find(id);
	if (it != m_targetUserPositionTotals.end())
	{ // find userid
		std::string key = productID;
		key += instrumentID;
		key += "|";
		key += isBuy ? "1" : "0";
		key += "|";
		key += hedgeFlag;
		int deltaVolume = volume;
		auto& positionTotal = it->second;
		positionTotal[key] = volume;

		auto itp = positionTotal.find(key);
		if (itp != positionTotal.end())
		{
			deltaVolume = volume - itp->second;
		}

		if (deltaVolume == 0)
			return;

		auto gptIt = m_groupPositionTotal.find(key);
		if (gptIt != m_groupPositionTotal.end())
		{
			stuGroupPositionTotal& gpt = gptIt->second;
			gpt.volume += deltaVolume;
		}
		else
		{
			stuGroupPositionTotal& gpt = m_groupPositionTotal[key];
			gpt.instrumentID;
			gpt.isBuy = isBuy;
			gpt.hedgeFlag = hedgeFlag;
			gpt.volume = deltaVolume;
		}
	}
	else
	{ // not find user
		FOLLOW_LOG_ERROR("[策略模块] 此策略组中没有ID为%d的账号", id);
	}
}


void CRelation::start()
{
}

void CRelation::clear()
{

}
