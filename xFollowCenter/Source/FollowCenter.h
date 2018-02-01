#ifndef FollowCenter_H
#define FollowCenter_H

#include <map>
#include <string>

#include "FollowHandle.h"
#include "UserRepository.h"
#include "UserStatusControl.h"

class CFollowCenter
{
public:
	CFollowCenter();
	~CFollowCenter();
public:

private:
	bool checkEnvironment();
	bool loadDatabase();

	CFollowHandle&                                 m_followHandle;
	std::map<std::string, std::string>             m_apiNames;             // key -- path
	std::map<int, std::string>                     m_apiToNames;           // ID -- path
	CUserRepository                                m_userRepository;
	CUserStatusControl                             m_userStatusControl;

public:
	void init();
	void start();
	void stop();
public:
	void rspUserLogin(int id, bool successed, int errorID);
	void rspUserInitialized(int id, bool successed, int errorID);
};

#endif // FollowCenter_H
