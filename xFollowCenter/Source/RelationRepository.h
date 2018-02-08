#ifndef RelationRepository_H
#define RelationRepository_H

#include <map>

class IRelation;

class CRelationRepository
{
public:
	static CRelationRepository& relationRepository();

	IRelation* createRelation(int id, char status);
	IRelation* getRelation(int id);

private:
	static CRelationRepository*      s_instance;

	std::map<int, IRelation*>        m_relations;
};

#endif // RelationRepository_H
