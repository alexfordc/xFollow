#ifndef Instrument_H
#define Instrument_H

#include "../Interface/IBaseDataRepository.h"

class CInstrument : public IInstrument
{
public:
	CInstrument(char marketType, const char* instrumentID, const char* instrumentName, const char* productID, const char* exchangeID);
	virtual ~CInstrument();

	virtual void setData(double priceTick);

	virtual char marketType();
	virtual const char* instrumentID();
	virtual const char* instrumentName();
	virtual const char* productID();
	virtual const char* exchangeID();
	virtual double priceTick();

	// 行情数据
	virtual void lastPrice(double value);
	virtual double lastPrice();

	virtual double bestPrice(bool isBuy);
private:
	char   m_marketType;
	char   m_instrumentID[32];
	char   m_instrumentName[32];
	char   m_productID[32];
	char   m_exchangeID[32];
	double m_priceTick;

	double m_lastPrice;
};

#endif // Instrument_H
