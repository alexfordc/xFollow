#include "FollowCenter.h"

#include "../../XNYSTools/Interface/IConfigure.h"
#include <fstream>
#include <cassert>

CFollowCenter::CFollowCenter()
	: m_followHandle(*CFollowHandle::followHandle())
{

}

CFollowCenter::~CFollowCenter()
{

}

bool CFollowCenter::checkEnvironment()
{
	XNYSTools::IConfigure* pconfig = XNYSTools::IConfigure::createConfigure();
	if (pconfig == nullptr) return false;

	bool rtn = pconfig->openFile("./config/Plugin.conf");
	if (!rtn) {
		return false;
	} else {
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

		return true;
	}
}

bool CFollowCenter::loadDatabase()
{
	return true;
}

void CFollowCenter::init()
{
	bool rtn = false;
	do 
	{
		rtn = checkEnvironment();
		if (!rtn) break;

		rtn = loadDatabase();
		if (!rtn) break;
	} while (0);

	m_followHandle.initRsp(rtn);
}

void CFollowCenter::start()
{
/*
	for (auto& pp : m_apiToNames) {
		m_traderManage->registerApi(pp.second.c_str(), pp.first);
	}
	m_traderManage->registerSpi(this);

	auto users = m_userRepository.getAllUsers();
	for (auto& pp : users) {
		IUser* user = pp.second;
		assert(user);
		m_traderManage->reqUserLogin(user->apiID(), user->ip(), user->port(), user->accountID(), user->password());
	}
*/
}

void CFollowCenter::stop()
{
}

void CFollowCenter::rspUserLogin()
{

}

void CFollowCenter::rspUserInitialized()
{

}
