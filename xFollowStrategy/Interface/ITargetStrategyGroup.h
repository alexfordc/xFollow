#ifndef ITargetStrategyGroup_H
#define ITargetStrategyGroup_H

class ITargetStrategyGroupSpi;
class IUser;

class ITargetStrategyGroup
{
public:
	static ITargetStrategyGroup* createTargetStrategyGroup();
	virtual ~ITargetStrategyGroup() {}

	virtual void registerSpi(ITargetStrategyGroupSpi* spi) = 0;
	virtual void addFollowUser(IUser* user) = 0;
	virtual void addTargetUser(IUser* user) = 0;

	virtual void rtnFollowPositionTotal(int id, const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume) = 0;
	virtual void rtnTrade(int id, const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume) = 0;
	virtual void rtnTargetPositionTotal(int id, const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume) = 0;

	virtual void calculate() = 0; // ¥•∑¢º∆À„
	virtual void clear() = 0;

};

#endif // ITargetStrategyGroup_H
