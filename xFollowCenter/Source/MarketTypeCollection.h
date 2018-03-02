#ifndef MarketTypeCollection_H
#define MarketTypeCollection_H

#include <map>

class CMarketTypeCollection
{
public:
	static CMarketTypeCollection& marketTypeCollection();
	~CMarketTypeCollection();

	void setData(int api_ID, char marketType, char type);
	char marketType(int api_ID);

	void clear();

private:
	CMarketTypeCollection();
	static CMarketTypeCollection* s_instance;

	// api_ID <==> marketType
	std::map<int, std::pair<char, char>>       m_marketTypes;
};

#endif // MarketTypeCollection_H
