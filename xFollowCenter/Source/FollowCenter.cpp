#include "FollowCenter.h"

#include <fstream>
#include <cassert>

#include "../../XNYSTools/Interface/IConfigure.h"
#include "../../Include/X_MyLog.h"

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


bool CFollowCenter::startSystem()
{
	int fsCount = m_userStatusControl.getFollowSuccessed();
	int tsCount = m_userStatusControl.getTargetSuccessed();
	int ffCount = m_userStatusControl.getFollowFailed();
	int tfCount = m_userStatusControl.getTargetFailed();
	FOLLOW_LOG_DEBUG("[�û�׼�����] �����˺ųɹ�/ʧ��(%d/%d) Ŀ���˺ųɹ�/ʧ��(%d/%d)", fsCount, ffCount, tsCount, tfCount);
	return fsCount * tsCount > 0;
}

void CFollowCenter::init()
{
	bool rtn = false;
	do {
		// ��黷��
		rtn = checkEnvironment();
		if (!rtn) break;

		// ��������
		rtn = loadDatabase();
		if (!rtn) break;
	} while (0);

	m_followHandle.initRsp(rtn, 0);
}

void CFollowCenter::start()
{
	int id = m_userRepository.addFollowUser(1, "127.0.0.1", 6666, "y001", "8");
	m_userStatusControl.addUserInfo(true, id);

	m_followHandle.registerApi("xCTPPlugin.dll", 1);
	m_followHandle.registerSpi(&m_followHandle);
	m_followHandle.reqUserLogin(1, 1, "127.0.0.1", 6666, "y001", "8");

/*
	for (auto& pp : m_apiToNames) {
		m_followHandle.registerApi(pp.second.c_str(), pp.first);
	}
	m_followHandle.registerSpi(&m_followHandle);

	auto users = m_userRepository.getAllUsers();
	for (auto& pp : users) {
		IUser* user = pp.second;
		assert(user);
		m_followHandle.reqUserLogin(user->id(), user->apiID(), user->ip(), user->port(), user->accountID(), user->password());
	}
*/
}

void CFollowCenter::stop()
{
}

//////////////////////////////////////////////////////////////////////////
void CFollowCenter::rspUserLogin(int id, bool successed, int errorID)
{
	if (!successed)
	{
		IUser* user = m_userRepository.getUserByID(id);
		if (user == nullptr) {
			return;
		}

		m_userRepository.unRegisterUser(user);

		m_userStatusControl.addUserStatus(user->isFollow(), id, successed);
		if (m_userStatusControl.hasAllUserStatus())
		{
			m_followHandle.startRsp(startSystem(), 0); // Ӧ����ϵͳ����
		}
	}
}

void CFollowCenter::rspUserInitialized(int id, bool successed, int errorID)
{
	IUser* user = m_userRepository.getUserByID(id);
	if (user == nullptr) {
		return;
	}

	m_userRepository.registerUser(successed, user);

	m_userStatusControl.addUserStatus(user->isFollow(), id, successed);
	if (m_userStatusControl.hasAllUserStatus())
	{
		m_followHandle.startRsp(startSystem(), 0); // Ӧ����ϵͳ����
	}
}

void CFollowCenter::rtnTrade( int id, const char* instrumentID, char direction, char offerset, char hedgeFlag, int volume )
{ // targetUser ==> targetGroup ==> cal ==> followGroup
	IUser* user = m_userRepository.userByID(id);
	if (user == nullptr) {
		return;
	}

	user->rtnTrade(instrumentID, direction, offerset, hedgeFlag, volume);
}
