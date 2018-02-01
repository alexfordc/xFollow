#ifndef ICFollowCenter_H
#define ICFollowCenter_H

#include "X_DllExport.h"

#ifdef	XFOLLOWCENTER_EXPORTS
#define XFOLLOWCENTER_EXPORTS_C  DLL_EXPORT_CLASS_DECL
#define XFOLLOWCENTER_EXPORTS_A  DLL_EXPORT_C_DECL
#else
#define XFOLLOWCENTER_EXPORTS_C  DLL_IMPORT_CLASS_DECL
#define XFOLLOWCENTER_EXPORTS_A  DLL_IMPORT_C_DECL
#endif

XFOLLOWCENTER_EXPORTS_A void* CreateFollowCenter();

#endif // ICFollowCenter_H
