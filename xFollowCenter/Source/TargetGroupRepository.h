#ifndef TargetGroupRepository_H
#define TargetGroupRepository_H

#include <map>

class CTargetGroup;

class CTargetGroupRepository
{
public:
	static CTargetGroupRepository& targetGroupRepository();

	CTargetGroup* createTargetGroup(int id, char status);
	CTargetGroup* getTargetGroup(int id);

	void clear();

private:
	static CTargetGroupRepository*     s_instance;

	std::map<int, CTargetGroup*>       m_targetGroups;
};

#endif // TargetGroupRepository_H
