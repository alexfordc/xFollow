#ifndef Time_H
#define Time_H

#include "../Include/X_DllExport.h"

#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif  // _WIND32

// ����64λ����
#if defined(_WIN32)
typedef __int64 int64_t;
#else
typedef long long int64t;
#endif  // _WIN32


namespace XNYSTools {

	// ��ȡϵͳ�ĵ�ǰʱ�䣬��λ΢��(us)
	EXPORTS_API int64_t getSysTimeMicros();
	// ��-��-�� ʱ:��:��.����
	EXPORTS_API std::string getTimeString();

};

#endif // Time_H
