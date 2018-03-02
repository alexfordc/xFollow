#ifndef InstrumentRepository_H
#define InstrumentRepository_H

#include <map>
#include <string>

#include "../Interface/IBaseDataRepository.h"

class CInstrumentRepository : public IInstrumentRepository
{
public:
	static CInstrumentRepository& instrumentRepository();
	virtual ~CInstrumentRepository();

	virtual void setInstrument(char marketType, const char* instrumentID, const char* instrumentName, const char* productID, const char* exchangeID, double priceTick);
	virtual IInstrument* getInstrument(char marketType, const char* instrumentID);
	virtual std::list<IInstrument*> getInstruments(char marketType);

	virtual void clear();
private:
	CInstrumentRepository();

	static CInstrumentRepository*            s_instance;

	// marketType+'|'+exchangeID <==> CInstrument
	std::map<std::string, IInstrument*>      m_instruments;
};

#endif // InstrumentRepository_H
