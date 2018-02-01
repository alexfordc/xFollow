#ifndef Configure_H
#define Configure_H

#include <map>
#include <string>
#include <stdio.h>

#include "../Interface/IConfigure.h"

namespace XNYSTools {

class CConfigure : public IConfigure
{
public:
	CConfigure();
	~CConfigure();

public:
	bool openFile(const char* path);
	bool write(const char* key, const char* value);
	const char* getValue(const char* key);
	bool isEnd();
	std::pair<std::string, std::string> getValue();
	void moveNext();

private:
	void parser(std::string& content);
	FILE*                                    m_file;
	std::map<std::string, std::string>       m_config;
	std::map<std::string, std::string>::iterator m_itConfig;
};

}

#endif // Configure_H
