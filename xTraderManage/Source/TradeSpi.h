#ifndef TradeSpi_H
#define TradeSpi_H

#include "../Interface/ITrade.h"
#include "../Interface/ITraderManageSpi.h"

class CTradeSpi : public ITradeSpi
{
public:
	CTradeSpi();
	virtual ~CTradeSpi();

public:
	void registerID(int id);
	void registerSpi(ITraderManageSpi* spi);

private:
	int               m_id;
	ITraderManageSpi* m_spi;
private:
	virtual void rspUserLogin(bool successed, int errorID);
	virtual void rspUserInitialized(bool successed, int errorID);
};

#endif // TradeSpi_H
