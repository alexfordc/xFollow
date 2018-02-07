#ifndef FollowCenter_H
#define FollowCenter_H

#include <map>
#include <string>

#include "FollowHandle.h"
#include "UserRepository.h"
#include "UserStatusControl.h"
#include "DatabaseConnection.h"

class CFollowCenter
{
public:
	CFollowCenter();
	~CFollowCenter();
public:

private:
	bool checkEnvironment();
	bool initDatabase();

	bool loadDatabase();
	bool loadExchange();
	bool loadProduct();
	bool loadInstrument();
	bool loadTargetGroup();
	bool loadFollowGroup();
	bool loadUser();
	bool loadStrategy();

	bool startSystem();

	CFollowHandle&                                 m_followHandle;
	std::map<std::string, std::string>             m_apiNames;             // key -- path
	std::map<int, std::string>                     m_apiToNames;           // ID -- path
	CUserRepository                                m_userRepository;
	CUserStatusControl                             m_userStatusControl;

	CDatabaseConnection                            m_database;

public:
	void init();
	void start();
	void stop();
public:
	void rspUserLogin(int id, bool successed, int errorID);
	void rspUserInitialized(int id, bool successed, int errorID);

	void rtnTrade(int id, const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume);
	void rtnPositionTotal(int id, const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume);
};

#endif // FollowCenter_H
