#include "TargetGroupRepository.h"

#include "TargetGroup.h"

CTargetGroupRepository* CTargetGroupRepository::s_instance = nullptr;

CTargetGroupRepository& CTargetGroupRepository::targetGroupRepository()
{
	if (nullptr == s_instance)
	{
		s_instance = new CTargetGroupRepository;
	}
	return *s_instance;
}

CTargetGroup* CTargetGroupRepository::createTargetGroup( int id, char status )
{
	CTargetGroup* targetGroup = nullptr;
	auto it = m_targetGroups.find(id);
	if (it == m_targetGroups.end())
	{
		targetGroup = new CTargetGroup(id);
		m_targetGroups[id] = targetGroup;
	}
	else
	{
		targetGroup = it->second;
	}
	if (nullptr != targetGroup)
		targetGroup->setStatus(status);
	return targetGroup;
}

CTargetGroup* CTargetGroupRepository::getTargetGroup( int id )
{
	CTargetGroup* targetGroup = nullptr;
	auto it = m_targetGroups.find(id);
	if (it != m_targetGroups.end())
	{
		targetGroup = it->second;
	}
	return targetGroup;
}

void CTargetGroupRepository::clear()
{
	for (auto& t : m_targetGroups)
	{
		if (t.second != nullptr)
			delete t.second;
	}
	m_targetGroups.clear();
}
