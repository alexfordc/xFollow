#ifndef X_DLL_EXPORT_HEADER
#define X_DLL_EXPORT_HEADER

#pragma warning(disable: 4251 4996 4275)

#ifdef WIN32
	#include <Windows.h>
	#ifdef __cplusplus 
		#define DLL_EXPORT_C_DECL extern "C" __declspec(dllexport)
		#define DLL_IMPORT_C_DECL extern "C" __declspec(dllimport)
		#define DLL_EXPORT_DECL extern __declspec(dllexport)
		#define DLL_IMPORT_DECL extern __declspec(dllimport)
		#define DLL_EXPORT_CLASS_DECL __declspec(dllexport)
		#define DLL_IMPORT_CLASS_DECL __declspec(dllimport)
	#else
		#define DLL_EXPORT_C_DECL
		#define DLL_IMPORT_C_DECL
		#define DLL_EXPORT_DECL
		#define DLL_IMPORT_DECL
		#define DLL_EXPORT_CLASS_DECL
		#define DLL_IMPORT_CLASS_DECL
	#endif
#else
	#ifdef __cplusplus
		#define DLL_EXPORT_C_DECL extern "C"
		#define DLL_IMPORT_C_DECL extern "C"
		#define DLL_EXPORT_DECL extern
		#define DLL_IMPORT_DECL extern
		#define DLL_EXPORT_CLASS_DECL
		#define DLL_IMPORT_CLASS_DECL
	#else
		#define DLL_EXPORT_C_DECL
		#define DLL_IMPORT_C_DECL
		#define DLL_EXPORT_DECL
		#define DLL_IMPORT_DECL
		#define DLL_EXPORT_CLASS_DECL
		#define DLL_IMPORT_CLASS_DECL
	#endif
#endif

#ifdef	X_DLL_EXPORTS
	#define EXPORTS_CLASS DLL_EXPORT_CLASS_DECL
	#define EXPORTS_API   DLL_EXPORT_C_DECL
#else
	#define EXPORTS_CLASS DLL_IMPORT_CLASS_DECL
	#define EXPORTS_API   DLL_IMPORT_C_DECL
#endif

#endif //X_DLL_EXPORT_HEADER
