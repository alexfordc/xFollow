#include "DatabaseConnection.h"

#include "X_MyLog.h"



bool CDatabaseConnection::s_initCom = false;


//////////////////////////////////////////////////////////////////////////
void getData( _RecordsetPtr pRs, std::string key, char& t, emDataType dataType )
{
	t = '\0';
	_variant_t rst = pRs->Fields->GetItem(_variant_t(key.c_str()))->Value;
	if (rst.vt == VT_NULL) return;
	if (dataType == DT_CHAR) t = char(rst);
}

void getData( _RecordsetPtr pRs, std::string key, int& t, emDataType dataType )
{
	t = 0;
	_variant_t rst = pRs->Fields->GetItem(_variant_t(key.c_str()))->Value;
	if (rst.vt == VT_NULL) return;
	if (dataType == DT_INT) t = int(rst);
}

void getData( _RecordsetPtr pRs, std::string key, float& t, emDataType dataType )
{
	t = 0;
	_variant_t rst = pRs->Fields->GetItem(_variant_t(key.c_str()))->Value;
	if (rst.vt == VT_NULL) return;
	if (dataType == DT_FLOAT) t = float(rst);
}

void getData( _RecordsetPtr pRs, std::string key, double& t, emDataType dataType )
{
	t = 0;
	_variant_t rst = pRs->Fields->GetItem(_variant_t(key.c_str()))->Value;
	if (rst.vt == VT_NULL) return;
	if (dataType == DT_DOUBLE) t = double(rst);
}

void getData( _RecordsetPtr pRs, std::string key, std::string& t, emDataType dataType )
{
	t = "";
	_variant_t rst = pRs->Fields->GetItem(_variant_t(key.c_str()))->Value;
	if (rst.vt == VT_NULL) return;
	if (dataType == DT_STRING) t = _bstr_t(rst);
}

//////////////////////////////////////////////////////////////////////////
CDatabaseConnection::CDatabaseConnection()
	: m_connection(nullptr)
	, m_connected(false)
{
}

CDatabaseConnection::~CDatabaseConnection()
{
	::CoUninitialize();
}

void CDatabaseConnection::setConnectionString(const std::string user, const std::string password, const std::string name, const std::string address)
{
	if (!s_initCom)
	{
		s_initCom = true;
		::CoInitialize(NULL);
	}
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
			FOLLOW_LOG_ERROR("[���ݿ�] �������ݿ�ʵ��ʧ�� %d.", hr);
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
		FOLLOW_LOG_ERROR("[���ݿ�] �������ݿ�ʵ���������� %s.", msg.c_str());
	}
	catch (std::exception &k)
	{
		FOLLOW_LOG_ERROR("[���ݿ�] �������ݿ�ʵ�������쳣 %s.", k.what());
	}
	catch (...)
	{
		FOLLOW_LOG_ERROR("[���ݿ�] �������ݿ�ʵ������δ֪�쳣.");
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
			FOLLOW_LOG_ERROR("[���ݿ�] ����ADO����ʧ��");
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
