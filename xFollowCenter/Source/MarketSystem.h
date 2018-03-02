#ifndef MarketSystem_H
#define MarketSystem_H

#include <string>

class CMarketSystem
{
public:
	CMarketSystem(int id);
	~CMarketSystem();

public:
	void setData(std::string name, int api_ID, std::string accountID, std::string password, std::string ip1, int port1, std::string ip2, int port2, std::string ip3, int port3);
	void setStatus(char status);

	int         id();
	std::string name();
	int         api_ID();
	std::string accountID();
	std::string password();
	std::string ip1();
	int         port1();
	std::string ip2();
	int         port2();
	std::string ip3();
	int         port3();
	char        status();

private:
	int             m_id;
	std::string     m_name;
	int             m_api_ID;
	std::string     m_accountID;
	std::string     m_password;
	std::string     m_ip1;
	int             m_port1;
	std::string     m_ip2;
	int             m_port2;
	std::string     m_ip3;
	int             m_port3;
	char            m_status;
};

#endif // MarketSystem_H
