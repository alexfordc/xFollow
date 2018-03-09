#include "FollowCenter.h"

#include <fstream>
#include <cassert>

#include "../../XNYSTools/Interface/IConfigure.h"
#include "../../Include/X_MyLog.h"
#include "../../Include/X_IRelation.h"
#include "../../xBaseDatas/Interface/IBaseDataRepository.h"
#include "TargetGroup.h"
#include "TargetGroupRepository.h"
#include "TradeSystem.h"
#include "TradeSystemRepository.h"
#include "MarketSystem.h"
#include "MarketSystemRepository.h"
#include "Strategy.h"
#include "StrategyRepository.h"
#include "RelationRepository.h"
#include "OrganizationRepository.h"
#include "MarketTypeCollection.h"

CFollowCenter::CFollowCenter()
	: m_followHandle(*CFollowHandle::followHandle())
	, m_isStarted(false)
	, m_thread(nullptr)
	, m_isJsonRpc(false)
{
	strncpy_s(m_http_port, "8000", sizeof(m_http_port));
}

CFollowCenter::~CFollowCenter()
{

}

bool CFollowCenter::loadConfig()
{
	// 文件数据读取
	XNYSTools::IConfigure* pconfig = XNYSTools::IConfigure::createConfigure();
	if (pconfig == nullptr) return false;

	bool rtn = pconfig->openFile("./config/ApiPlugin.conf");
	if (rtn) {
		while (!pconfig->isEnd()) {
			auto pp = pconfig->getValue();

			std::fstream file;
			file.open(pp.second, std::ios::in);
			if (file.good()) {
				FOLLOW_LOG_DEBUG("[加载插件配置] 接口%s 对应文件%s", pp.first.c_str(), pp.second.c_str());
				m_apiNames[pp.first] = pp.second;
				file.close();
			}

			pconfig->moveNext();
		}
	}
	pconfig->close();

	rtn = pconfig->openFile("./config/RelationPlugin.conf");
	if (rtn) {
		while (!pconfig->isEnd()) {
			auto pp = pconfig->getValue();

			std::fstream file;
			file.open(pp.second, std::ios::in);
			if (file.good()) {
				FOLLOW_LOG_DEBUG("[加载关系配置] 类型%s 对应文件%s", pp.first.c_str(), pp.second.c_str());
				CRelationRepository::relationRepository().setRelationModule(pp.first, pp.second);
				file.close();
			}

			pconfig->moveNext();
		}
	}
	pconfig->close();

	XNYSTools::IConfigure::destroyConfigure(pconfig);

	return rtn;
}

bool CFollowCenter::initDatabase()
{
	XNYSTools::IConfigure* pconfig = XNYSTools::IConfigure::createConfigure();
	if (pconfig == nullptr) return false;

	bool rtn = pconfig->openFile("./config/server.conf");
	if (rtn) {
		std::string addr = pconfig->getValue("Address");
		std::string dbName = pconfig->getValue("DBName");
		std::string user = pconfig->getValue("user");
		std::string password = pconfig->getValue("password");
		FOLLOW_LOG_DEBUG("[加载系统配置] 地址 %s 数据库名 %s 用户名 %s", addr.c_str(), dbName.c_str(), user.c_str());
		m_database.setConnectionString(user, password, dbName, addr);
		rtn = m_database.confirmConnect();
	}
	pconfig->close();

	XNYSTools::IConfigure::destroyConfigure(pconfig);

	return rtn;
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

		rtn = loadMarketType();
		if (!rtn) break;

		rtn = loadMarketSystem();
		if (!rtn) break;

		rtn = loadOrganization();
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
		char marketType = '\0';
		while (pRs->adoEOF != VARIANT_TRUE)
		{
			getData(pRs, "ExchangeID", exchangeID, DT_STRING);
			getData(pRs, "ExchangeName", exchangeName, DT_STRING);
			getData(pRs, "MarketType", marketType, DT_CHAR);

			IExchangeRepository::exchangeRepository().setExchange(marketType, exchangeID.c_str(), exchangeName.c_str());

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
		std::string productName("");
		std::string exchangeID("");
		char marketType = '\0';
		while (pRs->adoEOF != VARIANT_TRUE)
		{
			getData(pRs, "ProductID", productID, DT_STRING);
			getData(pRs, "ProductName", productName, DT_STRING);
			getData(pRs, "ExchangeID", exchangeID, DT_STRING);
			getData(pRs, "MarketType", marketType, DT_CHAR);

			IProductRepository::productRepository().setProduct(marketType, productID.c_str(), productName.c_str(), exchangeID.c_str());

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
		double priceTick = 0;
		char marketType = '\0';
		while (pRs->adoEOF != VARIANT_TRUE)
		{
			getData(pRs, "InstrumentID", instrumentID, DT_STRING);
			getData(pRs, "InstrumentName", instrumentName, DT_STRING);
			getData(pRs, "ProductID", productID, DT_STRING);
			getData(pRs, "ExchangeID", exchangeID, DT_STRING);
			getData(pRs, "PriceTick", priceTick, DT_DOUBLE);
			getData(pRs, "MarketType", marketType, DT_CHAR);

			IInstrumentRepository::instrumentRepository().setInstrument(marketType, instrumentID.c_str(), instrumentName.c_str(), 
				productID.c_str(), exchangeID.c_str(), priceTick);

			pRs->MoveNext();
		}
		DB_QUERYSQL_END();
	}
	ADO_CATCH(false, sqltxt);
	return true;
}

bool CFollowCenter::loadMarketType()
{
	if (!m_database.confirmConnect())
	{
		return false;
	}

	char sqltxt[1024] = { 0 };
	try
	{
		sprintf(sqltxt, "select * from MarketType");
		_RecordsetPtr pRs = nullptr;
		pRs = m_database.querySql(sqltxt);
		if (pRs == nullptr)
		{
			FOLLOW_LOG_ERROR("[加载数据库] 执行SQL失败");
			return false;
		}

		int api_ID = 0;
		char marketType = '\0';
		char type = '\0';
		while (pRs->adoEOF != VARIANT_TRUE)
		{
			getData(pRs, "Api_ID", api_ID, DT_INT);
			getData(pRs, "MarketType", marketType, DT_CHAR);
			getData(pRs, "Type", type, DT_CHAR);

			CMarketTypeCollection::marketTypeCollection().setData(api_ID, marketType, type);

			pRs->MoveNext();
		}
		DB_QUERYSQL_END();
	}
	ADO_CATCH(false, sqltxt);
	return true;
}

bool CFollowCenter::loadMarketSystem()
{
	if (!m_database.confirmConnect()) 
	{
		return false;
	}

	bool rtn = true;
	char sqltxt[1024] = {0};
	try
	{
		sprintf(sqltxt, "select * from MarketSystem");
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
		std::string accountID("");
		std::string password("");
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
			getData(pRs, "AccountID", accountID, DT_STRING);
			getData(pRs, "Password", password, DT_STRING);
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
				CMarketSystemRepository::marketSystemRepository().createMarketSystem(_ID, name, api_ID, accountID, password, ip1, port1, ip2, port2, ip3, port3, status);
			}

			pRs->MoveNext();
		}
		DB_QUERYSQL_END();
	}
	ADO_CATCH(false, sqltxt);
	return rtn;
}

bool CFollowCenter::loadOrganization()
{
	if (!m_database.confirmConnect())
	{
		return false;
	}

	char sqltxt[1024] = { 0 };
	try
	{
		sprintf(sqltxt, "select * from Organization");
		_RecordsetPtr pRs = nullptr;
		pRs = m_database.querySql(sqltxt);
		if (pRs == nullptr)
		{
			FOLLOW_LOG_ERROR("[加载数据库] 执行SQL失败");
			return false;
		}

		int _ID = 0;
		std::string orgName("");
		char status = '\0';
		while (pRs->adoEOF != VARIANT_TRUE)
		{
			getData(pRs, "ID", _ID, DT_INT);
			getData(pRs, "OrgName", orgName, DT_STRING);
			getData(pRs, "Status", status, DT_CHAR);

			COrganizationRepository::organizationRepository()->setOrganization(_ID, orgName, status);

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
		int org_ID = 0;
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
			getData(pRs, "Org_ID", org_ID, DT_INT);

			auto itan = m_apiToNames.find(api_ID);
			if (itan == m_apiToNames.end())
			{
				FOLLOW_LOG_ERROR("[加载数据] 没有匹配到接口编号为 %d 的", api_ID);
				rtn = false;
			}
			else
			{
				CTradeSystemRepository::tradeSystemRepository().createTradeSystem(_ID, name, api_ID, ip1, port1, ip2, port2, ip3, port3, status, org_ID);
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
			"from AccountInGroup ag, TargetGroup g where ag.Group_ID=g.ID order by g.ID");
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
		std::string authProductID;
		double rate = 0;
		char status = '\0';
		int org_ID = 0;
		while (pRs->adoEOF != VARIANT_TRUE)
		{
			getData(pRs, "ID", _ID, DT_INT);
			getData(pRs, "Account_ID", account_ID, DT_INT);
			getData(pRs, "Group_ID", targetGroup_ID, DT_INT);
			getData(pRs, "Strategy_ID", strategy_ID, DT_INT);
			getData(pRs, "AuthProductID", authProductID, DT_STRING);
			getData(pRs, "Rate", rate, DT_DOUBLE);
			getData(pRs, "Status", status, DT_CHAR);
			getData(pRs, "Org_ID", org_ID, DT_INT);

			FOLLOW_LOG_DEBUG("[加载关系数据] 组 %d 关系ID %d 的Account_ID %d 账号跟随组 %d 使用策略 %d 跟单 授权品种 %d 比率为 %f 状态为 %c", 
				org_ID, _ID, account_ID, targetGroup_ID, strategy_ID, authProductID.c_str(), rate, status);
			do
			{
				IStrategy* strategy = CStrategyRepository::strategyRepository().getStrategy(strategy_ID);
				if (nullptr == strategy)
				{
					FOLLOW_LOG_WARN("[加载关系数据] 未找到ID为 %d 的策略模块", strategy_ID);
					break;
				}
				COrganization* org = COrganizationRepository::organizationRepository()->getOrganization(org_ID);
				if (nullptr == org)
				{
					FOLLOW_LOG_WARN("[加载关系数据] 未找到ID为 %d 的机构模块", org_ID);
					break;
				}

				IRelation* relation = CRelationRepository::relationRepository().createRelation(_ID, strategy->strategyType(), org_ID);
				if (nullptr == relation)
				{
					FOLLOW_LOG_WARN("[加载关系数据] 未找到ID为 %d 的关系模块", _ID);
					break;
				}
				relation->registerSpi(&m_followHandle);

				relation->addFollowUser(account_ID);
				IUser* followUser = m_userRepository.getUserByID(account_ID);
				assert(followUser);
				followUser->setRelationID(_ID);

				CTargetGroup* targetGroup = CTargetGroupRepository::targetGroupRepository().getTargetGroup(targetGroup_ID);
				auto account_IDs = targetGroup->getAccount_IDs();
				for (int aid : account_IDs)
				{
					relation->addTargetUser(aid);
					IUser* targetUser = m_userRepository.getUserByID(aid);
					assert(targetUser);
					targetUser->setRelationID(_ID);
				}
				CTradeSystem* tradeSystem = CTradeSystemRepository::tradeSystemRepository().getTradeSystem(followUser->system_ID());
				assert(tradeSystem);
				char marketType = CMarketTypeCollection::marketTypeCollection().marketType(tradeSystem->api_ID());
				relation->setStrategy(strategy);
				relation->setMarketType(marketType);
				relation->setOrgStatus(org->status());
				relation->setStatus(status);
				relation->setAuthProductID(authProductID);
				relation->setRate(rate);
			} while (0);

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

	if (!rtn)
	{
		clear();
		m_followHandle.initFaild();
	}
	m_followHandle.initRsp(rtn, 0);
}

void CFollowCenter::start()
{
	for (auto& pp : m_apiToNames) {
		m_followHandle.registerApi(pp.second.c_str(), pp.first);
	}
	m_followHandle.registerSpi(&m_followHandle);

	x_stuMUserLogin muserLogin = {0};
	auto marketSystems = CMarketSystemRepository::marketSystemRepository().getMarketSystems();
	for (auto& m : marketSystems)
	{
		assert(m);
		char marketType = CMarketTypeCollection::marketTypeCollection().marketType(m->api_ID());
		muserLogin.marketSystemID = m->id();
		muserLogin.marketType = marketType;
		muserLogin.apiID = m->api_ID();
		strncpy_s(muserLogin.ip1, m->ip1().c_str(), sizeof(muserLogin.ip1));
		muserLogin.port1 = m->port1();
		strncpy_s(muserLogin.ip2, m->ip2().c_str(), sizeof(muserLogin.ip2));
		muserLogin.port2 = m->port2();
		strncpy_s(muserLogin.ip3, m->ip3().c_str(), sizeof(muserLogin.ip3));
		muserLogin.port3 = m->port3();
		strncpy_s(muserLogin.accountID, m->accountID().c_str(), sizeof(muserLogin.accountID));
		strncpy_s(muserLogin.password, m->password().c_str(), sizeof(muserLogin.password));
		auto instruments = IInstrumentRepository::instrumentRepository().getInstruments(marketType);
		for (auto& i : instruments)
		{
			muserLogin.instruments.push_back(i->instrumentID());
		}
		m_followHandle.reqUserLogin(muserLogin);
	}

	CTradeSystem* tradeSystem = nullptr;
	x_stuUserLogin userLogin = {0};
	auto users = m_userRepository.getAllUsers();
	for (auto& pp : users) {
		IUser* user = pp.second;
		assert(user);
		tradeSystem = CTradeSystemRepository::tradeSystemRepository().getTradeSystem(user->system_ID());
		assert(tradeSystem);

		userLogin.id = user->id();
		userLogin.apiID = tradeSystem->api_ID();
		userLogin.isFollow = user->isFollow();
		strncpy_s(userLogin.ip1, tradeSystem->ip1().c_str(), sizeof(userLogin.ip1));
		userLogin.port1 = tradeSystem->port1();
		strncpy_s(userLogin.ip2, tradeSystem->ip2().c_str(), sizeof(userLogin.ip2));
		userLogin.port2 = tradeSystem->port2();
		strncpy_s(userLogin.ip3, tradeSystem->ip3().c_str(), sizeof(userLogin.ip3));
		userLogin.port3 = tradeSystem->port3();
		strncpy_s(userLogin.accountID, user->accountID(), sizeof(userLogin.accountID));
		strncpy_s(userLogin.password, user->password(), sizeof(userLogin.password));
		m_followHandle.reqUserLogin(userLogin);
	}
}

void CFollowCenter::clear()
{
	m_isJsonRpc = false;
	if (nullptr != m_thread)
	{
		m_thread->join();
		delete m_thread;
		m_thread = nullptr;
	}
	m_apiNames.clear();
	m_apiToNames.clear();
	m_userRepository.clear();
	m_userStatusControl.clear();
	m_dictionarys.clear();
	m_isStarted = false;

	IExchangeRepository::exchangeRepository().clear();
	IProductRepository::productRepository().clear();
	IInstrumentRepository::instrumentRepository().clear();

	COrganizationRepository::organizationRepository()->clear();
	CRelationRepository::relationRepository().clear();
	CStrategyRepository::strategyRepository().clear();
	CTargetGroupRepository::targetGroupRepository().clear();

	CMarketSystemRepository::marketSystemRepository().clear();
	CTradeSystemRepository::tradeSystemRepository().clear();
	CMarketTypeCollection::marketTypeCollection().clear();
}

void CFollowCenter::stop()
{
	clear();
	m_followHandle.stopRsp();
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

		FOLLOW_LOG_DEBUG("[登录状态响应] 账号 %s 登录%s", user->accountID(), successed ? "成功" : "失败");
		if (!m_isStarted)
		{
			m_userRepository.unRegisterUser(user);

			isSystemStarted(user, id, successed);
		}
	}
}

void CFollowCenter::rspUserInitialized(int id, bool successed, int errorID)
{
	IUser* user = m_userRepository.getUserByID(id);
	if (user == nullptr) {
		return;
	}

	FOLLOW_LOG_DEBUG("[登录状态响应] 账号 %s 初始化%s", user->accountID(), successed ? "完成" : "错误");
	if (!m_isStarted)
	{
		m_userRepository.registerUser(successed, user);

		isSystemStarted(user, id, successed);
	}
}

void CFollowCenter::rtnOrder( int relationID, int orderIndex, char orderStatus, int volume )
{
	if (!m_isStarted) return;

	IRelation* relation = CRelationRepository::relationRepository().getRelation(relationID);
	if (nullptr == relation) {
		FOLLOW_LOG_ERROR("[跟单回报] 未找到对应的跟单关系");
		return;
	}

	FOLLOW_LOG_DEBUG("[收到委托推送] 关系 %d 序号 %d 状态 %c %d手", relation->id(), orderIndex, orderStatus, volume);
	relation->rtnOrder(orderIndex, orderStatus, volume);
}

void CFollowCenter::rtnTrade( int id, const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume )
{
	if (!m_isStarted) return;

	IUser* user = m_userRepository.userByID(id);
	if (user == nullptr) {
		return;
	}

	FOLLOW_LOG_DEBUG("[收到成交推送] 账号 %s 品种 %s 合约 %s 投保标志 %c %s%s%d手", 
		user->accountID(), productID, instrumentID, hedgeFlag, isBuy ? "买" : "卖", isOpen ? "开" : "平", volume);
	user->rtnTrade(productID, instrumentID, isBuy, isOpen, hedgeFlag, volume);
}

void CFollowCenter::rtnPositionTotal( int id, const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume )
{
	IUser* user = m_userRepository.userByID(id);
	if (user == nullptr) {
		return;
	}

	FOLLOW_LOG_DEBUG("[收到持仓推送] 账号 %s 品种 %s 合约 %s 投保标志 %c %s持仓%d手", 
		user->accountID(), productID, instrumentID, hedgeFlag, isBuy ? "多" : "空", volume);
	user->rtnPositionTotal(productID, instrumentID, isBuy, hedgeFlag, volume);
}

void CFollowCenter::rspMUserLogin( char marketType, bool successed, int errorID )
{
	FOLLOW_LOG_DEBUG("[行情登陆响应] 市场 %c 的账号登陆 %s", marketType, successed ? "成功" : "失败");
}

void CFollowCenter::rtnMarketData( char marketType, const char* instrumentID, double lastPrice )
{
	IInstrument* instrument = IInstrumentRepository::instrumentRepository().getInstrument(marketType, instrumentID);
	if (nullptr == instrument)
	{
		// 说明这个合约的行情交易端没有
	}
	else
	{
		instrument->lastPrice(lastPrice);
	}
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
		m_isStarted = fsCount * tsCount > 0;
		m_followHandle.startRsp(m_isStarted, 0); // 应该是系统启动
		if (m_isStarted)
		{
			this->startListen();
			FOLLOW_LOG_TRACE("[系统状态] 系统启动成功!");
		}
		else
		{
			FOLLOW_LOG_WARN("[系统状态] 系统启动失败!");
			m_followHandle.startFaild();
		}
	}
}

void CFollowCenter::startListen()
{
	ns_mgr_init(&m_mgr, NULL);
	m_conn = ns_bind(&m_mgr, m_http_port, handler);
	ns_set_protocol_http_websocket(m_conn);

	FOLLOW_LOG_DEBUG("[管理模块] 启动对端口 %s 的侦听\n", m_http_port);
	if (nullptr != m_thread)
	{
		m_thread->join();
		delete m_thread;
		m_thread = nullptr;
	}
	if (nullptr == m_thread)
	{
		m_isJsonRpc = true;
		m_thread = new std::thread(std::bind(&CFollowCenter::run, this));
	}
}

int CFollowCenter::run()
{
	while (m_isJsonRpc)
	{
		ns_mgr_poll(&m_mgr, 1000);
	}
	ns_mgr_free(&m_mgr);
	return 0;
}

void CFollowCenter::handler( struct ns_connection *nc, int ev, void *ev_data )
{
	struct http_message *hm = (struct http_message *) ev_data;
	static const char *methods[] = { "sum", "login", NULL };
	static ns_rpc_handler_t handlers[] = { rpc_sum, rpcLogin, NULL };
	char buf[100] = {0};
	switch (ev) {
	case NS_HTTP_REQUEST:
		ns_rpc_dispatch(hm->body.p, hm->body.len, buf, sizeof(buf),
			methods, handlers);
		ns_printf(nc, "HTTP/1.0 200 OK\r\nContent-Length: %d\r\n"
			"Content-Type: application/json\r\n\r\n%s",
			(int) strlen(buf), buf);
		nc->flags |= NSF_SEND_AND_CLOSE;
		break;
	default:
		break;
	}
}

std::string CFollowCenter::str2str( const char* value )
{
	std::string rst;
	int len = strlen(value);
	bool isFirstSpace = true;
	bool isPlus = false;
	for (int i = 0; i < len; ++i)
	{
		const char& v = value[i];
		if (isFirstSpace && v == ' ')
			continue;

		isFirstSpace = false;
		if (!isPlus)
		{
			if (v == '\"')
				break;
			if (v == '\\')
			{
				isPlus = true;
				continue;
			}
		}
		else
		{
			isPlus = false;
		}

		rst += v;
	}
	return rst;
}

int CFollowCenter::rpc_sum( char *buf, int len, struct ns_rpc_request *req )
{
	double sum = 0;
	int i;
	if (req->params[0].type != JSON_TYPE_ARRAY) {
		return ns_rpc_create_std_error(buf, len, req,
			JSON_RPC_INVALID_PARAMS_ERROR);
	}
	for (i = 0; i < req->params[0].num_desc; i++) {
		if (req->params[i + 1].type != JSON_TYPE_NUMBER) {
			return ns_rpc_create_std_error(buf, len, req,
				JSON_RPC_INVALID_PARAMS_ERROR);
		}
		sum += strtod(req->params[i + 1].ptr, NULL);
	}
	return ns_rpc_create_reply(buf, len, req, "f", sum);
}

int CFollowCenter::rpcLogin( char *buf, int len, struct ns_rpc_request *req )
{
	if (req->params[0].type != JSON_TYPE_ARRAY)
	{
		return ns_rpc_create_std_error(buf, len, req,
			JSON_RPC_INVALID_PARAMS_ERROR);
	}
	if (req->params[0].num_desc < 2)
	{
		return ns_rpc_create_std_error(buf, len, req,
			JSON_RPC_INVALID_PARAMS_ERROR);
	}
	if (req->params[1].type != JSON_TYPE_STRING)
	{
		return ns_rpc_create_std_error(buf, len, req,
			JSON_RPC_INVALID_PARAMS_ERROR);
	}
	if (req->params[2].type != JSON_TYPE_STRING)
	{
		return ns_rpc_create_std_error(buf, len, req,
			JSON_RPC_INVALID_PARAMS_ERROR);
	}
	std::string userName = str2str(req->params[1].ptr), password = str2str(req->params[2].ptr);
	return ns_rpc_create_reply(buf, len, req, "s", "0");
}
