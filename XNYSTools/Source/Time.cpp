#include "../Interface/Time.h"

#include <chrono>
#include <ctime>

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif  // _WIND32

namespace XNYSTools {

// ��ȡϵͳ�ĵ�ǰʱ�䣬��λ΢��(us)
int64_t getSysTimeMicros()
{
#ifdef _WIN32
	// ��1601��1��1��0:0:0:000��1970��1��1��0:0:0:000��ʱ��(��λ100ns)
#define EPOCHFILETIME   (116444736000000000UL)
	FILETIME ft;
	LARGE_INTEGER li;
	int64_t tt = 0;
	GetSystemTimeAsFileTime(&ft);
	li.LowPart = ft.dwLowDateTime;
	li.HighPart = ft.dwHighDateTime;
	// ��1970��1��1��0:0:0:000�����ڵ�΢����(UTCʱ��)
	tt = (li.QuadPart - EPOCHFILETIME) / 10;
	return tt;
#else
	timeval tv;
	gettimeofday(&tv, 0);
	return (int64_t)tv.tv_sec * 1000000 + (int64_t)tv.tv_usec;
#endif // _WIN32
}

void getTimeString(char* time, size_t len)
{
	if (len < 25) return;

	int64_t us = getSysTimeMicros();
	std::chrono::system_clock::duration dt(us * 10);
	std::chrono::system_clock::time_point tp(dt);
	std::time_t curTime = std::chrono::system_clock::to_time_t(tp);
	struct tm timeTem = { 0 };
	localtime_s(&timeTem, &curTime);

	char buf[20] = {0};
#if _MSC_VER > 1700 // @note �ϰ汾û��F��T�Ľ���
 	strftime(buf, sizeof(buf), "%F %T", &timeTem);
#else
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeTem);
#endif
	sprintf_s(time, len, "%s.%03d", buf, int(us / 1000 % 1000));
}

};
