#include "TargetUser.h"

#include "TargetGroup.h"

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

void CTargetUser::rtnTrade( const char* instrumentID, char direction, char offerset, char hedgeFlag, int volume )
{
	m_group->calculate(this);
}
