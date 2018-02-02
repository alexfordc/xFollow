#ifndef DatabaseDefine_H
#define DatabaseDefine_H


#import "../msado15.dll" no_namespace rename("EOF","adoEOF")

///定义系统来源
#define SYSTEM_MONITOR				'1'

#define ADO_CATCH(RET, sql) \
	catch (_com_error &e) \
	{\
		std::string msg = e.Description(); \
		FOLLOW_LOG_ERROR("[数据库] 连接数据库异常: %s", msg.c_str());\
		FOLLOW_LOG_ERROR(sql);\
		m_database.disconnected();\
		return RET;\
	}\
	catch (std::exception &k)\
	{\
		FOLLOW_LOG_ERROR( "[数据库] 连接数据库异常: %s",k.what());\
		FOLLOW_LOG_ERROR(sql);\
		m_database.disconnected();\
		return RET;\
	}\
	catch (...)\
	{\
		FOLLOW_LOG_ERROR( "[数据库] 连接数据库异常.");\
		FOLLOW_LOG_ERROR(sql);\
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
		FOLLOW_LOG_ERROR("[数据库] 执行SQL失败");\
		return -1;\
	}\

#define DB_QUERYSQL_AFTER() \
	pRs = m_database.querySql(sqltxt, pRs);\
	if (pRs == nullptr)\
	{\
		FOLLOW_LOG_ERROR("[数据库] 执行SQL失败");\
		return -1;\
	}\

#define DB_QUERYSQL_END() \
	pRs->Close();\

// 存储sql脚本的字符串的名称需定义为sqltxt
#define DB_EXECSQL() \
	m_database.querySql(sqltxt);\

#endif // DatabaseDefine_H
