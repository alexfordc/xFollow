#ifndef ILog_H
#define ILog_H


#include "../Include/X_DllExport.h"
#include "ILogStream.h"

namespace XNYSTools {
enum MyEnum
{
	EM_LOG_DEFAULT = 0,
	EM_LOG_DEBUG,
	EM_LOG_TRADE,
	EM_LOG_WARNN,
	EM_LOG_ERROR,
};

class EXPORTS_CLASS ILog
{
public:
	/**
	 * @fn     stream
	 * @brief  
	 * @param  [in] const char * path 
	                       1. 为空则输出到控制台
						   2. 输出到文件
				    const ILogStream* stream
					       1. 回调输出
	 * @return bool 
	**/
	static bool stream(const char* path);
	static bool stream(const ILogStream* stream);
	static void start();
	static void send(int colorType, const char* buf, ...);
	static void stop();
};

}

#define LOG_CONSOLE() { XNYSTools::ILog::stream(""); }
#define LOG_FILE(path) { XNYSTools::ILog::stream(path); }
#define LOG_OPEN() { XNYSTools::ILog::start(); }
#define LOG_CLOSE() { XNYSTools::ILog::stop(); }

#define LOG_DEBUG(format, ...) { XNYSTools::ILog::send(EM_LOG_DEBUG, format, ##__VA_ARGS__); }
#define LOG_TRACE(format, ...) { XNYSTools::ILog::send(EM_LOG_TRADE, format, ##__VA_ARGS__); }
#define LOG_WARNN(format, ...) { XNYSTools::ILog::send(EM_LOG_WARNN, format, ##__VA_ARGS__); }
#define LOG_ERROR(format, ...) { XNYSTools::ILog::send(EM_LOG_ERROR, format, ##__VA_ARGS__); }

#endif // ILog_H
