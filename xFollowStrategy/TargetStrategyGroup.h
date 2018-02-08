#ifndef TargetGroup_H
#define TargetGroup_H

#include <list>
#include <map>
#include <set>
#include <string>

#include "Interface/ITargetStrategyGroup.h"

class IUser;
class ITargetStrategyGroupSpi;
class IFollowCenter;

class CTargetStrategyGroup : public ITargetStrategyGroup
{
public:
	CTargetStrategyGroup();
	virtual ~CTargetStrategyGroup();

public:
	virtual void registerSpi(ITargetStrategyGroupSpi* spi);
	virtual void addFollowUser(IUser* user);
	virtual void addTargetUser(IUser* user);

	virtual void rtnFollowPositionTotal(int id, const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume);
	virtual void rtnTrade(int id, const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume);
	virtual void rtnTargetPositionTotal(int id, const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume);

	virtual void calculate(); // 触发计算
	virtual void clear();

private:
	std::map<int, IUser*>        m_idF2Users;
	std::map<int, IUser*>        m_idT2Users;
	ITargetStrategyGroupSpi*     m_spi;

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
	int       m_strategyType;
	std::set<std::string> m_authProducts; // 约定: 空为不跟
	double    m_rate;
	bool      m_isSameDirection;
};

#endif // TargetGroup_H
