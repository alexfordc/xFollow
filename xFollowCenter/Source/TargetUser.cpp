#include "TargetUser.h"

#include "TargetStrategyGroup.h"

CTargetUser::CTargetUser( int apiID, const char* ip, int port, const char* accountID, const char* password )
	: m_apiID(apiID)
	, m_port(port)
	, m_ip(ip)
	, m_accountID(accountID)
	, m_password(password)
{
}

CTargetUser::~CTargetUser()
{

}

bool CTargetUser::isFollow()
{
	return false;
}

bool CTargetUser::isTarget()
{
	return true;
}

int CTargetUser::id()
{
	return 0;
}

int CTargetUser::apiID()
{
	return 0;
}

const char* CTargetUser::ip()
{
	return "";
}

int CTargetUser::port()
{
	return 0;
}

const char* CTargetUser::accountID()
{
	return "";
}

const char* CTargetUser::password()
{
	return "";
}

void CTargetUser::rtnTrade( const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume )
{
	m_group->rtnTrade(m_id, productID, instrumentID, isBuy, isOpen, hedgeFlag, volume);
}

void CTargetUser::rtnPositionTotal(const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume)
{
	m_group->rtnTargetPositionTotal(m_id, productID, instrumentID, isBuy, hedgeFlag, volume);
}
