#ifndef DatabaseDefine_H
#define DatabaseDefine_H


#import "msado15.dll" no_namespace rename("EOF","adoEOF")

///����ϵͳ��Դ
#define SYSTEM_MONITOR				'1'

#define ADO_CATCH(RET, sql) \
	catch (_com_error &e) \
	{\
	std::string msg = e.Description(); \
	KN_FINACIAL_LOG_ERROR("Access database raise a exception: %s", msg.c_str());\
	KN_FINACIAL_LOG_ERROR(sql);\
	m_database.disconnected();\
	return RET;\
	}\
	catch (std::exception &k)\
	{\
	KN_FINACIAL_LOG_ERROR( "Access database raise a exception: %s",k.what());\
	KN_FINACIAL_LOG_ERROR(sql);\
	m_database.disconnected();\
	return RET;\
	}\
	catch (...)\
	{\
	KN_FINACIAL_LOG_ERROR( "Access database raise a exception.");\
	KN_FINACIAL_LOG_ERROR(sql);\
	m_database.disconnected();\
	return RET;\
	}

#define DB_CONFIRMCONNECT() \
	if (!m_database.confirmConnect()) \
	{\
	return -1;\
	}\

#define DB_QUERYSQL_BEFORE() \
	_RecordsetPtr pRs = nullptr;\
	pRs = m_database.querySql(sqltxt);\
	if (pRs == nullptr)\
	{\
	KN_FINACIAL_LOG_ERROR("[�������ñ�] ִ��SQLʧ��");\
	return -1;\
	}\

#define DB_QUERYSQL_AFTER() \
	pRs = m_database.querySql(sqltxt, pRs);\
	if (pRs == nullptr)\
	{\
	KN_FINACIAL_LOG_ERROR("[�������ñ�] ִ��SQLʧ��");\
	return -1;\
	}\

#define DB_QUERYSQL_END() \
	pRs->Close();\

// �洢sql�ű����ַ����������趨��Ϊsqltxt
#define DB_EXECSQL() \
	m_database.querySql(sqltxt);\

#endif // DatabaseDefine_H
