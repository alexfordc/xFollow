#ifndef ITraderSpi_H
#define ITraderSpi_H

class ITraderSpi
{
public:
	virtual void rspUserLogin() = 0;
	virtual void rspUserInitialized() = 0;

};

#endif // ITraderSpi_H
