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
	IUser* loadUser(int id, int apiID, char accountType, std::string& accountID, std::string& password);
	IUser* addUser(int apiID, char accountType, std::string& accountID, std::string& password);

	std::map<std::string, IUser*> getAllUsers();
	IUser* getUserByID(int id);

	void registerUser(bool successed, IUser* user); // ֻ��¼��ɳ�ʼ�����û�
	void unRegisterUser(IUser* user);   // 
	IUser* userByID(int id);
private:
	IUser* addFollowUser(int id, int apiID, std::string& accountID, std::string& password);
	IUser* addTargetUser(int id, int apiID, std::string& accountID, std::string& password);

	int                               m_userid;
	std::map<std::string, IUser*>     m_users;
	std::map<int, IUser*>             m_id2users;

	std::map<int, IUser*>             m_id2usersUsed; // ֻ��¼��ɳ�ʼ�����û�
};

#endif // UserRepository_H
