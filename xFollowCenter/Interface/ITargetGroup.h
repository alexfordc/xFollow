#ifndef ITargetGroup_H
#define ITargetGroup_H


class ITargetGroup
{
public:
	virtual ~ITargetGroup() {}

	virtual void setStatus(char status) = 0;
	virtual void addTargetUser(int accountID) = 0;
	virtual void removeTargetUser(int accountID) = 0;

	virtual void rtnTrade(int id, const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume) = 0;
	virtual void rtnTargetPositionTotal(int id, const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume) = 0;

	virtual void clear() = 0;
};

#endif // ITargetGroup_H
