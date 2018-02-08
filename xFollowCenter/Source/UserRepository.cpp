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

IUser* CUserRepository::loadUser( int id, int system_ID, char accountType, std::string& accountID, std::string& password )
{
	if (id > m_userid) m_userid = id;

	IUser* user = nullptr;
	if (accountType == '1')
	{
		user = addFollowUser(id, system_ID, accountID, password);
	}
	else
	{
		user = addTargetUser(id, system_ID, accountID, password);
	}
	return user;
}

IUser* CUserRepository::addUser( int system_ID, char accountType, std::string& accountID, std::string& password )
{
	IUser* user = nullptr;
	++m_userid;
	if (accountType == '1')
	{
		user = addFollowUser(m_userid, system_ID, accountID, password);
	}
	else
	{
		user = addTargetUser(m_userid, system_ID, accountID, password);
	}
	return user;
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

//////////////////////////////////////////////////////////////////////////
IUser* CUserRepository::addFollowUser( int id, int system_ID, std::string& accountID, std::string& password )
{
	IUser* followUser = new CFollowUser(id, system_ID, accountID, password);

	std::string key = std::string(accountID);
	m_users[key] = followUser;
	m_id2users[id] = followUser;
	return followUser;
}

IUser* CUserRepository::addTargetUser( int id, int system_ID, std::string& accountID, std::string& password )
{
	IUser* targetUser = new CTargetUser(id, system_ID, accountID, password);

	std::string key = std::string(accountID);
	m_users[key] = targetUser;
	m_id2users[id] = targetUser;
	return targetUser;
}
