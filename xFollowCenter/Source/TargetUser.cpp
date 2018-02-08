#include "TargetUser.h"

#include "../../xFollowStrategy/Interface/ITargetStrategyGroup.h"

CTargetUser::CTargetUser(int id, int apiID, std::string& accountID, std::string& password )
	: m_id(id)
	, m_apiID(apiID)
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
	return m_id;
}

int CTargetUser::apiID()
{
	return m_apiID;
}

const char* CTargetUser::accountID()
{
	return m_accountID.c_str();
}

const char* CTargetUser::password()
{
	return m_password.c_str();
}

void CTargetUser::registerStrategyGroup(ITargetStrategyGroup* targetGroup)
{
	m_group = targetGroup;
}

void CTargetUser::rtnTrade( const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume )
{
	m_group->rtnTrade(m_id, productID, instrumentID, isBuy, isOpen, hedgeFlag, volume);
}

void CTargetUser::rtnPositionTotal(const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume)
{
	m_group->rtnTargetPositionTotal(m_id, productID, instrumentID, isBuy, hedgeFlag, volume);
}
