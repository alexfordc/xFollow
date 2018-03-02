#include "QueryTask.h"

#include <chrono>

std::mutex CQueryTask::s_mutex;
CQueryTask* CQueryTask::s_instance = nullptr;

//////////////////////////////////////////////////////////////////////////
CQueryTask& CQueryTask::queryTask()
{
	if (s_instance == nullptr)
	{
		std::lock_guard<std::mutex> lck(s_mutex);
		if (s_instance == nullptr)
		{
			s_instance = new CQueryTask;
		}
	}
	return *s_instance;
}

CQueryTask::~CQueryTask()
{
	m_isStarted = false;
	if (nullptr != m_thread)
	{
		m_thread->join();
		delete m_thread;
	}
}

void CQueryTask::login( CThostFtdcTraderApi* api, CThostFtdcReqUserLoginField& field )
{
	{
		CThostFtdcReqUserLoginField* p = new CThostFtdcReqUserLoginField;
		*p = field;
		std::lock_guard<std::mutex> lck(m_mutex);
		stuEvent event = {0};
		event.api = api;
		event.type = ET_LOGIN;
		event.data = (void*)p;
		m_events.push_back(event);
	}
	start();
}

void CQueryTask::qryPositionDetail( CThostFtdcTraderApi* api, CThostFtdcQryInvestorPositionDetailField& field )
{
	{
		CThostFtdcQryInvestorPositionDetailField* p = new CThostFtdcQryInvestorPositionDetailField;
		*p = field;
		std::lock_guard<std::mutex> lck(m_mutex);
		stuEvent event = {0};
		event.api = api;
		event.type = ET_QRYPOSITIONDETAIL;
		event.data = (void*)p;
		m_events.push_back(event);
	}
	start();
}

void CQueryTask::qryOrder( CThostFtdcTraderApi* api, CThostFtdcQryOrderField& field )
{
	{
		CThostFtdcQryOrderField* p = new CThostFtdcQryOrderField;
		*p = field;
		std::lock_guard<std::mutex> lck(m_mutex);
		stuEvent event = {0};
		event.api = api;
		event.type = ET_QRYORDER;
		event.data = (void*)p;
		m_events.push_back(event);
	}
	start();
}

void CQueryTask::qryTrade( CThostFtdcTraderApi* api, CThostFtdcQryTradeField& field )
{
	{
		CThostFtdcQryTradeField* p = new CThostFtdcQryTradeField;
		*p = field;
		std::lock_guard<std::mutex> lck(m_mutex);
		stuEvent event = {0};
		event.api = api;
		event.type = ET_QRYTRADE;
		event.data = (void*)p;
		m_events.push_back(event);
	}
	start();
}

CQueryTask::CQueryTask()
	: m_isStarted(false)
	, m_thread(nullptr)
{

}

void CQueryTask::start()
{
	if (!m_isStarted)
	{
		m_isStarted = true;
		// Æô¶¯Ïß³Ì
		m_thread = new std::thread(std::bind(&CQueryTask::run, this));
	}
}

int CQueryTask::run()
{
	static std::chrono::system_clock::time_point s_now = std::chrono::system_clock::now();
	static bool s_isFirst = true;

	while (m_isStarted)
	{
		stuEvent event = {0};
		{
			std::lock_guard<std::mutex> lck(m_mutex);
			if (m_events.size() > 0)
			{
				event = *(m_events.begin());
				m_events.pop_front();
			}
		}
		auto now = std::chrono::system_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - s_now);
		if (!s_isFirst && (duration.count() < 1000))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1000 - duration.count()));
		}
		s_isFirst = false;
		s_now = now;

		if (event.api != nullptr)
		{
			if (event.type == ET_LOGIN)
			{
				CThostFtdcReqUserLoginField* p = (CThostFtdcReqUserLoginField*)event.data;
				event.api->ReqUserLogin(p, 0);
				delete p;
			}
			else if (event.type == ET_QRYPOSITIONDETAIL)
			{
				CThostFtdcQryInvestorPositionDetailField* p = (CThostFtdcQryInvestorPositionDetailField*)event.data;
				event.api->ReqQryInvestorPositionDetail(p, 0);
				delete p;
			}
			else if (event.type == ET_QRYORDER)
			{
				CThostFtdcQryOrderField* p = (CThostFtdcQryOrderField*)event.data;
				event.api->ReqQryOrder(p, 0);
				delete p;
			}
			else if (event.type == ET_QRYTRADE)
			{
				CThostFtdcQryTradeField* p = (CThostFtdcQryTradeField*)event.data;
				event.api->ReqQryTrade(p, 0);
				delete p;
			}
		}
	}

	return 0;
}
