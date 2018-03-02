#ifndef ITraderManageSpi_H
#define ITraderManageSpi_H

class ITraderManageSpi
{
public:
	virtual void rspUserLogin(int id, bool successed, int errorID) = 0;
	virtual void rspUserInitialized(int id, bool successed, int errorID) = 0;

	virtual void rtnOrder(int orderIndex, char orderStatus, int volume) = 0;
	virtual void rtnTrade(int id, const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume) = 0;
	virtual void rtnPositionTotal(int id, const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume) = 0;

	virtual void rspMUserLogin(char marketType, bool successed, int errorID) = 0;
	virtual void rtnMarketData(char marketType, const char* instrumentID, double lastPrice) = 0;
};

#endif // ITraderManageSpi_H
