#include "TradeSystem.h"


CTradeSystem::CTradeSystem( int id )
	: m_id(id)
{

}

CTradeSystem::~CTradeSystem()
{

}

void CTradeSystem::setData( std::string name, int api_ID, std::string ip1, int port1, std::string ip2, int port2, std::string ip3, int port3 )
{
	m_name = name;
	m_api_ID = api_ID;
	m_ip1 = ip1;
	m_port1 = port1;
	m_ip2 = ip2;
	m_port2 = port2;
	m_ip3 = ip3;
	m_port3 = port3;
}

void CTradeSystem::setStatus( char status )
{
	m_status = status;
}

int CTradeSystem::id()
{
	return m_id;
}

std::string CTradeSystem::name()
{
	return m_name;
}

int CTradeSystem::api_ID()
{
	return m_api_ID;
}

std::string CTradeSystem::ip1()
{
	return m_ip1;
}

int CTradeSystem::port1()
{
	return m_port1;
}

std::string CTradeSystem::ip2()
{
	return m_ip2;
}

int CTradeSystem::port2()
{
	return m_port2;
}

std::string CTradeSystem::ip3()
{
	return m_ip3;
}

int CTradeSystem::port3()
{
	return m_port3;
}

char CTradeSystem::status()
{
	return m_status;
}
