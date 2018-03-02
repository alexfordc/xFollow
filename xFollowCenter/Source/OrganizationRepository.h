#ifndef OrganizationRepository_H
#define OrganizationRepository_H

#include <map>

#include "Organization.h"

class COrganizationRepository
{
public:
	static COrganizationRepository* organizationRepository();
	~COrganizationRepository();

	void setOrganization(int id, std::string name, char status);
	COrganization* getOrganization(int id);

	void clear();

private:
	COrganizationRepository();

	static COrganizationRepository*  s_instance;

	std::map<int, COrganization*>    m_organizations;
};

#endif // OrganizationRepository_H
