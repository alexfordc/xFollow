#ifndef EventStruct_H
#define EventStruct_H

#include "../../Include/X_Define.h"

struct stuNotifyEvent
{
	bool successed;
	int  errorID;
};

struct stuRegisterApiEvent
{
	char apiName[256];
	int  apiID;
};

struct stuUserLoginEvent
{
	x_stuUserLogin userLogin;
};

struct stuUserNotifyEvent
{
	int  id;
	bool successed;
	int  errorID;
};

struct stuRtnTradeEvent
{
	int  id;
	char productID[16];
	char instrumentID[16];
	bool isBuy;
	bool isOpen;
	char hedgeFlag;
	int  volume;
};


#endif // EventStruct_H
