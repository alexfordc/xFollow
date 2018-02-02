#ifndef TargetGroup_H
#define TargetGroup_H

#include <map>

class CTargetUser;

class CTargetGroup
{
public:
	CTargetGroup();
	~CTargetGroup();

public:
	void addUser(CTargetUser* user);
	void calculate(CTargetUser* user); // �����֮�������Ҫ����Handle�����¼�

private:
	std::map<int, CTargetUser*>        m_id2Users;
};

#endif // TargetGroup_H
