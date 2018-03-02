#ifndef Organization_H
#define Organization_H

#include <string>

class COrganization
{
public:
	COrganization(int id, std::string name, char status);
	~COrganization();

	void setData(std::string name, char status);
	std::string name();
	char status();

private:
	int         m_id;
	std::string m_name;
	char        m_status;
};

#endif // Organization_H
