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
	bool loadConfig();
	bool initDatabase();
	bool loadDictionary();
	bool checkData();

	bool loadDatabase();
	bool loadExchange();
	bool loadProduct();
	bool loadInstrument();
	bool loadTradeSystem();
	bool loadUser();
	bool loadTargetGroup();
	bool loadFollowGroup();
	bool loadStrategy();
	bool loadRelation();

	void isSystemStarted(IUser* user, int id, bool successed);

	CFollowHandle&                                 m_followHandle;
	std::map<std::string, std::string>             m_apiNames;             // key -- path
	std::map<int, std::string>                     m_apiToNames;           // ID -- path
	CUserRepository                                m_userRepository;
	CUserStatusControl                             m_userStatusControl;

	CDatabaseConnection                            m_database;

	std::map<std::string, std::string>             m_dictionarys;

	bool                                           m_isStarted; // 系统是否启动成功
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
