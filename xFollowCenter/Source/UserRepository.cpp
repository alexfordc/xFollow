#include "UserRepository.h"

#include "FollowUser.h"
#include "TargetUser.h"


CUserRepository::CUserRepository()
{

}

CUserRepository::~CUserRepository()
{

}

void CUserRepository::addFollowUser( int apiID, const char* ip, int port, const char* accountID, const char* password )
{
	IUser* followUser = new CFollowUser;

	std::string key = std::string(accountID);
	m_users[key] = followUser;
}

void CUserRepository::addTargetUser( int apiID, const char* ip, int port, const char* accountID, const char* password )
{
	IUser* targetUser = new CTargetUser;

	std::string key = std::string(accountID);
	m_users[key] = targetUser;
}

std::map<std::string, IUser*> CUserRepository::getAllUsers()
{
	return m_users;
}
