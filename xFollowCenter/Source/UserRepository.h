#ifndef UserRepository_H
#define UserRepository_H

#include "IUser.h"

#include <map>
#include <string>

class CUserRepository
{
public:
	CUserRepository();
	~CUserRepository();

public:
	void addFollowUser(int apiID, const char* ip, int port, const char* accountID, const char* password);
	void addTargetUser(int apiID, const char* ip, int port, const char* accountID, const char* password);

	std::map<std::string, IUser*> getAllUsers();
	IUser* getUserByID(int id);

private:
	int                               m_userid;
	std::map<std::string, IUser*>     m_users;
	std::map<int, IUser*>             m_id2users;
};

#endif // UserRepository_H
