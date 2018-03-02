#ifndef Strategy_H
#define Strategy_H

#include <set>
#include <string>

class CStrategy
{
public:
	CStrategy();
	~CStrategy();

	void setAuthProductID(std::string authProductID);
	void parser(std::string strategyData);

public:
	bool isInstrumentValid(std::string productID, std::string instrumentID);
	bool isSameDirection();

private:
	bool      m_hasLimitProduct;
	std::set<std::string> m_authProducts; // Լ��: ��Ϊ����
	bool      m_isSameDirection;

};

#endif // Strategy_H
