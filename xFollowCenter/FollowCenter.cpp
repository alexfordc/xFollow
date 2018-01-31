#include "FollowCenter.h"

#include "../XNYSTools/Configure.h"
#include <fstream>

CFollowCenter::CFollowCenter()
	: m_traderManage(nullptr)
{

}

CFollowCenter::~CFollowCenter()
{

}

bool CFollowCenter::checkEnvironment()
{
	XNYSTools::CConfigure config;
	bool rtn = config.openFile("./config/Plugin.conf");
	if (!rtn) {
		return false;
	} else {
		while (!config.isEnd()) {
			auto pp = config.getValue();

			std::fstream file;
			file.open(pp.second, std::ios::in);
			if (file.good()) {
				m_apiNames[pp.first] = pp.second;
				file.close();
			}

			config.moveNext();
		}

		return true;
	}
}

bool CFollowCenter::loadDatabase()
{
	return true;
}

bool CFollowCenter::init()
{
	bool rtn = false;

	m_traderManage = ITraderManage::createTraderManage();
	if (m_traderManage == nullptr)
		return false;

	rtn = checkEnvironment();
	if (!rtn)
		return false;

	rtn = loadDatabase();
	if (!rtn)
		return false;

	return true;
}

bool CFollowCenter::start()
{
	for (auto& pp : m_apiToNames) {
		m_traderManage->registerApi(pp.second.c_str(), pp.first);
	}
	m_traderManage->registerSpi(this);

	return true;
}

bool CFollowCenter::stop()
{
	return true;
}
