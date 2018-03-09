#include "FollowHandle.h"

#include "../../Include/X_MyLog.h"
#include "FollowCenter.h"
#include "EventDefine.h"
#include "EventStruct.h"

std::mutex CFollowHandle::s_mutex;
CFollowHandle* CFollowHandle::s_followHandle = nullptr;

//////////////////////////////////////////////////////////////////////////
CFollowHandle* CFollowHandle::followHandle()
{
	if (s_followHandle == nullptr) {
		std::lock_guard<std::mutex> lck(s_mutex);
		if (s_followHandle == nullptr) {
			s_followHandle = new CFollowHandle;
		}
	}

	return s_followHandle;
}

CFollowHandle::CFollowHandle()
	: m_iisStart(false)
	, m_ithread(nullptr)
	, m_oisStart(false)
	, m_othread(nullptr)

	, m_traderManage(nullptr)
	, m_followCenter(nullptr)
	, m_followCenterSpi(nullptr)

	, m_isInit(false)
	, m_isStart(false)

	, m_uniqueID(0)
{

}

CFollowHandle::~CFollowHandle()
{

}

void CFollowHandle::startHandle()
{
	if (m_followCenter == nullptr)
		m_followCenter = new CFollowCenter;
	if (m_traderManage == nullptr)
		m_traderManage = ITraderManage::createTraderManage();

	if (!m_iisStart) {
		m_iisStart = true;
		m_ithread = new std::thread(std::bind(&CFollowHandle::irun, this));
	}
	if (!m_oisStart) {
		m_oisStart = true;
		m_othread = new std::thread(std::bind(&CFollowHandle::orun, this));
	}
}

int CFollowHandle::irun()
{
	while (m_iisStart) {
		int eventID = 0;
		void* eventData = nullptr;
		std::tie(eventID, eventData) = m_ievents.getEvent();

		if (eventID != 0) {
			switch (eventID)
			{
			case IEVENTID_INIT:
			{
				m_followCenter->init();
			}
			break;
			case IEVENTID_START:
			{
				m_followCenter->start();
			}
			break;
			case IEVENTID_STOP:
			{
				m_followCenter->stop();
			}
			break;

			case IEVENTID_USERLOGIN_RSP:
			{
				stuUserNotifyEvent* ed = (stuUserNotifyEvent*)eventData;
				m_followCenter->rspUserLogin(ed->id, ed->successed, ed->errorID);
				delete ed;
			}
			break;
			case IEVENTID_USERINITIALIZED_RSP:
			{
				stuUserNotifyEvent* ed = (stuUserNotifyEvent*)eventData;
				m_followCenter->rspUserInitialized(ed->id, ed->successed, ed->errorID);
				delete ed;
			}
			break;
			case IEVENTID_MUSERLOGIN_RSP:
			{
				stuMUserNotifyEvent* ed = (stuMUserNotifyEvent*)eventData;
				m_followCenter->rspMUserLogin(ed->marketType, ed->successed, ed->errorID);
				delete ed;
			}
			break;

			case IEVENTID_RTNORDER:
			{
				stuRtnOrderEvent* ed = (stuRtnOrderEvent*)eventData;
				auto it = m_placeOrders.find(ed->orderIndex);
				if (it != m_placeOrders.end())
				{
					m_followCenter->rtnOrder(it->second.first, it->second.second, ed->orderStatus, ed->volume);
				}
				delete ed;
			}
			break;
			case IEVENTID_RTNTRADE:
			{
				stuRtnTradeEvent* ed = (stuRtnTradeEvent*)eventData;
				m_followCenter->rtnTrade(ed->id, ed->productID, ed->instrumentID, ed->isBuy, ed->isOpen, ed->hedgeFlag, ed->volume);
				delete ed;
			}
			break;
			case IEVENTID_RTNPOSITION:
			{
				stuRtnPositionEvent* ed = (stuRtnPositionEvent*)eventData;
				m_followCenter->rtnPositionTotal(ed->id, ed->productID, ed->instrumentID, ed->isBuy, ed->hedgeFlag, ed->volume);
				delete ed;
			}
			break;
			case IEVENTID_RTNMARKETDATA:
			{
				stuRtnMarketDataEvent* ed = (stuRtnMarketDataEvent*)eventData;
				m_followCenter->rtnMarketData(ed->marketType, ed->instrumentID, ed->lastPrice);
				delete ed;
			}
			break;

			default:
				break;
			}
		}
		Sleep(1);
	}
	return 0;
}

int CFollowHandle::orun()
{
	while (m_oisStart) {
		int eventID = 0;
		void* eventData = nullptr;
		std::tie(eventID, eventData) = m_oevents.getEvent();

		if (eventID != 0) {
			switch (eventID)
			{
			case OEVENTID_INIT_RSP:
			{
				stuNotifyEvent* ed = (stuNotifyEvent*)eventData;
				m_followCenterSpi->initRsp(ed->successed, ed->errorID);
				delete ed;
			}
			break;
			case OEVENTID_START_RSP:
			{
				stuNotifyEvent* ed = (stuNotifyEvent*)eventData;
				m_followCenterSpi->startRsp(ed->successed, ed->errorID);
				delete ed;
			}
			break;
			case OEVENTID_STOP_RSP:
			{
				// 说明center已停止，停止事件入线程
				m_iisStart = false;
				if (nullptr != m_ithread)
				{
					m_ithread->join();
					delete m_ithread;
					m_ithread = nullptr;
				}
				m_uniqueID = 0;

				FOLLOW_LOG_DEBUG("[系统状态] 系统已停止.");
				m_followCenterSpi->stopRsp();
			}
			break;

			case OEVENTID_REGISTERAPI_REQ:
			{
				stuRegisterApiEvent* ed = (stuRegisterApiEvent*)eventData;
				m_traderManage->registerApi(ed->apiName, ed->apiID);
				delete ed;
			}
			break;
			case OEVENTID_REGISTERSPI_REQ:
			{
				m_traderManage->registerSpi((ITraderManageSpi*)eventData);
			}
			break;
			case OEVENTID_USERLOGIN_REQ:
			{
				stuUserLoginEvent* ed = (stuUserLoginEvent*)eventData;
				m_traderManage->reqUserLogin(ed->userLogin);
				delete ed;
			}
			break;
			case OEVENTID_MUSERLOGIN_REQ:
			{
				stuMUserLoginEvent* ed = (stuMUserLoginEvent*)eventData;
				m_traderManage->reqUserLogin(ed->muserLogin);
				delete ed;
			}
			break;
			case OEVENTID_PLACEORDER_REQ:
			{
				stuPlaceOrderEvent* ed = (stuPlaceOrderEvent*)eventData;
				m_placeOrders[++m_uniqueID] = std::make_pair(ed->orderIndex, ed->relationID);
				m_traderManage->reqPlaceOrder(ed->id, m_uniqueID, ed->productID, ed->instrumentID, ed->isBuy, ed->isOpen, ed->hedgeFlag, ed->volume, ed->price);
				delete ed;
			}
			break;
			default:
				break;
			}
		}
		Sleep(1);
	}
	return 0;
}

void CFollowHandle::registerSpi( IFollowCenterSpi* spi )
{
	m_followCenterSpi = spi;
}

void CFollowHandle::registerLogStream(uintptr_t logStream)
{
	XNYSTools::ILog::logStream(logStream);
}

void CFollowHandle::initFaild()
{
	m_isInit = false;
}

void CFollowHandle::startFaild()
{
	stop();
}

//////////////////////////////////////////////////////////////////////////
void CFollowHandle::initRsp(bool successed, int errorID)
{
	if (!m_isInit) return;

	stuNotifyEvent* eventData = new stuNotifyEvent;
	eventData->successed = successed;
	eventData->errorID = errorID;
	m_oevents.pushEvent(OEVENTID_INIT_RSP, (void*)eventData);
}

void CFollowHandle::startRsp(bool successed, int errorID)
{
	if (!m_isInit) return;

	stuNotifyEvent* eventData = new stuNotifyEvent;
	eventData->successed = successed;
	eventData->errorID = errorID;
	m_oevents.pushEvent(OEVENTID_START_RSP, (void*)eventData);
}

void CFollowHandle::stopRsp()
{
	m_oevents.pushEvent(OEVENTID_STOP_RSP, nullptr);
}

//////////////////////////////////////////////////////////////////////////
// 外部调用
void CFollowHandle::init()
{
	if (!m_isInit) {
		// 初始化日志
		LOG_START();
		FOLLOW_LOG_DEBUG("[系统状态] 系统启动中...");

		m_isInit = true;
		startHandle();

		// 发送初始化事件
		m_ievents.pushEvent(IEVENTID_INIT);
	}
}

void CFollowHandle::start()
{
	if (m_isInit && !m_isStart) {
		m_isStart = true;

		// 发送启动事件
		m_ievents.pushEvent(IEVENTID_START);
	}
}

void CFollowHandle::stop()
{
	m_isInit = false;
	m_isStart = false;
	m_ievents.clear();
	m_oevents.clear();

/*
	if (m_traderManage != nullptr)
	{
		ITraderManage::destroyTraderManage(m_traderManage);
		m_traderManage = nullptr;
	}
*/
	// 发送停止事件
	m_ievents.pushEvent(IEVENTID_STOP);

/*
	m_oisStart = false;
	if (nullptr != m_othread)
	{
		m_othread->join();
		delete m_othread;
		m_othread = nullptr;
	}
*/
}

//////////////////////////////////////////////////////////////////////////
// 报盘调用
void CFollowHandle::registerApi(const char* apiName, int apiID)
{
	if (!m_isInit) return;

	stuRegisterApiEvent* eventData = new stuRegisterApiEvent;
	strncpy(eventData->apiName, apiName, sizeof(eventData->apiName));
	eventData->apiID = apiID;
	m_oevents.pushEvent(OEVENTID_REGISTERAPI_REQ, (void*)eventData);
}

void CFollowHandle::registerSpi(ITraderManageSpi* spi)
{
	if (!m_isInit) return;

	m_oevents.pushEvent(OEVENTID_REGISTERSPI_REQ, (void*)spi);
}

void CFollowHandle::reqUserLogin( x_stuUserLogin& userLogin )
{
	if (!m_isInit) return;

	stuUserLoginEvent* eventData = new stuUserLoginEvent;
	eventData->userLogin = userLogin;
/*
	eventData->id = userLogin.id;
	eventData->apiID = userLogin.apiID;
	eventData->isFollow = userLogin.isFollow;
	strncpy(eventData->ip1, userLogin.ip1, sizeof(eventData->ip1));
	eventData->port1 = userLogin.port1;
	strncpy(eventData->ip2, userLogin.ip2, sizeof(eventData->ip2));
	eventData->port2 = userLogin.port2;
	strncpy(eventData->ip3, userLogin.ip3, sizeof(eventData->ip3));
	eventData->port3 = userLogin.port3;
	strncpy(eventData->accountID, userLogin.accountID, sizeof(eventData->accountID));
	strncpy(eventData->password, userLogin.password, sizeof(eventData->password));
*/
	m_oevents.pushEvent(OEVENTID_USERLOGIN_REQ, (void*)eventData);
}

void CFollowHandle::reqUserLogin( x_stuMUserLogin& userLogin )
{
	if (!m_isInit) return;

	stuMUserLoginEvent* eventData = new stuMUserLoginEvent;
	eventData->muserLogin = userLogin;
	m_oevents.pushEvent(OEVENTID_MUSERLOGIN_REQ, (void*)eventData);
}

void CFollowHandle::reqPlaceOrder( int id, int relationID, int orderIndex, const char* productID, const char* instrumentID, 
								  bool isBuy, bool isOpen, char hedgeFlag, int volume, double price )
{
	if (!m_isInit) return;

	stuPlaceOrderEvent* eventData = new stuPlaceOrderEvent;
	eventData->id = id;
	eventData->relationID = relationID;
	eventData->orderIndex = orderIndex;
	strncpy_s(eventData->productID, productID, sizeof(eventData->productID));
	strncpy_s(eventData->instrumentID, instrumentID, sizeof(eventData->instrumentID));
	eventData->isBuy = isBuy;
	eventData->isOpen = isOpen;
	eventData->hedgeFlag = hedgeFlag;
	eventData->volume = volume;
	eventData->price = price;
	m_oevents.pushEvent(OEVENTID_PLACEORDER_REQ, (void*)eventData);
}

//////
// 报盘回调
void CFollowHandle::rspUserLogin(int id, bool successed, int errorID)
{
	if (!m_isInit) return;

	stuUserNotifyEvent* eventData = new stuUserNotifyEvent;
	eventData->id = id;
	eventData->successed = successed;
	eventData->errorID = errorID;
	m_ievents.pushEvent(IEVENTID_USERLOGIN_RSP, (void*)eventData);
}

void CFollowHandle::rspUserInitialized(int id, bool successed, int errorID)
{
	if (!m_isInit) return;

	stuUserNotifyEvent* eventData = new stuUserNotifyEvent;
	eventData->id = id;
	eventData->successed = successed;
	eventData->errorID = errorID;
	m_ievents.pushEvent(IEVENTID_USERINITIALIZED_RSP, (void*)eventData);
}

void CFollowHandle::rtnOrder( int orderIndex, char orderStatus, int volume )
{
	if (!m_isInit) return;

	stuRtnOrderEvent* eventData = new stuRtnOrderEvent;
	eventData->orderIndex = orderIndex;
	eventData->orderStatus = orderStatus;
	eventData->volume = volume;
	m_ievents.pushEvent(IEVENTID_RTNORDER, (void*)eventData);
}

void CFollowHandle::rtnTrade( int id, const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume )
{
	if (!m_isInit) return;

	stuRtnTradeEvent* eventData = new stuRtnTradeEvent;
	eventData->id = id;
	strncpy(eventData->productID, productID, sizeof(eventData->productID));
	strncpy(eventData->instrumentID, instrumentID, sizeof(eventData->instrumentID));
	eventData->isBuy = isBuy;
	eventData->isOpen = isOpen;
	eventData->hedgeFlag = hedgeFlag;
	eventData->volume = volume;
	m_ievents.pushEvent(IEVENTID_RTNTRADE, (void*)eventData);
}

void CFollowHandle::rtnPositionTotal( int id, const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume )
{
	if (!m_isInit) return;

	stuRtnPositionEvent* eventData = new stuRtnPositionEvent;
	eventData->id = id;
	strncpy(eventData->productID, productID, sizeof(eventData->productID));
	strncpy(eventData->instrumentID, instrumentID, sizeof(eventData->instrumentID));
	eventData->isBuy = isBuy;
	eventData->hedgeFlag = hedgeFlag;
	eventData->volume = volume;
	m_ievents.pushEvent(IEVENTID_RTNPOSITION, (void*)eventData);
}

void CFollowHandle::rspMUserLogin( char marketType, bool successed, int errorID )
{
	if (!m_isInit) return;

	stuMUserNotifyEvent* eventData = new stuMUserNotifyEvent;
	eventData->marketType = marketType;
	eventData->successed = successed;
	eventData->errorID = errorID;
	m_ievents.pushEvent(IEVENTID_MUSERLOGIN_RSP, (void*)eventData);
}

void CFollowHandle::rtnMarketData( char marketType, const char* instrumentID, double lastPrice )
{
	if (!m_isInit) return;

	stuRtnMarketDataEvent* eventData = new stuRtnMarketDataEvent;
	eventData->marketType = marketType;
	strncpy_s(eventData->instrumentID, instrumentID, sizeof(eventData->instrumentID));
	eventData->lastPrice = lastPrice;
	m_ievents.pushEvent(IEVENTID_RTNMARKETDATA, (void*)eventData);
}
