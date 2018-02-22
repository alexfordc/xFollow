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

			case IEVENTID_RTNTRADE:
			{
				stuRtnTradeEvent* ed = (stuRtnTradeEvent*)eventData;
				m_followCenter->rtnTrade(ed->id, ed->productID, ed->instrumentID, ed->isBuy, ed->isOpen, ed->hedgeFlag, ed->volume);
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

//////////////////////////////////////////////////////////////////////////
void CFollowHandle::initRsp(bool successed, int errorID)
{
	stuNotifyEvent* eventData = new stuNotifyEvent;
	eventData->successed = successed;
	eventData->errorID = errorID;
	m_oevents.pushEvent(OEVENTID_INIT_RSP, (void*)eventData);
}

void CFollowHandle::startRsp(bool successed, int errorID)
{
	stuNotifyEvent* eventData = new stuNotifyEvent;
	eventData->successed = successed;
	eventData->errorID = errorID;
	m_oevents.pushEvent(OEVENTID_START_RSP, (void*)eventData);
}

void CFollowHandle::stopRsp(bool successed, int errorID)
{

}

//////////////////////////////////////////////////////////////////////////
// 外部调用
void CFollowHandle::init()
{
	if (!m_isInit) {
		// 初始化日志
		LOG_START();
		FOLLOW_LOG_DEBUG("系统启动中...");

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
// 	m_isInit = false;
// 	m_isStart = false;
}

//////////////////////////////////////////////////////////////////////////
// 报盘调用
void CFollowHandle::registerApi(const char* apiName, int apiID)
{
	stuRegisterApiEvent* eventData = new stuRegisterApiEvent;
	strncpy(eventData->apiName, apiName, sizeof(eventData->apiName));
	eventData->apiID = apiID;
	m_oevents.pushEvent(OEVENTID_REGISTERAPI_REQ, (void*)eventData);
}

void CFollowHandle::registerSpi(ITraderManageSpi* spi)
{
	m_oevents.pushEvent(OEVENTID_REGISTERSPI_REQ, (void*)spi);
}

void CFollowHandle::reqUserLogin( x_stuUserLogin& userLogin )
{
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

void CFollowHandle::reqPlaceOrder( int id, const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume )
{

}

//////
// 报盘回调
void CFollowHandle::rspUserLogin(int id, bool successed, int errorID)
{
	stuUserNotifyEvent* eventData = new stuUserNotifyEvent;
	eventData->id = id;
	eventData->successed = successed;
	eventData->errorID = errorID;
	m_ievents.pushEvent(IEVENTID_USERLOGIN_RSP, (void*)eventData);
}

void CFollowHandle::rspUserInitialized(int id, bool successed, int errorID)
{
	stuUserNotifyEvent* eventData = new stuUserNotifyEvent;
	eventData->id = id;
	eventData->successed = successed;
	eventData->errorID = errorID;
	m_ievents.pushEvent(IEVENTID_USERINITIALIZED_RSP, (void*)eventData);
}

void CFollowHandle::rspPlaceOrder()
{

}

void CFollowHandle::rspCancelOrder()
{

}

void CFollowHandle::rtnPositionTotal()
{

}

void CFollowHandle::rtnTrade( int id, const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume )
{
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

}
