#include "MarketSystem.h"


CMarketSystem::CMarketSystem( int id )
	: m_id(id)
{

}

CMarketSystem::~CMarketSystem()
{

}

void CMarketSystem::setData( std::string name, int api_ID, std::string accountID, std::string password, std::string ip1, int port1, std::string ip2, int port2, std::string ip3, int port3 )
{
	m_name = name;
	m_api_ID = api_ID;
	m_accountID = accountID;
	m_password = password;
	m_ip1 = ip1;
	m_port1 = port1;
	m_ip2 = ip2;
	m_port2 = port2;
	m_ip3 = ip3;
	m_port3 = port3;
}

void CMarketSystem::setStatus( char status )
{
	m_status = status;
}

int CMarketSystem::id()
{
	return m_id;
}

std::string CMarketSystem::name()
{
	return m_name;
}

int CMarketSystem::api_ID()
{
	return m_api_ID;
}

std::string CMarketSystem::accountID()
{
	return m_accountID;
}

std::string CMarketSystem::password()
{
	return m_password;
}

std::string CMarketSystem::ip1()
{
	return m_ip1;
}

int CMarketSystem::port1()
{
	return m_port1;
}

std::string CMarketSystem::ip2()
{
	return m_ip2;
}

int CMarketSystem::port2()
{
	return m_port2;
}

std::string CMarketSystem::ip3()
{
	return m_ip3;
}

int CMarketSystem::port3()
{
	return m_port3;
}

char CMarketSystem::status()
{
	return m_status;
}
