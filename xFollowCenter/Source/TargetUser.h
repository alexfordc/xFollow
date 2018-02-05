#ifndef TargetUser_H
#define TargetUser_H

#include "IUser.h"

#include <string>

class CTargetStrategyGroup;

class CTargetUser : public IUser
{
public:
	CTargetUser();
	virtual ~CTargetUser();

public:
	virtual bool isFollow();
	virtual bool isTarget();
	virtual int id();
	virtual int apiID();
	virtual const char* ip();
	virtual int port();
	virtual const char* accountID();
	virtual const char* password();

private:
	bool            m_isFollow;
	int             m_id;
	int             m_apiID;
	std::string     m_ip;
	int             m_port;
	std::string     m_accountID;
	std::string     m_password;

	CTargetStrategyGroup*   m_group;

public:
	virtual void rtnTrade(const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume);
	virtual void rtnPositionTotal(const char* instrumentID, bool isBuy, char hedgeFlag, int volume);
};

#endif // TargetUser_H
