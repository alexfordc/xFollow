#ifndef PluginAttribute_H
#define PluginAttribute_H

#include "X_DllExport.h"
#include "X_Define.h"

#ifdef	XAPIPLUGIN_EXPORTS
#define XAPIPLUGIN_EXPORTS_C  DLL_EXPORT_CLASS_DECL
#define XAPIPLUGIN_EXPORTS_A  DLL_EXPORT_C_DECL
#else
#define XAPIPLUGIN_EXPORTS_C  DLL_IMPORT_CLASS_DECL
#define XAPIPLUGIN_EXPORTS_A  DLL_IMPORT_C_DECL
#endif

XAPIPLUGIN_EXPORTS_A const char* pluginName();
XAPIPLUGIN_EXPORTS_A char marketType();
XAPIPLUGIN_EXPORTS_A char type();


#endif // PluginAttribute_H