#include "InstrumentRepository.h"

#include "Instrument.h"

IInstrumentRepository& IInstrumentRepository::instrumentRepository()
{
	return CInstrumentRepository::instrumentRepository();
}
//////////////////////////////////////////////////////////////////////////
CInstrumentRepository* CInstrumentRepository::s_instance = nullptr;

CInstrumentRepository& CInstrumentRepository::instrumentRepository()
{
	if (s_instance == nullptr)
	{
		s_instance = new CInstrumentRepository;
	}
	return *s_instance;
}

CInstrumentRepository::CInstrumentRepository()
{

}

CInstrumentRepository::~CInstrumentRepository()
{

}

void CInstrumentRepository::setInstrument( char marketType, const char* instrumentID, const char* instrumentName, const char* productID, const char* exchangeID, double priceTick )
{
	std::string key = &marketType;
	key += "|";
	key += instrumentID;
	auto it = m_instruments.find(key);
	if (it == m_instruments.end())
	{
		IInstrument* instrument = new CInstrument(marketType, instrumentID, instrumentName, productID, exchangeID);
		instrument->setData(priceTick);
		m_instruments[key] = instrument;
	}
	else
	{
		it->second->setData(priceTick);
	}
}

IInstrument* CInstrumentRepository::getInstrument( char marketType, const char* instrumentID )
{
	std::string key = &marketType;
	key += "|";
	key += instrumentID;
	auto it = m_instruments.find(key);
	return it == m_instruments.end() ? nullptr : it->second;
}

std::list<IInstrument*> CInstrumentRepository::getInstruments( char marketType )
{
	std::list<IInstrument*> instruments;
	for (auto& i : m_instruments)
	{
		if (i.second != nullptr)
			instruments.push_back(i.second);
	}
	return instruments;
}

void CInstrumentRepository::clear()
{
	for (auto& e : m_instruments)
	{
		if (e.second != nullptr)
			delete e.second;
	}
	m_instruments.clear();
}
