#ifndef FollowUser_H
#define FollowUser_H

#include "IUser.h"

#include <string>

class CFollowUser : public IUser
{
public:
	CFollowUser(int id, int system_ID, std::string& accountID, std::string& password);
	virtual ~CFollowUser();

public:
	virtual bool isFollow();
	virtual bool isTarget();

public:
	virtual void rtnTrade(const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume);
	virtual void rtnPositionTotal(const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume);
};

#endif // FollowUser_H
