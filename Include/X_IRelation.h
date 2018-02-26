#ifndef IRelation_H
#define IRelation_H

#include "X_DllExport.h"

#ifdef	XFOLLOW_STRATEGY_EXPORTS
#define XFOLLOW_STRATEGY_EXPORTS_C  DLL_EXPORT_CLASS_DECL
#define XFOLLOW_STRATEGY_EXPORTS_A  DLL_EXPORT_C_DECL
#else
#define XFOLLOW_STRATEGY_EXPORTS_C  DLL_IMPORT_CLASS_DECL
#define XFOLLOW_STRATEGY_EXPORTS_A  DLL_IMPORT_C_DECL
#endif

#include "../xFollowCenter/Interface/IStrategy.h"
#include "X_IStrategyResultSpi.h"

class IRelation
{
public:
	virtual ~IRelation() {};

	virtual void start() = 0; // @note 主要用来表明数据发送完成
	virtual void clear() = 0;
	virtual void registerSpi(IStrategyResultSpi* spi) = 0;

	virtual void setStatus(char status) = 0;

	virtual void addFollowUser(int id) = 0;
	virtual void addTargetUser(int id) = 0;
	virtual void removeFollowUser(int id) = 0;
	virtual void removeTargetUser(int id) = 0;

	virtual void setStrategy(IStrategy* strategy) = 0;

	virtual void rtnFollowPositionTotal(int id, const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume) = 0;
	virtual void rtnOrder(int orderIndex, char status, int volume) = 0;
	virtual void rtnTrade(int id, const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume) = 0;
	virtual void rtnTargetPositionTotal(int id, const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume) = 0;

};

XFOLLOW_STRATEGY_EXPORTS_A IRelation* createRelation(int id);

#endif // IRelation_H
