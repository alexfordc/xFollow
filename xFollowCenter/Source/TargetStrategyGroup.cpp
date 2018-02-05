#include "TargetStrategyGroup.h"

#include "DataDefine.h"
#include "IUser.h"
#include "TargetStrategyGroupSpi.h"

#include "../../Include/X_MyLog.h"

CTargetStrategyGroup::CTargetStrategyGroup()
{

}

CTargetStrategyGroup::~CTargetStrategyGroup()
{

}

void CTargetStrategyGroup::addUser( IUser* user )
{
	m_id2Users[user->id()] = user;
}

void CTargetStrategyGroup::registerSpi(ITargetStrategyGroupSpi* spi)
{
	m_spi = spi;
}

void CTargetStrategyGroup::rtnTrade( int id, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume )
{
	auto it = m_userPositionTotals.find(id);
	if (it != m_userPositionTotals.end())
	{ // find userid
		std::string key = instrumentID;
		key += "|";
		key += isBuy ? "1" : "0";
		key += "|";
		key += hedgeFlag;
		auto& positionTotal = it->second;
		auto itp = positionTotal.find(key);
		if (itp != positionTotal.end())
		{
			itp->second += ((isOpen ? 1: -1) * volume);
		}
		else
		{
			if (!isOpen) return; // 没持仓不可能平仓...

			positionTotal[key] = volume;
		}
	}
	else
	{ // not find user
		FOLLOW_LOG_ERROR("[策略模块] 此策略组中没有ID为%d的账号", id);
	}
	if (STRATEGY_TRADE)
	{
		//
	}
}

void CTargetStrategyGroup::rtnPositionTotal(int id, const char* instrumentID, bool isBuy, char hedgeFlag, int volume)
{
	auto it = m_userPositionTotals.find(id);
	if (it != m_userPositionTotals.end())
	{ // find userid
		std::string key = instrumentID;
		key += "|";
		key += isBuy ? "1" : "0";
		key += "|";
		key += hedgeFlag;
		auto& positionTotal = it->second;
		auto itp = positionTotal.find(key);
		if (itp != positionTotal.end())
		{
			itp->second += ((isOpen ? 1: -1) * volume);
		}
		else
		{
			if (!isOpen) return; // 没持仓不可能平仓...

			positionTotal[key] = volume;
		}

		positionTotal[key] = volume;
	}
	else
	{ // not find user
		FOLLOW_LOG_ERROR("[策略模块] 此策略组中没有ID为%d的账号", id);
	}
}

void CTargetStrategyGroup::calculate()
{
}
