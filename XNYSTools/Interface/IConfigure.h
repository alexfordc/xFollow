#ifndef IConfigure_H
#define IConfigure_H

#include "X_DllExport.h"
#include <map>
#include <string>

#ifdef	XNYSTOOLS_EXPORTS
#define XNYSTOOLS_EXPORTS_C  DLL_EXPORT_CLASS_DECL
#define XNYSTOOLS_EXPORTS_A  DLL_EXPORT_C_DECL
#else
#define XNYSTOOLS_EXPORTS_C  DLL_IMPORT_CLASS_DECL
#define XNYSTOOLS_EXPORTS_A  DLL_IMPORT_C_DECL
#endif

namespace XNYSTools {

class XNYSTOOLS_EXPORTS_C IConfigure
{
public:
	static IConfigure* createConfigure();
	static void        destroyConfigure(IConfigure* target);

	virtual bool openFile(const char* path) = 0;
	virtual bool write(const char* key, const char* value) = 0;
	virtual const char* getValue(const char* key) = 0;
	virtual bool isEnd() = 0;
	virtual std::pair<std::string, std::string> getValue() = 0;
	virtual void moveNext() = 0;
};

}


#endif // IConfigure_H
