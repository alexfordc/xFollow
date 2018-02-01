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
	virtual void rtnTrade() = 0;
};

#endif // ITraderManageSpi_H
