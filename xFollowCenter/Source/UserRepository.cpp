#include "UserRepository.h"

#include "FollowUser.h"
#include "TargetUser.h"


CUserRepository::CUserRepository()
	: m_userid(0)
{

}

CUserRepository::~CUserRepository()
{

}

int CUserRepository::addFollowUser( int apiID, const char* ip, int port, const char* accountID, const char* password )
{
	++m_userid;
	IUser* followUser = new CFollowUser;

	std::string key = std::string(accountID);
	m_users[key] = followUser;
	m_id2users[m_userid] = followUser;
	return m_userid;
}

int CUserRepository::addTargetUser( int apiID, const char* ip, int port, const char* accountID, const char* password )
{
	++m_userid;
	IUser* targetUser = new CTargetUser;

	std::string key = std::string(accountID);
	m_users[key] = targetUser;
	m_id2users[m_userid] = targetUser;
	return m_userid;
}

std::map<std::string, IUser*> CUserRepository::getAllUsers()
{
	return m_users;
}

IUser* CUserRepository::getUserByID(int id)
{
	auto it = m_id2users.find(id);
	return it == m_id2users.end() ? nullptr : it->second;
}

void CUserRepository::registerUser( bool successed, IUser* user )
{
	if (successed)
		m_id2usersUsed[user->id()] = user;
	else
		this->unRegisterUser(user);
}

void CUserRepository::unRegisterUser( IUser* user )
{
	m_id2usersUsed.erase(user->id());
}

IUser* CUserRepository::userByID( int id )
{
	auto it = m_id2usersUsed.find(id);
	return it == m_id2usersUsed.end() ? nullptr : it->second;
}
