#include "Log.h"

#include "Time.h"

namespace XNYSTools {
//////////////////////////////////////////////////////////////////////////
static CLog* s_log = new CLog;

bool ILog::stream( const char* path )
{
	return s_log->stream(path);
}

bool ILog::stream( const ILogStream* stream )
{
	return s_log->stream(stream);
}

void ILog::start()
{
	s_log->start();
}

void ILog::send( int colorType, const char* buf, ... )
{
	s_log->send(colorType, buf);
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

}

CLog::~CLog()
{

}

bool CLog::stream( const char* path )
{
	if (path == nullptr || path[0] == '\0')
	{
		m_ostream = &(std::cout);
		m_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	std::fstream* fs = new std::fstream(path);
	if (fs->good())
		m_fstream = fs;
	else
		return false;
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
		m_thread = new std::thread(std::bind(&CLog::run, this));
		m_isStart = true;
	}
}

void CLog::send( int colorType, const char* buf )
{
	stuBuf* data = new stuBuf;
	data->colorType = colorType;
	data->buf = buf;
	m_bufs.push_back(data);
}

void CLog::stop()
{
	m_logStream = nullptr;
	m_isStart = false;
	if (m_thread != nullptr) {
		m_thread->join();
		delete m_thread;
		m_thread = nullptr;
	}
}

void CLog::detailBuf( int colorType, std::string buf )
{
	std::string strTime = getTimeString();

	if (m_ostream != nullptr) {
		SetConsoleTextAttribute(m_consoleHandle, toConsoleColor(colorType));
		*m_ostream << strTime << ":" << buf << std::endl;
	}
}

int CLog::run()
{
	while (m_isStart)
	{
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
}

WORD CLog::toConsoleColor(int colorType)
{
	WORD consoleColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	switch (colorType) {
	case EM_LOG_DEBUG: // 绿色
		consoleColor = FOREGROUND_GREEN;
		break;
	case EM_LOG_TRADE: // 默认色 -- 白色
		break;
	case EM_LOG_WARNN: // 黄色
		consoleColor = FOREGROUND_RED | FOREGROUND_GREEN;
		break;
	case EM_LOG_ERROR: // 红色
		consoleColor = FOREGROUND_RED;
		break;
	default:
		break;
	}
}

}