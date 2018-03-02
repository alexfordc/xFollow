#ifndef QueryTask_H
#define QueryTask_H

#include <list>
#include <mutex>
#include <thread>

#include "Include/ThostFtdcTraderApi.h"

class CQueryTask
{
public:
	static CQueryTask& queryTask();
	~CQueryTask();

public:
	void login(CThostFtdcTraderApi* api, CThostFtdcReqUserLoginField& field);
	void qryPositionDetail(CThostFtdcTraderApi* api, CThostFtdcQryInvestorPositionDetailField& field);
	void qryOrder(CThostFtdcTraderApi* api, CThostFtdcQryOrderField& field);
	void qryTrade(CThostFtdcTraderApi* api, CThostFtdcQryTradeField& field);

private:
	CQueryTask();
	void start();

	static std::mutex  s_mutex;
	static CQueryTask* s_instance;

	enum emEventType
	{
		ET_LOGIN = 1,
		ET_QRYPOSITIONDETAIL,
		ET_QRYORDER,
		ET_QRYTRADE,
	};
	struct stuEvent
	{
		CThostFtdcTraderApi* api;
		int type;
		void* data;
	};
	std::list<stuEvent> m_events;
	int  run();
	bool               m_isStarted;
	std::mutex         m_mutex;
	std::thread*       m_thread;
};

#endif // QueryTask_H
