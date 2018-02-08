#ifndef TargetUser_H
#define TargetUser_H

#include "IUser.h"

#include <string>

class CTargetUser : public IUser
{
public:
	CTargetUser(int id, int apiID, std::string& accountID, std::string& password);
	virtual ~CTargetUser();

public:
	virtual bool isFollow();
	virtual bool isTarget();
	virtual int id();
	virtual int apiID();
	virtual const char* accountID();
	virtual const char* password();

	virtual void registerStrategyGroup(ITargetStrategyGroup* targetGroup);

private:
	bool            m_isFollow;
	int             m_id;
	int             m_apiID;
	std::string     m_accountID;
	std::string     m_password;

	ITargetStrategyGroup*   m_group;

public:
	virtual void rtnTrade(const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume);
	virtual void rtnPositionTotal(const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume);
};

#endif // TargetUser_H
