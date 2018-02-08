#include "Relation.h"

#include <cassert>

#include "../xFollowCenter/Source/DataDefine.h"
#include "../Include/X_MyLog.h"


IRelation& IRelation::createRelation()
{
	return *(new CRelation);
}

//////////////////////////////////////////////////////////////////////////
CRelation::CRelation()
	: m_spi(nullptr)
	, m_groupID(0)
	, m_strategyID(0)
	, m_strategyType(STRATEGY_DEFAULT)
	, m_rate(0)
	, m_isSameDirection(true)
{

}

CRelation::~CRelation()
{

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
}

void CRelation::registerSpi(IStrategyResultSpi* spi)
{
	m_spi = spi;
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
		FOLLOW_LOG_ERROR("[����ģ��] �˲�������û�� �����˺�IDΪ%d���˺�", id);
	}
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
			if (!isOpen) return; // û�ֲֲ�����ƽ��...

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
		FOLLOW_LOG_ERROR("[����ģ��] �˲�������û�� Ŀ���˺�IDΪ%d���˺�", id);
		return;
	}

// 	if (m_strategyType == STRATEGY_TRADE)
	{
		if (!m_strategy->isInstrumentValid(productID, instrumentID))
		{
			return; // ������Ȩ��Χ��,����
		}

		//
		bool is2Buy = m_isSameDirection ? isBuy : !isBuy;
		int tVolume = int(m_rate * volume);
		for (auto& fUserPP : m_followUserPositionTotals)
		{
			FOLLOW_LOG_TRACE("[����ģ��] IDΪ%d���˺� �ɽ����� %s ���� %0.3f ��Լ %s%s %s%s%d��", id, m_isSameDirection ? "ͬ��" : "����", m_rate, 
				productID, instrumentID, isBuy ? "��" : "��", isOpen ? "��" : "ƽ", tVolume);
			m_spi->reqPlaceOrder(fUserPP.first, productID, instrumentID, is2Buy, isOpen, hedgeFlag, tVolume);
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
		FOLLOW_LOG_ERROR("[����ģ��] �˲�������û��IDΪ%d���˺�", id);
	}
}

void CRelation::start()
{
}

void CRelation::clear()
{

}
