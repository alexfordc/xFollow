#ifndef EventStruct_H
#define EventStruct_H

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
	int  id;
	int  apiID;
	char ip[32];
	int  port;
	char accountID[16];
	char password[32];
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
