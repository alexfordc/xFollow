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

struct stuPlaceOrderEvent
{
	int id;
	int relationID;
	int orderIndex;
	char productID[32];
	char instrumentID[32];
	bool isBuy;
	bool isOpen;
	char hedgeFlag;
	int volume;
	double price;
};

struct stuRtnOrderEvent
{
	int orderIndex;
	char orderStatus;
	int volume;
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

struct stuRtnPositionEvent
{
	int  id;
	char productID[16];
	char instrumentID[16];
	bool isBuy;
	char hedgeFlag;
	int  volume;
};

//////////////////////////////////////////////////////////////////////////
struct stuMUserLoginEvent
{
	x_stuMUserLogin muserLogin;
};

struct stuMUserNotifyEvent
{
	char marketType;
	bool successed;
	int  errorID;
};

struct stuRtnMarketDataEvent
{
	char marketType;
	char instrumentID[16];
	double lastPrice;
};

#endif // EventStruct_H
