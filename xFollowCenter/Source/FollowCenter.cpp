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
#include "../../Include/X_IRelation.h"
#include "RelationRepository.h"
#include "../../xFollowStrategy/Interface/ITargetStrategyGroup.h"
#include "../../Include/X_MyLog.h"

CFollowCenter::CFollowCenter()
	: m_followHandle(*CFollowHandle::followHandle())
{

}

CFollowCenter::~CFollowCenter()
{

}

bool CFollowCenter::loadConfig()
{
	// 文件数据读取
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

bool CFollowCenter::loadDictionary()
{
	if (!m_database.confirmConnect())
	{
		return false;
	}

	char sqltxt[1024] = { 0 };
	try
	{
		sprintf(sqltxt, "select * from Dictionary");
		_RecordsetPtr pRs = nullptr;
		pRs = m_database.querySql(sqltxt);
		if (pRs == nullptr)
		{
			FOLLOW_LOG_ERROR("[加载数据库] 执行SQL失败");
			return false;
		}

		std::string dicKey("");
		std::string dicValue("");
		while (pRs->adoEOF != VARIANT_TRUE)
		{
			getData(pRs, "DicKey", dicKey, DT_STRING);
			getData(pRs, "DicValue", dicValue, DT_STRING);

			m_dictionarys[dicKey] = dicValue;

			pRs->MoveNext();
		}
		DB_QUERYSQL_END();
	}
	ADO_CATCH(false, sqltxt);
	return true;
}

bool CFollowCenter::checkData()
{
	bool rtn = false;
	rtn = loadDictionary();
	if (!rtn) return false;

	// 数据有效性校验 -- 是否有无效数据
	int api_ID = 0;
	for (auto& anpp : m_apiNames)
	{
		auto itd = m_dictionarys.find(anpp.first);
		if (itd == m_dictionarys.end())
		{
			FOLLOW_LOG_WARN("[接口类型校验] 发现配置路径为 %s 的 %s 接口不在使用中", anpp.second.c_str(), anpp.first.c_str());
		}
		else
		{
			try
			{
				api_ID = std::stoi(itd->second);
				m_apiToNames[api_ID] = anpp.second;
			}
			catch (...)
			{
				FOLLOW_LOG_WARN("[接口类型校验] 发现配置路径为 %s 的 %s 接口在数据库中对照异常 (%s)", anpp.second.c_str(), anpp.first.c_str(), itd->second.c_str());
			}
		}
	}
	return true;
}

bool CFollowCenter::loadDatabase()
{
	bool rtn = false;
	do {
		rtn = loadExchange();
		if (!rtn) break;

		rtn = loadProduct();
		if (!rtn) break;

		rtn = loadInstrument();
		if (!rtn) break;

		rtn = loadTradeSystem();
		if (!rtn) break;

		rtn = loadUser();
		if (!rtn) break;

		rtn = loadTargetGroup();
		if (!rtn) break;

		rtn = loadFollowGroup();
		if (!rtn) break;

		rtn = loadStrategy();
		if (!rtn) break;

		rtn = loadRelation();
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
	if (!m_database.confirmConnect())
	{
		return false;
	}

	char sqltxt[1024] = { 0 };
	try
	{
		sprintf(sqltxt, "select * from Product");
		_RecordsetPtr pRs = nullptr;
		pRs = m_database.querySql(sqltxt);
		if (pRs == nullptr)
		{
			FOLLOW_LOG_ERROR("[加载数据库] 执行SQL失败");
			return false;
		}

		std::string productID("");
		std::string exchangeID("");
		int system_ID = 0;
		while (pRs->adoEOF != VARIANT_TRUE)
		{
			getData(pRs, "ProductID", productID, DT_STRING);
			getData(pRs, "ExchangeID", exchangeID, DT_STRING);
			getData(pRs, "System_ID", system_ID, DT_INT);

			pRs->MoveNext();
		}
		DB_QUERYSQL_END();
	}
	ADO_CATCH(false, sqltxt);
	return true;
}

bool CFollowCenter::loadInstrument()
{
	if (!m_database.confirmConnect())
	{
		return false;
	}

	char sqltxt[1024] = { 0 };
	try
	{
		sprintf(sqltxt, "select * from Instrument");
		_RecordsetPtr pRs = nullptr;
		pRs = m_database.querySql(sqltxt);
		if (pRs == nullptr)
		{
			FOLLOW_LOG_ERROR("[加载数据库] 执行SQL失败");
			return false;
		}

		std::string instrumentID("");
		std::string instrumentName("");
		std::string productID("");
		std::string exchangeID("");
		int system_ID = 0;
		while (pRs->adoEOF != VARIANT_TRUE)
		{
			getData(pRs, "InstrumentID", instrumentID, DT_STRING);
			getData(pRs, "InstrumentName", instrumentName, DT_STRING);
			getData(pRs, "ProductID", productID, DT_STRING);
			getData(pRs, "ExchangeID", exchangeID, DT_STRING);
			getData(pRs, "System_ID", system_ID, DT_INT);

			pRs->MoveNext();
		}
		DB_QUERYSQL_END();
	}
	ADO_CATCH(false, sqltxt);
	return true;
}

bool CFollowCenter::loadTradeSystem()
{
	if (!m_database.confirmConnect()) 
	{
		return false;
	}

	bool rtn = true;
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
		int api_ID = 0;
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
			getData(pRs, "Api_ID", api_ID, DT_INT);
			getData(pRs, "IP1", ip1, DT_STRING);
			getData(pRs, "Port1", port1, DT_INT);
			getData(pRs, "IP2", ip2, DT_STRING);
			getData(pRs, "Port2", port2, DT_INT);
			getData(pRs, "IP3", ip3, DT_STRING);
			getData(pRs, "Port3", port3, DT_INT);
			getData(pRs, "Status", status, DT_CHAR);

			auto itan = m_apiToNames.find(api_ID);
			if (itan == m_apiToNames.end())
			{
				FOLLOW_LOG_ERROR("[加载数据] 没有匹配到接口编号为 %d 的", api_ID);
				rtn = false;
			}
			else
			{
				CTradeSystemRepository::tradeSystemRepository().createTradeSystem(_ID, name, api_ID, ip1, port1, ip2, port2, ip3, port3, status);
			}

			pRs->MoveNext();
		}
		DB_QUERYSQL_END();
	}
	ADO_CATCH(false, sqltxt);
	return rtn;
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
		sprintf(sqltxt, "select a.ID, a.AccountID, a.AccountName, a.AccountType, a.AccountPassword, a.Status, a.System_ID from Account a");
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
		int system_ID = 0;
		IUser* user = nullptr;
		while (pRs->adoEOF != VARIANT_TRUE)
		{
			getData(pRs, "ID", _ID, DT_INT);
			getData(pRs, "AccountID", accountID, DT_STRING);
			getData(pRs, "AccountName", accountName, DT_STRING);
			getData(pRs, "AccountType", accountType, DT_CHAR);
			getData(pRs, "AccountPassword", accountPassword, DT_STRING);
			getData(pRs, "Status", status, DT_CHAR);
			getData(pRs, "System_ID", system_ID, DT_INT);

			user = m_userRepository.loadUser(_ID, system_ID, accountType, accountID, accountPassword);
			assert(user);
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
		sprintf(sqltxt, "select ag.Account_ID, ag.Group_ID, ag.Status as AccountStatus, g.Status "
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
		char accountStatus = '\0';
		char status = '\0';
		while (pRs->adoEOF != VARIANT_TRUE)
		{
			getData(pRs, "Account_ID", account_ID, DT_INT);
			getData(pRs, "Group_ID", group_ID, DT_INT);
			getData(pRs, "AccountStatus", accountStatus, DT_CHAR);
			getData(pRs, "Status", status, DT_CHAR);

			CTargetGroup* targetGroup = CTargetGroupRepository::targetGroupRepository().createTargetGroup(group_ID, status);
			assert(targetGroup);
			targetGroup->addTargetUser(account_ID, accountStatus);

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
	if (!m_database.confirmConnect())
	{
		return false;
	}

	char sqltxt[1024] = { 0 };
	try
	{
		sprintf(sqltxt, "select * from Relation");
		_RecordsetPtr pRs = nullptr;
		pRs = m_database.querySql(sqltxt);
		if (pRs == nullptr)
		{
			FOLLOW_LOG_ERROR("[加载数据库] 执行SQL失败");
			return false;
		}

		int _ID = 0;
		int account_ID = 0;
		int targetGroup_ID = 0;
		int strategy_ID = 0;
		IStrategy* strategy = nullptr;
		char status = '\0';
		while (pRs->adoEOF != VARIANT_TRUE)
		{
			getData(pRs, "ID", _ID, DT_INT);
			getData(pRs, "Account_ID", account_ID, DT_INT);
			getData(pRs, "Group_ID", targetGroup_ID, DT_INT);
			getData(pRs, "Strategy_ID", strategy_ID, DT_INT);
			getData(pRs, "Status", status, DT_CHAR);

			IRelation* relation = CRelationRepository::relationRepository().createRelation(_ID, status);
			relation->addFollowUser(account_ID);
			CTargetGroup* targetGroup = CTargetGroupRepository::targetGroupRepository().getTargetGroup(targetGroup_ID);
			auto account_IDs = targetGroup->getAccount_IDs();
			for (int aid : account_IDs)
			{
				relation->addTargetUser(aid);
			}
			strategy = CStrategyRepository::strategyRepository().getStrategy(strategy_ID);
			relation->setStrategy(strategy);
			relation->setStatus(status);

			pRs->MoveNext();
		}
		DB_QUERYSQL_END();
	}
	ADO_CATCH(false, sqltxt);
	return true;
}

//////////////////////////////////////////////////////////////////////////
void CFollowCenter::init()
{
	bool rtn = false;
	do {
		// 加载配置文件
		rtn = loadConfig();
		if (!rtn) break;

		// 数据库建立连接
		rtn = initDatabase();
		if (!rtn) break;

		// 检查数据
		rtn = checkData();
		if (!rtn) break;

		// 加载数据
		rtn = loadDatabase();
		if (!rtn) break;
	} while (0);

	m_followHandle.initRsp(rtn, 0);
}

void CFollowCenter::start()
{
	for (auto& pp : m_apiToNames) {
		m_followHandle.registerApi(pp.second.c_str(), pp.first);
	}
	m_followHandle.registerSpi(&m_followHandle);

	CTradeSystem* tradeSystem = nullptr;
	auto users = m_userRepository.getAllUsers();
	for (auto& pp : users) {
		IUser* user = pp.second;
		assert(user);
		tradeSystem = CTradeSystemRepository::tradeSystemRepository().getTradeSystem(user->system_ID());
		assert(tradeSystem);
		m_followHandle.reqUserLogin(user->id(), tradeSystem->api_ID(), tradeSystem->ip1().c_str(), tradeSystem->port1(), user->accountID(), user->password());
	}
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

//////////////////////////////////////////////////////////////////////////
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
