#include "RelationRepository.h"

#include "../../Include/X_IRelation.h"

CRelationRepository* CRelationRepository::s_instance = nullptr;

CRelationRepository& CRelationRepository::relationRepository()
{
	if (nullptr == s_instance)
	{
		s_instance = new CRelationRepository;
	}
	return *s_instance;
}

IRelation* CRelationRepository::createRelation(int id, char status)
{
	IRelation* relation = nullptr;
	auto it = m_relations.find(id);
	if (it == m_relations.end())
	{
		//relation = IRelation::createRelation(id); @not 得动态加载的
	}
	else
	{
		relation = it->second;
	}
	relation->setStatus(status);
	return relation;
}

IRelation* CRelationRepository::getRelation(int id)
{
	IRelation* relation = nullptr;
	auto it = m_relations.find(id);
	if (it != m_relations.end())
	{
		relation = it->second;
	}
	return relation;
}
