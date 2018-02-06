#ifndef FollowUser_H
#define FollowUser_H

#include "IUser.h"

#include <string>

class CTargetStrategyGroup;

class CFollowUser : public IUser
{
public:
	CFollowUser(int apiID, const char* ip, int port, const char* accountID, const char* password);
	virtual ~CFollowUser();
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
	virtual void rtnTrade(const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume);
	virtual void rtnPositionTotal(const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume);
};

#endif // FollowUser_H
