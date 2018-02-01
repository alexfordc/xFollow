#ifndef UserStatusControl_H
#define UserStatusControl_H

#include <map>
#include <set>

class CUserStatusControl
{
public:
	CUserStatusControl();
	~CUserStatusControl();

	void addUserInfo(bool isFollowUser, int id);
	void addUserStatus(bool isFollowUser, int id, bool successed);
	bool hasAllUserStatus();
	int  getFollowSuccessed();
	int  getTargetSuccessed();
	int  getFollowFailed();
	int  getTargetFailed();

	void clear();
private:
	enum emStatus
	{
		STATUS_DEFAULT = 0,
		STATUS_TRUE,
		STATUS_FALSE,
	};
	std::map<int, emStatus>    m_followStatus;
	std::map<int, emStatus>    m_targetStatus;

	std::set<int>              m_follow;
	std::set<int>              m_target;
};

#endif // UserStatusControl_H
