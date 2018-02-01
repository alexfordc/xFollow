#ifndef Log_H
#define Log_H

#include "../Interface/ILog.h"
#include "../Interface/ILogStream.h"

#include <atomic>
#include <iostream>
#include <fstream>
#include <list>
#include <map>
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
	WORD toConsoleColor(int colorType);
	void toLogTypeString();

	std::ostream*             m_ostream;
	std::fstream*             m_fstream;
	ILogStream*               m_logStream;
	HANDLE                    m_consoleHandle;
	std::map<int, std::string> m_logTypeStrings;

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
