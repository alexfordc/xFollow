#ifndef ITraderManageSpi_H
#define ITraderManageSpi_H

class ITraderManageSpi
{
public:
	virtual void rspUserLogin(int id, bool successed, int errorID) = 0;
	virtual void rspUserInitialized(int id, bool successed, int errorID) = 0;

	virtual void rspPlaceOrder() = 0;
	virtual void rspCancelOrder() = 0;
	virtual void rtnPositionTotal() = 0;
	virtual void rtnTrade(int id, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume) = 0;
	virtual void rtnPositionTotal(int id, const char* instrumentID, bool isBuy, char hedgeFlag, int volume) = 0;
};

#endif // ITraderManageSpi_H
