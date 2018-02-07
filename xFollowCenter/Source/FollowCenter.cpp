#include "FollowCenter.h"

#include <fstream>
#include <cassert>

#include "../../XNYSTools/Interface/IConfigure.h"
#include "ITargetStrategyGroup.h"
#include "../../Include/X_MyLog.h"

CFollowCenter::CFollowCenter()
	: m_followHandle(*CFollowHandle::followHandle())
{

}

CFollowCenter::~CFollowCenter()
{

}

bool CFollowCenter::checkEnvironment()
{
	XNYSTools::IConfigure* pconfig = XNYSTools::IConfigure::createConfigure();
	if (pconfig == nullptr) return false;

	bool rtn = pconfig->openFile("./config/Plugin.conf");
	if (rtn) {
		while (!pconfig->isEnd()) {
			auto pp = pconfig->getValue();

			std::fstream file;
			file.open(pp.second, std::ios::in);
			if (file.good()) {
				m_apiNames[pp.first] = pp.second;
				file.close();
			}

			pconfig->moveNext();
		}
	}

	XNYSTools::IConfigure::destroyConfigure(pconfig);
	return rtn;
}


bool CFollowCenter::initDatabase()
{
	XNYSTools::IConfigure* pconfig = XNYSTools::IConfigure::createConfigure();
	if (pconfig == nullptr) return false;

	bool rtn = pconfig->openFile("./config/server.conf");
	if (rtn) {
		m_database.setConnectionString(pconfig->getValue("user"), pconfig->getValue("password"), pconfig->getValue("DBName"), pconfig->getValue("Address"));
		return m_database.confirmConnect();
	}

	return false;
}

bool CFollowCenter::loadDatabase()
{
	bool rtn = false;
	do {
		rtn = loadExchange();
		if (!rtn) break;

		rtn = loadUser();
		if (!rtn) break;

	} while (0);

	return rtn;
}

bool CFollowCenter::loadExchange()
{
	if (!m_database.confirmConnect()) 
	{
		return false;
	}

	char sqltxt[1024] = {0};
	try
	{
		sprintf(sqltxt, "select * from Exchange");
		_RecordsetPtr pRs = nullptr;
		pRs = m_database.querySql(sqltxt);
		if (pRs == nullptr)
		{
			FOLLOW_LOG_ERROR("[加载数据库] 执行SQL失败");
			return false;
		}

		std::string exchangeID("");
		std::string exchangeName("");
		int system_ID = 0;
		while (pRs->adoEOF != VARIANT_TRUE)
		{
			exchangeID = "";
			exchangeName = "";
			system_ID = 0;

			getData<std::string>(pRs, "ExchangeID", exchangeID, DT_STRING);
			getData<std::string>(pRs, "ExchangeName", exchangeName, DT_STRING);
			getData<int>(pRs, "System_ID", system_ID, DT_INT);

			pRs->MoveNext();
		}
		DB_QUERYSQL_END();
	}
	ADO_CATCH(false, sqltxt);
	return true;
}


bool CFollowCenter::loadUser()
{
	if (!m_database.confirmConnect()) 
	{
		return false;
	}

	char sqltxt[1024] = {0};
	try
	{
		sprintf(sqltxt, "select * from Account");
		_RecordsetPtr pRs = nullptr;
		pRs = m_database.querySql(sqltxt);
		if (pRs == nullptr)
		{
			FOLLOW_LOG_ERROR("[加载数据库] 执行SQL失败");
			return false;
		}

		int _ID = 0;
		std::string accountID("");
		std::string accountName("");
		int system_ID = 0;
		double test = 0;
		while (pRs->adoEOF != VARIANT_TRUE)
		{
			accountID = "";
			accountName = "";
			system_ID = 0;

			getData<std::string>(pRs, "AccountID", accountID, DT_STRING);
			getData<std::string>(pRs, "AccountName", accountName, DT_STRING);
			getData<int>(pRs, "SystemID", system_ID, DT_INT);
			getData<double>(pRs, "test", test, DT_DOUBLE);

			pRs->MoveNext();
		}
		DB_QUERYSQL_END();
	}
	ADO_CATCH(false, sqltxt);
	return true;
}

bool CFollowCenter::startSystem()
{
	int fsCount = m_userStatusControl.getFollowSuccessed();
	int tsCount = m_userStatusControl.getTargetSuccessed();
	int ffCount = m_userStatusControl.getFollowFailed();
	int tfCount = m_userStatusControl.getTargetFailed();
	FOLLOW_LOG_DEBUG("[用户准备情况] 跟随账号成功/失败(%d/%d) 目标账号成功/失败(%d/%d)", fsCount, ffCount, tsCount, tfCount);
	return fsCount * tsCount > 0;
}

void CFollowCenter::init()
{
	bool rtn = false;
	do {
		// 检查环境
		rtn = checkEnvironment();
		if (!rtn) break;

		rtn = initDatabase();
		if (!rtn) break;

		// 加载数据
		rtn = loadDatabase();
		if (!rtn) break;
	} while (0);

	m_followHandle.initRsp(rtn, 0);
}

void CFollowCenter::start()
{
	IUser* user = m_userRepository.addFollowUser(1, "127.0.0.1", 6666, "f001", "8");
	m_userStatusControl.addUserInfo(true, user->id());
	ITargetStrategyGroup* targetGroup = ITargetStrategyGroup::createTargetStrategyGroup();
	targetGroup->registerSpi(&m_followHandle);
	targetGroup->addFollowUser(user);
	user->registerStrategyGroup(targetGroup);

	user = m_userRepository.addTargetUser(1, "127.0.0.1", 6666, "t001", "8");
	m_userStatusControl.addUserInfo(false, user->id());
	targetGroup->addTargetUser(user);
	user->registerStrategyGroup(targetGroup);

	m_followHandle.registerApi("xCTPPlugin.dll", 1);
	m_followHandle.registerSpi(&m_followHandle);
	m_followHandle.reqUserLogin(1, 1, "127.0.0.1", 6666, "f001", "8");
	m_followHandle.reqUserLogin(2, 1, "127.0.0.1", 6666, "t001", "8");

/*
	for (auto& pp : m_apiToNames) {
		m_followHandle.registerApi(pp.second.c_str(), pp.first);
	}
	m_followHandle.registerSpi(&m_followHandle);

	auto users = m_userRepository.getAllUsers();
	for (auto& pp : users) {
		IUser* user = pp.second;
		assert(user);
		m_followHandle.reqUserLogin(user->id(), user->apiID(), user->ip(), user->port(), user->accountID(), user->password());
	}
*/
}

void CFollowCenter::stop()
{
}

//////////////////////////////////////////////////////////////////////////
void CFollowCenter::rspUserLogin(int id, bool successed, int errorID)
{
	if (!successed)
	{
		IUser* user = m_userRepository.getUserByID(id);
		if (user == nullptr) {
			return;
		}

		m_userRepository.unRegisterUser(user);

		m_userStatusControl.addUserStatus(user->isFollow(), id, successed);
		if (m_userStatusControl.hasAllUserStatus())
		{
			m_followHandle.startRsp(startSystem(), 0); // 应该是系统启动
		}
	}
}

void CFollowCenter::rspUserInitialized(int id, bool successed, int errorID)
{
	IUser* user = m_userRepository.getUserByID(id);
	if (user == nullptr) {
		return;
	}

	m_userRepository.registerUser(successed, user);

	m_userStatusControl.addUserStatus(user->isFollow(), id, successed);
	if (m_userStatusControl.hasAllUserStatus())
	{
		m_followHandle.startRsp(startSystem(), 0); // 应该是系统启动
	}
}

void CFollowCenter::rtnTrade( int id, const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume )
{ // targetUser ==> targetStrategyGroup ==> cal ==> followGroup
	IUser* user = m_userRepository.userByID(id);
	if (user == nullptr) {
		return;
	}

	user->rtnTrade(productID, instrumentID, isBuy, isOpen, hedgeFlag, volume);
}

void CFollowCenter::rtnPositionTotal( int id, const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume )
{
	IUser* user = m_userRepository.userByID(id);
	if (user == nullptr) {
		return;
	}

	user->rtnPositionTotal(productID, instrumentID, isBuy, hedgeFlag, volume);
}
