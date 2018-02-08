#include "FollowUser.h"

#include "../../xFollowStrategy/Interface/ITargetStrategyGroup.h"

CFollowUser::CFollowUser( int id, int apiID, std::string& accountID, std::string& password )
	: m_id(id)
	, m_apiID(apiID)
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
	return m_id;
}

int CFollowUser::apiID()
{
	return m_apiID;
}

const char* CFollowUser::accountID()
{
	return m_accountID.c_str();
}

const char* CFollowUser::password()
{
	return m_password.c_str();
}

void CFollowUser::registerStrategyGroup(ITargetStrategyGroup* targetGroup)
{
	m_group = targetGroup;
}

void CFollowUser::rtnTrade( const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume )
{
	// ¸úËæÕËºÅÃ»ÓÃ
}

void CFollowUser::rtnPositionTotal( const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume )
{
	m_group->rtnTargetPositionTotal(m_id, productID, instrumentID, isBuy, hedgeFlag, volume);
}
