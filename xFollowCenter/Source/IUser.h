#ifndef IUser_H
#define IUser_H

class ITargetStrategyGroup;

class IUser
{
public:
	virtual bool isFollow() = 0;
	virtual bool isTarget() = 0;
	virtual int id() = 0;
	virtual int apiID() = 0;
	virtual const char* ip() = 0;
	virtual int port() = 0;
	virtual const char* accountID() = 0;
	virtual const char* password() = 0;

	virtual void registerStrategyGroup(ITargetStrategyGroup* targetGroup) = 0;

public:
	virtual void rtnTrade(const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume) = 0;
	virtual void rtnPositionTotal(const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume) = 0;
};

#endif // IUser_H
