#include "OrganizationRepository.h"


COrganizationRepository* COrganizationRepository::s_instance = nullptr;

COrganizationRepository* COrganizationRepository::organizationRepository()
{
	if (nullptr == s_instance)
	{
		s_instance = new COrganizationRepository;
	}
	return s_instance;
}

COrganizationRepository::~COrganizationRepository()
{

}

COrganizationRepository::COrganizationRepository()
{

}

void COrganizationRepository::setOrganization( int id, std::string name, char status )
{
	auto it = m_organizations.find(id);
	if (it == m_organizations.end())
	{
		COrganization* org = new COrganization(id, name, status);
		m_organizations[id] = org;
	}
	else
	{
		it->second->setData(name, status);
	}
}

COrganization* COrganizationRepository::getOrganization( int id )
{
	auto it = m_organizations.find(id);
	return (it == m_organizations.end()) ? nullptr : it->second;
}

void COrganizationRepository::clear()
{
	for (auto& org : m_organizations)
	{
		if (org.second != nullptr)
			delete org.second;
	}
	m_organizations.clear();
}
