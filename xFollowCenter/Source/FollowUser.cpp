#include "FollowUser.h"

#include "TargetStrategyGroup.h"

CFollowUser::CFollowUser( int apiID, const char* ip, int port, const char* accountID, const char* password )
	: m_apiID(apiID)
	, m_port(port)
	, m_ip(ip)
	, m_accountID(accountID)
	, m_password(password)
{
}

CFollowUser::~CFollowUser()
{

}

bool CFollowUser::isFollow()
{
	return true;
}

bool CFollowUser::isTarget()
{
	return false;
}

int CFollowUser::id()
{
	return 0;
}

int CFollowUser::apiID()
{
	return 0;
}

const char* CFollowUser::ip()
{
	return "";
}

int CFollowUser::port()
{
	return 0;
}

const char* CFollowUser::accountID()
{
	return "";
}

const char* CFollowUser::password()
{
	return "";
}

void CFollowUser::rtnTrade( const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume )
{
	// ¸úËæÕËºÅÃ»ÓÃ
}

void CFollowUser::rtnPositionTotal( const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume )
{
	m_group->rtnTargetPositionTotal(m_id, productID, instrumentID, isBuy, hedgeFlag, volume);
}
