#include "Organization.h"


COrganization::COrganization( int id, std::string name, char status )
	: m_id(id)
	, m_name(name)
	, m_status(status)
{

}

COrganization::~COrganization()
{

}

void COrganization::setData( std::string name, char status )
{
	m_name = name;
	m_status = status;
}

std::string COrganization::name()
{
	return m_name;
}

char COrganization::status()
{
	return m_status;
}
