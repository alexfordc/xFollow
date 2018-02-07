#ifndef DatabaseConnection_H
#define DatabaseConnection_H

#include <string>

#include "DatabaseDefine.h"

enum emDataType
{

	DT_DEFAULT = 1,
	DT_CHAR,
	DT_INT,
	DT_FLOAT,
	DT_DOUBLE,
	DT_STRING,
};

template<class T>
void getData(_RecordsetPtr pRs, std::string key, T& t, emDataType dataType);

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
	static bool    s_initCom;
	_ConnectionPtr m_connection;
	char           m_connStr[256];

	bool           m_connected;

};

#endif // DatabaseConnection_H
