#include "FollowCenter.h"

#include <fstream>
#include <cassert>

#include "../../XNYSTools/Interface/IConfigure.h"
#include "TargetGroup.h"
#include "TargetGroupRepository.h"
#include "TradeSystem.h"
#include "TradeSystemRepository.h"
#include "Strategy.h"
#include "StrategyRepository.h"
#include "../../xFollowStrategy/Interface/ITargetStrategyGroup.h"
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
	// 接口对照
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

	// 数据库有效性校验 -- 是否有无效数据

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
			getData(pRs, "ExchangeID", exchangeID, DT_STRING);
			getData(pRs, "ExchangeName", exchangeName, DT_STRING);
			getData(pRs, "System_ID", system_ID, DT_INT);

			pRs->MoveNext();
		}
		DB_QUERYSQL_END();
	}
	ADO_CATCH(false, sqltxt);
	return true;
}


bool CFollowCenter::loadProduct()
{
	return true;
}

bool CFollowCenter::loadInstrument()
{
	return true;
}

bool CFollowCenter::loadTradeSystem()
{
	if (!m_database.confirmConnect()) 
	{
		return false;
	}

	char sqltxt[1024] = {0};
	try
	{
		sprintf(sqltxt, "select * from TradeSystem");
		_RecordsetPtr pRs = nullptr;
		pRs = m_database.querySql(sqltxt);
		if (pRs == nullptr)
		{
			FOLLOW_LOG_ERROR("[加载数据库] 执行SQL失败");
			return false;
		}

		int _ID = 0;
		std::string name("");
		std::string ip1("");
		int port1 = 0;
		std::string ip2("");
		int port2 = 0;
		std::string ip3("");
		int port3 = 0;
		char status = '\0';
		while (pRs->adoEOF != VARIANT_TRUE)
		{
			getData(pRs, "ID", _ID, DT_INT);
			getData(pRs, "Name", name, DT_STRING);
			getData(pRs, "IP1", ip1, DT_STRING);
			getData(pRs, "Port1", port1, DT_INT);
			getData(pRs, "IP2", ip2, DT_STRING);
			getData(pRs, "Port2", port2, DT_INT);
			getData(pRs, "IP3", ip3, DT_STRING);
			getData(pRs, "Port3", port3, DT_INT);
			getData(pRs, "Status", status, DT_CHAR);

			CTradeSystemRepository::tradeSystemRepository().createTradeSystem(_ID, name, ip1, port1, ip2, port2, ip3, port3, status);

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
		sprintf(sqltxt, "select a.ID, a.AccountID, a.AccountName, a.AccountType, a.AccountPassword, a.Status, t.Api_ID "
			"from Account a, TradeSystem t where a.System_ID=t.ID");
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
		char accountType = '\0';
		std::string accountPassword("");
		char status = '\0';
		int api_ID = 0;
		IUser* user = nullptr;
		while (pRs->adoEOF != VARIANT_TRUE)
		{
			getData(pRs, "ID", _ID, DT_INT);
			getData(pRs, "AccountID", accountID, DT_STRING);
			getData(pRs, "AccountName", accountName, DT_STRING);
			getData(pRs, "AccountType", accountType, DT_CHAR);
			getData(pRs, "AccountPassword", accountPassword, DT_STRING);
			getData(pRs, "Status", status, DT_CHAR);
			getData(pRs, "Api_ID", api_ID, DT_INT);

			user = m_userRepository.loadUser(_ID, api_ID, accountType, accountID, accountPassword);
			m_userStatusControl.addUserInfo(user->isFollow(), user->id());

			pRs->MoveNext();
		}
		DB_QUERYSQL_END();
	}
	ADO_CATCH(false, sqltxt);
	return true;
}

bool CFollowCenter::loadTargetGroup()
{
	if (!m_database.confirmConnect()) 
	{
		return false;
	}

	char sqltxt[1024] = {0};
	try
	{
		sprintf(sqltxt, "select ag.Account_ID, ag.Group_ID, g.Status "
			"from AccountInGroup ag, Group g where ag.Group_ID=g.ID order by g.ID");
		_RecordsetPtr pRs = nullptr;
		pRs = m_database.querySql(sqltxt);
		if (pRs == nullptr)
		{
			FOLLOW_LOG_ERROR("[加载数据库] 执行SQL失败");
			return false;
		}

		int account_ID = 0;
		int group_ID = 0;
		char status = '\0';
		while (pRs->adoEOF != VARIANT_TRUE)
		{
			getData(pRs, "Account_ID", account_ID, DT_INT);
			getData(pRs, "Group_ID", group_ID, DT_INT);
			getData(pRs, "Status", status, DT_CHAR);

			CTargetGroup* targetGroup = CTargetGroupRepository::targetGroupRepository().createTargetGroup(group_ID, status);
			assert(targetGroup);
			targetGroup->addTargetUser(account_ID);

			pRs->MoveNext();
		}
		DB_QUERYSQL_END();
	}
	ADO_CATCH(false, sqltxt);
	return true;
}

bool CFollowCenter::loadFollowGroup()
{
	return true;
}

bool CFollowCenter::loadStrategy()
{
	if (!m_database.confirmConnect()) 
	{
		return false;
	}

	char sqltxt[1024] = {0};
	try
	{
		sprintf(sqltxt, "select * from Strategy");
		_RecordsetPtr pRs = nullptr;
		pRs = m_database.querySql(sqltxt);
		if (pRs == nullptr)
		{
			FOLLOW_LOG_ERROR("[加载数据库] 执行SQL失败");
			return false;
		}

		int _ID = 0;
		std::string strategyType("");
		std::string strategyData("");
		while (pRs->adoEOF != VARIANT_TRUE)
		{
			getData(pRs, "ID", _ID, DT_INT);
			getData(pRs, "StrategyType", strategyType, DT_STRING);
			getData(pRs, "StrategyData", strategyData, DT_STRING);

			CStrategyRepository::strategyRepository().createStrategy(_ID, strategyType, strategyData);

			pRs->MoveNext();
		}
		DB_QUERYSQL_END();
	}
	ADO_CATCH(false, sqltxt);
	return true;
}

bool CFollowCenter::loadRelation()
{
	return true;
}

void CFollowCenter::isSystemStarted(IUser* user, int id, bool successed)
{
	m_userStatusControl.addUserStatus(user->isFollow(), id, successed);
	if (m_userStatusControl.hasAllUserStatus())
	{
		int fsCount = m_userStatusControl.getFollowSuccessed();
		int tsCount = m_userStatusControl.getTargetSuccessed();
		int ffCount = m_userStatusControl.getFollowFailed();
		int tfCount = m_userStatusControl.getTargetFailed();
		FOLLOW_LOG_DEBUG("[账号准备情况] 跟随账号成功/失败(%d/%d) 目标账号成功/失败(%d/%d)", fsCount, ffCount, tsCount, tfCount);
		bool isStarted = fsCount * tsCount > 0;
		m_followHandle.startRsp(isStarted, 0); // 应该是系统启动
		if (isStarted)
		{
			FOLLOW_LOG_TRACE("[系统启动情况] 系统启动成功!");
		}
		else
		{
			FOLLOW_LOG_WARN("[系统启动情况] 系统启动失败!");
		}
	}
}

void CFollowCenter::init()
{
	bool rtn = false;
	do {
		// 检查环境
		rtn = checkEnvironment();
		if (!rtn) break;

		// 数据库建立连接
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
/*
	IUser* user = m_userRepository.addFollowUser(1, "f001", "8");
	m_userStatusControl.addUserInfo(true, user->id());
	ITargetStrategyGroup* targetGroup = ITargetStrategyGroup::createTargetStrategyGroup();
	targetGroup->registerSpi(&m_followHandle);
	targetGroup->addFollowUser(user);
	user->registerStrategyGroup(targetGroup);

	user = m_userRepository.addTargetUser(1, "t001", "8");
	m_userStatusControl.addUserInfo(false, user->id());
	targetGroup->addTargetUser(user);
	user->registerStrategyGroup(targetGroup);
*/

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

		isSystemStarted(user, id, successed);
	}
}

void CFollowCenter::rspUserInitialized(int id, bool successed, int errorID)
{
	IUser* user = m_userRepository.getUserByID(id);
	if (user == nullptr) {
		return;
	}

	m_userRepository.registerUser(successed, user);

	isSystemStarted(user, id, successed);
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
