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

void CRelationRepository::setRelationModule( const std::string type, const std::string name )
{
	m_relationName[type] = name;
}

IRelation* CRelationRepository::createRelation(int id, const std::string& strategyType, char status)
{
	IRelation* relation = nullptr;
	auto it = m_relations.find(id);
	if (it == m_relations.end())
	{
		HMODULE module = getRelationModelByType(strategyType);
		if (module == nullptr) return nullptr;

		typedef IRelation* (*CREATERELATIONPLUGIN)(int id);
		CREATERELATIONPLUGIN func = (CREATERELATIONPLUGIN)GetProcAddress(module, "createRelation");

		relation = func(id);
		m_relations[id] = relation;
	}
	else
	{
		relation = it->second;
	}
	if (nullptr != relation)
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

HMODULE CRelationRepository::getRelationModelByType( const std::string& strategyType )
{
	auto it = m_relationModel.find(strategyType);
	if (it == m_relationModel.end()) {
		auto itName = m_relationName.find(strategyType);
		if (itName == m_relationName.end()) {
			return nullptr;
		}
		else {
			std::string dllName = itName->second;
			HMODULE module = LoadLibrary(dllName.c_str());
			if (module == nullptr) {
				return nullptr;
			}
			m_relationModel[strategyType] = module;

			return module;
		}
	}
	else {
		return it->second;
	}
}
