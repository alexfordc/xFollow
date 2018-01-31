#ifndef DatabaseConnection_H
#define DatabaseConnection_H

#include <string>
#include "DatabaseDefine.h"

class CDatabaseConnection
{
public:
	CDatabaseConnection();
	~CDatabaseConnection();

	void setConnectionString(const std::string user, const std::string password, const std::string name, const std::string address);
	bool confirmConnect();
	bool closeConnection();
	void disconnected();

	_RecordsetPtr querySql(const std::string& qrySql, _RecordsetPtr pRs = nullptr);
	void execSql(const std::string& qrySql);

private:
	_ConnectionPtr m_connection;
	char           m_connStr[256];

	bool           m_connected;

};

#endif // DatabaseConnection_H
