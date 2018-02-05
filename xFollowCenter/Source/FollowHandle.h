#ifndef FollowHandle_H
#define FollowHandle_H

#include <atomic>
#include <list>
#include <thread>
#include <mutex>

#include "../xTraderManage/Interface/ITraderManageSpi.h"
#include "../xTraderManage/Interface/ITraderManage.h"
#include "../Interface/IFollowCenter.h"
#include "FollowEvent.h"

class CFollowCenter;

class CFollowHandle : public IFollowCenter, public ITraderManageSpi
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

private:
	std::atomic<bool>       m_isInit;
	std::atomic<bool>       m_isStart;

	virtual void registerSpi(IFollowCenterSpi* spi);
	virtual void registerLogStream(uintptr_t logStream);
	virtual void init();
	virtual void start();
	virtual void stop();

public:
	virtual void initRsp(bool successed, int errorID);
	virtual void startRsp(bool successed, int errorID);
	virtual void stopRsp(bool successed, int errorID);

public:
	void registerApi(const char* apiName, int apiID);
	void registerSpi(ITraderManageSpi* spi);
	void reqUserLogin(int id, int apiID, const char* ip, int port, const char* accountID, const char* password);

	void reqPlaceOrder(int id, const char* instrumentID, char direction, char offerset, char hedgeFlag, int volume);
private:
	virtual void rspUserLogin(int id, bool successed, int errorID);
	virtual void rspUserInitialized(int id, bool successed, int errorID);

	virtual void rspPlaceOrder();
	virtual void rspCancelOrder();
	virtual void rtnPositionTotal();
	virtual void rtnTrade(int id, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume);
	virtual void rtnPositionTotal(int id, const char* instrumentID, bool isBuy, char hedgeFlag, int volume);
};

#endif // FollowHandle_H
