#include "Log.h"

#include <stdarg.h>
#include "../Interface/Time.h"

namespace XNYSTools {
//////////////////////////////////////////////////////////////////////////
static CLog* s_log = new CLog;

bool ILog::stream( const char* path )
{
	return s_log->stream(path);
}

bool ILog::logStream(const void* stream )
{
	const ILogStream* log = (ILogStream*)const_cast<void*>(stream);
	return s_log->stream(log);
}

void ILog::start()
{
	s_log->start();
}

void ILog::send( int colorType, const char* buf, ... )
{
	va_list argv = nullptr;
	char logBuf[2048] = {0};
	va_start(argv, buf);
	vsprintf(logBuf, buf, argv);
	va_end(argv);

	s_log->send(colorType, logBuf);
}

void ILog::stop()
{
	s_log->stop();
}

//////////////////////////////////////////////////////////////////////////
CLog::CLog()
	: m_ostream(nullptr)
	, m_fstream(nullptr)
	, m_logStream(nullptr)
	, m_consoleHandle(nullptr)
	, m_isStart(false)
	, m_thread(nullptr)
{
	toLogTypeString();
}

CLog::~CLog()
{

}

bool CLog::stream( const char* path )
{
	if (path == nullptr || path[0] == '\0') {
		m_ostream = &(std::cout);
		m_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	} else {
		std::fstream* fs = new std::fstream(path);
		if (fs->good())
			m_fstream = fs;
		else
			return false;
	}

	return true;
}

bool CLog::stream( const ILogStream* stream )
{
	if (stream != nullptr) {
		m_logStream = const_cast<ILogStream*>(stream);
		return true;
	} else {
		return false;
	}
}

void CLog::start()
{
	if (!m_isStart) {
		if (m_thread != nullptr) {
			m_isStart = false;
			m_thread->join();
			delete m_thread;
		}
		m_isStart = true;
		m_thread = new std::thread(std::bind(&CLog::run, this));
	}
}

void CLog::send( int colorType, const char* buf )
{
	if (m_isStart) {
		if (colorType < EM_LOG_DEFAULT || colorType >= EM_LOG_OTHER)
			colorType = EM_LOG_DEFAULT;
		stuBuf* data = new stuBuf;
		data->colorType = colorType;
		data->buf = buf;
		m_bufs.push_back(data);
	}
}

void CLog::stop()
{
	m_ostream = nullptr;
	m_fstream = nullptr;
	m_logStream = nullptr;
	m_consoleHandle = nullptr;
	m_isStart = false;
	if (m_thread != nullptr) {
		m_thread->join();
		delete m_thread;
		m_thread = nullptr;
	}
	for (auto& buf : m_bufs)
	{
		if (buf != nullptr) delete buf;
	}
	m_bufs.clear();
}

void CLog::detailBuf( int colorType, std::string buf )
{
	char timeBuf[55] = {0};
	getTimeString(timeBuf, sizeof(timeBuf));
	std::string strTimeLog = std::string("[") + timeBuf + " " + m_logTypeStrings[colorType] + "] ";

	if (m_ostream != nullptr) {
		SetConsoleTextAttribute(m_consoleHandle, toConsoleColor(colorType));
		*m_ostream << strTimeLog << buf << std::endl;
	}
	if (m_fstream != nullptr) {
		*m_fstream << strTimeLog << buf << std::endl;
	}
	if (m_logStream != nullptr) {
		*m_fstream << strTimeLog << buf << std::endl;
		std::string logBuf = strTimeLog + buf;
		m_logStream->logBuffer(logBuf.c_str(), logBuf.length() + 1);
	}
}

int CLog::run()
{
	while (m_isStart) {
		stuBuf* data = nullptr;
		{
			if (m_bufs.size() > 0) {
				data = *(m_bufs.begin());
				m_bufs.pop_front();
			}
		}

		if (data != nullptr) {
			detailBuf(data->colorType, data->buf);
			delete data;
		}
		Sleep(1);
	}

	return 0;
}

WORD CLog::toConsoleColor(int colorType)
{
	WORD consoleColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	switch (colorType) {
	case EM_LOG_DEBUG: // 绿色
		consoleColor = FOREGROUND_INTENSITY | FOREGROUND_GREEN;
		break;
	case EM_LOG_TRADE: // 默认色 -- 白色
		break;
	case EM_LOG_WARNN: // 黄色
		consoleColor = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN;
		break;
	case EM_LOG_ERROR: // 红色
		consoleColor = FOREGROUND_INTENSITY| FOREGROUND_RED;
		break;
	default:
		break;
	}
	return consoleColor;
}

void CLog::toLogTypeString()
{
	m_logTypeStrings[EM_LOG_DEFAULT] = "OTHER";
	m_logTypeStrings[EM_LOG_DEBUG] = "DEBUG";
	m_logTypeStrings[EM_LOG_TRADE] = "TRADE";
	m_logTypeStrings[EM_LOG_WARNN] = "WARNN";
	m_logTypeStrings[EM_LOG_ERROR] = "ERROR";
}

}