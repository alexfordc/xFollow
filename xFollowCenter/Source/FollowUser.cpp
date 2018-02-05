#include "FollowUser.h"


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

void CFollowUser::rtnTrade( const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume )
{

}

void CFollowUser::rtnPositionTotal( const char* instrumentID, bool isBuy, char hedgeFlag, int volume )
{

}
