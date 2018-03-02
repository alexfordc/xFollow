#ifndef RelationRepository_H
#define RelationRepository_H

#include <list>
#include <map>
#include <string>
#include <windows.h>

class IRelation;

class CRelationRepository
{
public:
	static CRelationRepository& relationRepository();

	void setRelationModule(const std::string type, const std::string name);

	IRelation* createRelation(int id, const std::string& strategyType, int org_ID);
	IRelation* getRelation(int id);

	void clear();

private:
	HMODULE getRelationModelByType(const std::string& strategyType);
	static CRelationRepository*          s_instance;

	typedef void (*DESTROYRELATIONPLUGIN)(IRelation* relation);
	std::list<std::pair<DESTROYRELATIONPLUGIN, IRelation*>>  m_rs;
	std::map<int, IRelation*>            m_relations;
	std::map<int, std::list<IRelation*>>        m_torelations;

	std::map<std::string, std::string>   m_relationName;
	std::map<std::string, HMODULE>       m_relationModel;

};

#endif // RelationRepository_H
