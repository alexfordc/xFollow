#ifndef MarketSystemRepository_H
#define MarketSystemRepository_H

#include <list>
#include <map>

class CMarketSystem;

class CMarketSystemRepository
{
public:
	static CMarketSystemRepository& marketSystemRepository();

	CMarketSystem* createMarketSystem(int id, std::string name, int api_ID, std::string accountID, std::string password, 
		std::string ip1, int port1, std::string ip2, int port2, std::string ip3, int port3, char status);
	CMarketSystem* getMarketSystem(int id);
	std::list<CMarketSystem*> getMarketSystems();

	void clear();

private:
	static CMarketSystemRepository*       s_instance;

	std::map<int, CMarketSystem*>         m_marketSystems;
};

#endif // MarketSystemRepository_H
