#include "FollowHandle.h"

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

		if (eventID != 0)
		{
			switch (eventID)
			{
			case EVENTID_INIT:
				m_followCenter->init();
				break;
			default:
//				delete (*)eventData;
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

		if (eventID != 0)
		{
			switch (eventID)
			{
			case EVENTID_INIT_RSP:
				{
					stuInitEvent* ed = (stuInitEvent*)eventData;
					m_followCenterSpi->initRsp(ed->successed);
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

//////////////////////////////////////////////////////////////////////////
void CFollowHandle::initRsp( bool successed )
{
	stuInitEvent* eventData = new stuInitEvent;
	eventData->successed = successed;
	m_oevents.pushEvent(EVENTID_INIT_RSP, (void*)eventData);
}

void CFollowHandle::startRsp( bool successed )
{

}

void CFollowHandle::stopRsp( bool successed )
{

}

//////////////////////////////////////////////////////////////////////////
// 外部调用
void CFollowHandle::init()
{
	if (!m_isInit)
	{
		m_isInit = true;
		startHandle();

		// 发送初始化事件
		m_ievents.pushEvent(EVENTID_INIT);
	}
}

void CFollowHandle::start()
{
	if (m_isInit && !m_isStart)
	{
		m_isStart = true;
	}
}

void CFollowHandle::stop()
{
// 	m_isInit = false;
// 	m_isStart = false;
}

//////////////////////////////////////////////////////////////////////////
// 报盘回调
void CFollowHandle::rspUserLogin()
{

}

void CFollowHandle::rspUserInitialized()
{

}
