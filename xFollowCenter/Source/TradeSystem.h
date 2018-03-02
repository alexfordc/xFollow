#ifndef TradeSystem_H
#define TradeSystem_H

#include <string>

class CTradeSystem
{
public:
	CTradeSystem(int id);
	~CTradeSystem();

public:
	void setData(std::string name, int api_ID, std::string ip1, int port1, std::string ip2, int port2, std::string ip3, int port3);
	void setStatus(char status);
	void setOrgID(int org_ID);

	int         id();
	std::string name();
	int         api_ID();
	std::string ip1();
	int         port1();
	std::string ip2();
	int         port2();
	std::string ip3();
	int         port3();
	char        status();
	int         orgID();

private:
	int             m_id;
	std::string     m_name;
	int             m_api_ID;
	std::string     m_ip1;
	int             m_port1;
	std::string     m_ip2;
	int             m_port2;
	std::string     m_ip3;
	int             m_port3;
	char            m_status;
	int             m_org_ID;
};

#endif // TradeSystem_H
