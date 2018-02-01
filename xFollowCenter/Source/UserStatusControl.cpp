#include "UserStatusControl.h"


CUserStatusControl::CUserStatusControl()
{

}

CUserStatusControl::~CUserStatusControl()
{

}

void CUserStatusControl::addUserInfo(bool isFollowUser, int id)
{
	if (isFollowUser) {
		m_followStatus[id] = STATUS_DEFAULT;
	} else {
		m_targetStatus[id] = STATUS_DEFAULT;
	}
}

void CUserStatusControl::addUserStatus(bool isFollowUser, int id, bool successed)
{
	if (isFollowUser) {
		auto it = m_followStatus.find(id);
		if (it != m_followStatus.end()) {
			it->second = successed ? STATUS_TRUE : STATUS_FALSE;
			m_follow.insert(id);
		}
	}
	else {
		auto it = m_targetStatus.find(id);
		if (it != m_targetStatus.end()) {
			it->second = successed ? STATUS_TRUE : STATUS_FALSE;
			m_target.insert(id);
		}
	}
}

bool CUserStatusControl::hasAllUserStatus()
{
	return m_followStatus.size() == m_follow.size() && m_targetStatus.size() == m_target.size();
}

int CUserStatusControl::getFollowSuccessed()
{
	int count = 0;
	for (auto& pp : m_followStatus) {
		if (pp.second == STATUS_TRUE)
			++count;
	}
	return count;
}

int CUserStatusControl::getTargetSuccessed()
{
	int count = 0;
	for (auto& pp : m_targetStatus) {
		if (pp.second == STATUS_TRUE)
			++count;
	}
	return count;
}

int CUserStatusControl::getFollowFailed()
{
	int count = 0;
	for (auto& pp : m_followStatus) {
		if (pp.second == STATUS_FALSE)
			++count;
	}
	return count;
}

int CUserStatusControl::getTargetFailed()
{
	int count = 0;
	for (auto& pp : m_targetStatus) {
		if (pp.second == STATUS_FALSE)
			++count;
	}
	return count;
}

void CUserStatusControl::clear()
{
	m_followStatus.clear();
	m_targetStatus.clear();
	m_follow.clear();
	m_target.clear();
}
