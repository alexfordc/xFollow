#include "TargetUser.h"

#include "TargetStrategyGroup.h"

CTargetUser::CTargetUser()
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

void CTargetUser::rtnTrade( const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume )
{
	m_group->rtnTrade(m_id, instrumentID, isBuy, isOpen, hedgeFlag, volume);
}

void CTargetUser::rtnPositionTotal(const char* instrumentID, bool isBuy, char hedgeFlag, int volume)
{
	m_group->rtnPositionTotal(m_id, instrumentID, isBuy, hedgeFlag, volume);
}
