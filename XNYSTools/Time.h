#ifndef Time_H
#define Time_H

#include "../Include/X_DllExport.h"

#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif  // _WIND32

// 定义64位整形
#if defined(_WIN32)
typedef __int64 int64_t;
#else
typedef long long int64t;
#endif  // _WIN32


namespace XNYSTools {

	// 获取系统的当前时间，单位微秒(us)
	EXPORTS_API int64_t getSysTimeMicros();
	// 年-月-日 时:分:秒.毫秒
	EXPORTS_API std::string getTimeString();

};

#endif // Time_H
