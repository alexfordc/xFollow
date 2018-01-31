#include "Log.h"


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
		: m_logStream(nullptr)
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
			m_streams.push_back(&(std::cout));
		std::fstream* fs = new std::fstream(path);
		if (fs->good())
			m_streams.push_back(fs);
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
		if (!m_isStart)
		{
			if (m_thread != nullptr)
			{
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
		if (m_thread != nullptr)
		{
			m_thread->join();
			delete m_thread;
			m_thread = nullptr;
		}
	}

	void CLog::detailBuf( int colorType, std::string buf )
	{

	}

	int CLog::run()
	{
		while (m_isStart)
		{
			stuBuf* data = nullptr;
			{
				if (m_bufs.size() > 0)
				{
					data = *(m_bufs.begin());
					m_bufs.pop_front();
				}
			}

			if (data != nullptr)
				detailBuf(data->colorType, data->buf);
			Sleep(1);
		}
	}

}