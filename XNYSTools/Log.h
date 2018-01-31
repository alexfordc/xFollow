#ifndef Log_H
#define Log_H

#include "ILog.h"
#include <atomic>
#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <thread>

namespace XNYSTools {


class CLog
{
public:
	CLog();
	~CLog();

public:
	bool stream(const char* path);
	bool stream(const ILogStream* stream);
	void start();
	void send(int colorType, const char* buf);
	void stop();

private:
	void detailBuf(int colorType, std::string buf);
	int  run();

	std::list<std::ios*>      m_streams;
	ILogStream*               m_logStream;
	std::atomic<bool>         m_isStart;
	std::thread*              m_thread;

	struct stuBuf 
	{
		int colorType;
		std::string buf;
	};
	std::list<stuBuf*>        m_bufs;
};

}
#endif // Log_H
