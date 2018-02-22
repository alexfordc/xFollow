#ifndef TTradeSpi_H
#define TTradeSpi_H

#include "../Interface/ITTrade.h"
#include "../Interface/ITraderManageSpi.h"

class CTTradeSpi : public ITTradeSpi
{
public:
	CTTradeSpi();
	virtual ~CTTradeSpi();

public:
	void registerID(int id);
	void registerSpi(ITraderManageSpi* spi);

private:
	int               m_id;
	ITraderManageSpi* m_spi;
private:
	virtual void rspUserLogin(bool successed, int errorID);
	virtual void rspUserInitialized(bool successed, int errorID);

	virtual void rtnTrade(const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume);
	virtual void rtnPositionTotal(const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume);
};

#endif // TTradeSpi_H
