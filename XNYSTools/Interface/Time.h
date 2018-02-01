#ifndef Time_H
#define Time_H

#include "X_DllExport.h"

#ifdef	XNYSTOOLS_EXPORTS
#define XNYSTOOLS_EXPORTS_C  DLL_EXPORT_CLASS_DECL
#define XNYSTOOLS_EXPORTS_A  DLL_EXPORT_C_DECL
#else
#define XNYSTOOLS_EXPORTS_C  DLL_IMPORT_CLASS_DECL
#define XNYSTOOLS_EXPORTS_A  DLL_IMPORT_C_DECL
#endif

#include <string>

// ����64λ����
#if defined(_WIN32)
typedef __int64 int64_t;
#else
typedef long long int64t;
#endif  // _WIN32


namespace XNYSTools {

	// ��ȡϵͳ�ĵ�ǰʱ�䣬��λ΢��(us)
	XNYSTOOLS_EXPORTS_A int64_t getSysTimeMicros();
	// ��-��-�� ʱ:��:��.����(���ȱ�����ڵ���25)
	XNYSTOOLS_EXPORTS_A void getTimeString(char* time, size_t len);

};

#endif // Time_H
