#ifndef Relation_H
#define Relation_H

#include <list>
#include <map>
#include <set>
#include <string>

#include "../Include/X_IRelation.h"
#include "Strategy.h"

class CRelation : public IRelation
{
public:
	CRelation(int id);
	virtual ~CRelation();

public:
	virtual int  id();

	virtual void start();
	virtual void clear();
	virtual void registerSpi(IStrategyResultSpi* spi);

	virtual void setAuthProductID(std::string authProductID);
	virtual void setRate(double rate);
	virtual void setMarketType(char marketType);
	virtual void setOrgStatus(char status);
	virtual void setStatus(char status);

	virtual void addFollowUser(int id);
	virtual void addTargetUser(int id);
	virtual void removeFollowUser(int id);
	virtual void removeTargetUser(int id);

	virtual void setStrategy(IStrategy* strategy);

	virtual void rtnFollowPositionTotal(int id, const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume);
	virtual void rtnOrder(int orderIndex, char orderStatus, int volume);
	virtual void rtnTrade(int id, const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume);
	virtual void rtnTargetPositionTotal(int id, const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume);

private:
	int                       m_id;

	IStrategyResultSpi*       m_spi;
	char                      m_marketType;
	char                      m_orgStatus;
	char                      m_status;
	IStrategy*                m_strategy;

	// group
	struct stuGroupPositionTotal
	{
		char instrumentID[32];
		bool isBuy;
		char hedgeFlag;
		int  volume;
	};
	// id == <key, position>  key=instrumentID+'|'+direction+'|'+hedgeFlag;
	std::map<int, std::map<std::string, int>>         m_followUserPositionTotals;
	std::map<int, std::map<std::string, int>>         m_targetUserPositionTotals;
	std::map<std::string, stuGroupPositionTotal>      m_groupPositionTotal;

	// strategy
	int       m_groupID;
	int       m_strategyID;
	CStrategy m_dataStrategy;
	double    m_rate;

	int       m_orderIndex;
};

#endif // Relation_H
