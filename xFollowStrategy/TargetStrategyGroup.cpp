#include "TargetStrategyGroup.h"

#include <cassert>

#include "../xFollowCenter/Source/DataDefine.h"
#include "../xFollowCenter/Source/IUser.h"
#include "Interface/ITargetStrategyGroupSpi.h"
#include "../xFollowCenter/Interface/IFollowCenter.h"
#include "../Include/X_MyLog.h"


ITargetStrategyGroup* ITargetStrategyGroup::createTargetStrategyGroup()
{
	return new CTargetStrategyGroup;
}

//////////////////////////////////////////////////////////////////////////
CTargetStrategyGroup::CTargetStrategyGroup()
	: m_spi(nullptr)
	, m_groupID(0)
	, m_strategyID(0)
	, m_strategyType(STRATEGY_DEFAULT)
	, m_rate(0)
	, m_isSameDirection(true)
{

}

CTargetStrategyGroup::~CTargetStrategyGroup()
{

}

void CTargetStrategyGroup::addFollowUser( IUser* user )
{
	m_idF2Users[user->id()] = user;
	m_followUserPositionTotals[user->id()];
}

void CTargetStrategyGroup::addTargetUser( IUser* user )
{
	m_idT2Users[user->id()] = user;
	m_targetUserPositionTotals[user->id()];
}

void CTargetStrategyGroup::registerSpi(ITargetStrategyGroupSpi* spi)
{
	m_spi = spi;
}

void CTargetStrategyGroup::rtnFollowPositionTotal( int id, const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume )
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

void CTargetStrategyGroup::rtnTrade( int id, const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume )
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

// 	if (m_strategyType == STRATEGY_TRADE)
	{
		//
/*
		auto it = m_authProducts.find(productID);
		if (it == m_authProducts.end())
		{
			std::string insID = productID;
			insID += instrumentID;
			it = m_authProducts.find(insID);
			if (it == m_authProducts.end())
			{
				return; // 不跟
			}
		}
*/

		//
		bool is2Buy = m_isSameDirection ? isBuy : !isBuy;
		int tVolume = int(m_rate * volume);
		for (auto& fUserPP : m_idF2Users)
		{
			FOLLOW_LOG_TRACE("[策略模块] ID为%d的账号 成交跟单 %s 比率 %0.3f 合约 %s%s %s%s%d手", id, m_isSameDirection ? "同向" : "反向", m_rate, 
				productID, instrumentID, isBuy ? "买" : "卖", isOpen ? "开" : "平", tVolume);
			m_spi->reqPlaceOrder(fUserPP.first, productID, instrumentID, is2Buy, isOpen, hedgeFlag, tVolume);
		}
	}
}

void CTargetStrategyGroup::rtnTargetPositionTotal(int id, const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume)
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

void CTargetStrategyGroup::calculate()
{
}

void CTargetStrategyGroup::clear()
{

}
