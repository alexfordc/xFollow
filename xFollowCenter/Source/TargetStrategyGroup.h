#ifndef TargetGroup_H
#define TargetGroup_H

#include <map>

class IUser;
class ITargetStrategyGroupSpi;

class CTargetStrategyGroup
{
public:
	CTargetStrategyGroup();
	~CTargetStrategyGroup();

public:
	void addUser(IUser* user);
	void registerSpi(ITargetStrategyGroupSpi* spi);

	void rtnTrade(int id, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume);
	void rtnPositionTotal(int id, const char* instrumentID, bool isBuy, char hedgeFlag, int volume);

	void calculate(); // ¥•∑¢º∆À„

private:
	std::map<int, IUser*>        m_id2Users;
	ITargetStrategyGroupSpi*     m_spi;

	// group
	struct stuGroupPositionTotal
	{
		char instrumentID[32];
		char direction;
		char hedgeFlag;
		int  volume;
	};
	// id == <key, position>  key=instrumentID+'|'+direction+'|'+hedgeFlag;
	std::map<int, std::map<std::string, int>>         m_userPositionTotals;
	std::map<std::string, stuGroupPositionTotal>      m_groupPositionTotal;

	// strategy
};

#endif // TargetGroup_H
