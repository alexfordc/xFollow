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
	void calculate(CTargetUser* user); // 计算过之后如果需要就往Handle里扔事件

private:
	std::map<int, CTargetUser*>        m_id2Users;
};

#endif // TargetGroup_H
