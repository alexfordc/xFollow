#include "TargetUser.h"


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
