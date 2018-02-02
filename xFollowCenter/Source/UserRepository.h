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
	int  addFollowUser(int apiID, const char* ip, int port, const char* accountID, const char* password);
	int  addTargetUser(int apiID, const char* ip, int port, const char* accountID, const char* password);

	std::map<std::string, IUser*> getAllUsers();
	IUser* getUserByID(int id);

	void registerUser(bool successed, IUser* user); // 只记录完成初始化的用户
	void unRegisterUser(IUser* user);   // 
	IUser* userByID(int id);
private:
	int                               m_userid;
	std::map<std::string, IUser*>     m_users;
	std::map<int, IUser*>             m_id2users;

	std::map<int, IUser*>             m_id2usersUsed; // 只记录完成初始化的用户
};

#endif // UserRepository_H
