#ifndef DatabaseDefine_H
#define DatabaseDefine_H


#import "msado15.dll" no_namespace rename("EOF","adoEOF")

///定义系统来源
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
	KN_FINACIAL_LOG_ERROR("[加载配置表] 执行SQL失败");\
	return -1;\
	}\

#define DB_QUERYSQL_AFTER() \
	pRs = m_database.querySql(sqltxt, pRs);\
	if (pRs == nullptr)\
	{\
	KN_FINACIAL_LOG_ERROR("[加载配置表] 执行SQL失败");\
	return -1;\
	}\

#define DB_QUERYSQL_END() \
	pRs->Close();\

// 存储sql脚本的字符串的名称需定义为sqltxt
#define DB_EXECSQL() \
	m_database.querySql(sqltxt);\

#endif // DatabaseDefine_H
