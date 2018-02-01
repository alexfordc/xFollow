#ifndef FollowHandle_H
#define FollowHandle_H

#include <atomic>
#include <list>
#include <thread>
#include <mutex>

#include "../xTraderManage/Interface/ITraderSpi.h"
#include "../xTraderManage/Interface/ITraderManage.h"
#include "../Interface/IFollowCenter.h"
#include "FollowEvent.h"

class CFollowCenter;

class CFollowHandle : public IFollowCenter, public ITraderSpi
{
public:
	static CFollowHandle* followHandle();
	~CFollowHandle();

public:

private:
	CFollowHandle();
	void startHandle();
	int irun();
	int orun();

	static std::mutex       s_mutex;
	static CFollowHandle*   s_followHandle;

	std::mutex              m_imutex;
	std::atomic<bool>       m_iisStart;
	std::thread*            m_ithread;
	CFollowEvent            m_ievents;

	std::mutex              m_omutex;
	std::atomic<bool>       m_oisStart;
	std::thread*            m_othread;
	CFollowEvent            m_oevents;

	ITraderManage*          m_traderManage;
	CFollowCenter*          m_followCenter;
	IFollowCenterSpi*       m_followCenterSpi;

public:
	virtual void initRsp(bool successed);
	virtual void startRsp(bool successed);
	virtual void stopRsp(bool successed);

private:
	std::atomic<bool>       m_isInit;
	std::atomic<bool>       m_isStart;

	virtual void registerSpi(IFollowCenterSpi* spi);
	virtual void init();
	virtual void start();
	virtual void stop();
private:
	virtual void rspUserLogin();
	virtual void rspUserInitialized();
};

#endif // FollowHandle_H
