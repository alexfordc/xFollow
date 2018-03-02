#ifndef TradeSystemRepository_H
#define TradeSystemRepository_H

#include <map>

class CTradeSystem;

class CTradeSystemRepository
{
public:
	static CTradeSystemRepository& tradeSystemRepository();

	CTradeSystem* createTradeSystem(int id, std::string name, int api_ID, 
		std::string ip1, int port1, std::string ip2, int port2, std::string ip3, int port3, char status, int org_ID);
	CTradeSystem* getTradeSystem(int id);

	void clear();

private:
	static CTradeSystemRepository*       s_instance;

	std::map<int, CTradeSystem*>         m_tradeSystems;
};

#endif // TradeSystemRepository_H
