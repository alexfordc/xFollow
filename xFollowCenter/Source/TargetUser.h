#ifndef TargetUser_H
#define TargetUser_H

#include "IUser.h"

#include <string>

class CTargetUser : public IUser
{
public:
	CTargetUser(int id, int system_ID, std::string& accountID, std::string& password);
	virtual ~CTargetUser();

public:
	virtual bool isFollow();
	virtual bool isTarget();

public:
	virtual void rtnTrade(const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume);
	virtual void rtnPositionTotal(const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume);
};

#endif // TargetUser_H
