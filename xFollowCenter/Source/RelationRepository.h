#ifndef RelationRepository_H
#define RelationRepository_H

#include <map>
#include <string>
#include <windows.h>

class IRelation;

class CRelationRepository
{
public:
	static CRelationRepository& relationRepository();

	void setRelationModule(const std::string type, const std::string name);

	IRelation* createRelation(int id, const std::string& strategyType, char status);
	IRelation* getRelation(int id);

private:
	HMODULE getRelationModelByType(const std::string& strategyType);
	static CRelationRepository*      s_instance;

	std::map<int, IRelation*>        m_relations;

	std::map<std::string, std::string>   m_relationName;
	std::map<std::string, HMODULE>   m_relationModel;

};

#endif // RelationRepository_H
