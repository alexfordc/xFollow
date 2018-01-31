#ifndef FollowCenter_H
#define FollowCenter_H

#include "../xTraderManage/ITraderSpi.h"
#include "../xTraderManage/ITraderManage.h"
#include "IFollowCenter.h"

#include <map>
#include <string>

class CFollowCenter : public IFollowCenter, public ITraderSpi
{
public:
	CFollowCenter();
	~CFollowCenter();
public:

private:
	bool checkEnvironment();
	bool loadDatabase();

	std::map<std::string, std::string>             m_apiNames;             // key -- path
	std::map<int, std::string>                     m_apiToNames;           // ID -- path
	ITraderManage*                                 m_traderManage;

private:
	virtual bool init();
	virtual bool start();
	virtual bool stop();
private:
	virtual void rspUserLogin();
	virtual void rspUserInitialized();
};

#endif // FollowCenter_H
