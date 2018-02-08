#ifndef TargetGroup_H
#define TargetGroup_H

#include <list>
#include <map>
#include <set>
#include <string>

#include "../Interface/ITargetGroup.h"

class IUser;

class CTargetGroup : public ITargetGroup
{
public:
	CTargetGroup(int id);
	virtual ~CTargetGroup();

public:
	std::list<int> getAccount_IDs();

public:
	virtual void setStatus(char status);
	virtual void addTargetUser(int accountID, char status);
	virtual void removeTargetUser(int accountID);

	virtual void rtnTrade(int id, const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume);
	virtual void rtnTargetPositionTotal(int id, const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume);

	virtual void clear();

private:
	int       m_groupID;
	char      m_status;

	struct stuGroupPositionTotal
	{
		char instrumentID[32];
		bool isBuy;
		char hedgeFlag;
		int  volume;
	};
	// id == <key, position>  key=instrumentID+'|'+direction+'|'+hedgeFlag;
	std::map<int, bool>                               m_accountStatus;
	std::map<int, std::map<std::string, int>>         m_targetUserPositionTotals;
	std::map<std::string, stuGroupPositionTotal>      m_groupPositionTotal;
};

#endif // TargetGroup_H
