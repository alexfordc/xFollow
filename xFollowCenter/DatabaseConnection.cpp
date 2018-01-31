#include "DatabaseConnection.h"

#include <string>

#include "../include/KN_FinacialLog.h" 

#include "../Utility/KN_UtilityTool.h"


CDatabaseConnection::CDatabaseConnection()
	: m_connection(nullptr)
	, m_connected(false)
{

}

CDatabaseConnection::~CDatabaseConnection()
{

}

void CDatabaseConnection::setConnectionString(const std::string user, const std::string password, const std::string name, const std::string address)
{
	sprintf(m_connStr, "Provider=SQLOLEDB.1;User ID=%s;Password=%s;Database=%s;Network Address=%s;Network Library=dbmssocn;",
		user.c_str(), password.c_str(), name.c_str(), address.c_str());
}

bool CDatabaseConnection::confirmConnect()
{
	if (m_connected) return m_connected;

	HRESULT  hr;
	try	
	{
		closeConnection();
		hr = m_connection.CreateInstance(__uuidof(Connection)); 
		if (hr < 0)
		{
			KN_FINACIAL_LOG_ERROR("[数据库] 创建数据库实例失败 %d.", hr);
			return false;
		}

		m_connection->ConnectionTimeout = 600;
		m_connection->CommandTimeout = 600;
		m_connection->Open(m_connStr, "", "", adConnectUnspecified); 
		m_connected = true;
		return m_connected;
	}
	catch (_com_error &e) 
	{
		std::string msg = e.Description(); 
		KN_FINACIAL_LOG_ERROR("[数据库] 创建数据库实例发生错误 %s.", msg.c_str());
	}
	catch (std::exception &k)
	{
		KN_FINACIAL_LOG_ERROR("[数据库] 创建数据库实例发生异常 %s.", k.what());
	}
	catch (...)
	{
		KN_FINACIAL_LOG_ERROR("[数据库] 创建数据库实例发生未知异常.");
	}		
	return false;
}

bool CDatabaseConnection::closeConnection()
{
	try
	{
		if (m_connected && m_connection != nullptr) 
		{
			m_connection->Close();
			m_connection.Release();
		}
		m_connection = nullptr; 
		m_connected = false;
	}
	catch (...) {
	}
	return true;
}

void CDatabaseConnection::disconnected()
{
	m_connected = false;
}

_RecordsetPtr CDatabaseConnection::querySql( const std::string& qrySql, _RecordsetPtr pRs )
{
	if (pRs == nullptr)
	{
		_RecordsetPtr recordset = nullptr;

		HRESULT hr = recordset.CreateInstance(__uuidof(Recordset));
		if (hr < 0)
		{
			KN_FINACIAL_LOG_ERROR("[加载配置表] 创建ADO对象失败");
		}
		else
		{
			recordset->Open(qrySql.c_str(), (IDispatch*)m_connection, adOpenForwardOnly, adLockReadOnly, adCmdText);
		}
		return recordset;
	}
	else
	{
		pRs->Close();
		pRs->Open(qrySql.c_str(), (IDispatch*)m_connection, adOpenForwardOnly, adLockReadOnly, adCmdText);
		return pRs;
	}
}

void CDatabaseConnection::execSql( const std::string& qrySql )
{
	m_connection->Execute(qrySql.c_str(), NULL, adCmdText);
}
