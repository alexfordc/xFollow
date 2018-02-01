#include "Configure.h"

#include <regex>

namespace XNYSTools {

XNYSTools::IConfigure* IConfigure::createConfigure()
{
	return new CConfigure;
}

void IConfigure::destroyConfigure( IConfigure* target )
{
	delete target;
}

//////////////////////////////////////////////////////////////////////////
CConfigure::CConfigure()
	: m_file(nullptr)
{

}

CConfigure::~CConfigure()
{

}

void CConfigure::parser(std::string& content)
{
	std::regex r("(\\S*)\\s*=\\s*(\\S*)");
	std::smatch m;
	regex_match(content, m, r);

	while(regex_search(content, m, r)) {
		m_config[m[1]] = m[2];
	}
}

bool CConfigure::openFile( const char* path )
{
	if (fopen_s(&m_file, path, "a+")) {
		// false
		return false;
	}

	char buf[1024] = {0};
	std::string readString;
	while (!feof(m_file)) {
		fread_s(buf, sizeof(buf), sizeof(buf) - 1, 1, m_file);
		readString += buf;
		memset(buf, 0, sizeof(buf));
	}

	parser(readString);
	m_itConfig = m_config.begin();

	return true;
}

bool CConfigure::write( const char* key, const char* value )
{
	m_config[key] = value;

	std::string buf = key;
	buf += "=";
	buf += value;
	return fwrite(buf.c_str(), buf.length(), 1, m_file) == 0;
}

const char* CConfigure::getValue( const char* key )
{
	auto it = m_config.find(key);
	if (it == m_config.end()) {
		return nullptr;
	} else {
		return it->second.c_str();
	}
}

bool CConfigure::isEnd()
{
	if (m_itConfig == m_config.end()) {
		m_itConfig = m_config.begin();
		return true;
	} else {
		return false;
	}
}

std::pair<std::string, std::string> CConfigure::getValue()
{
	return *m_itConfig;
}

void CConfigure::moveNext()
{
	if (m_itConfig != m_config.end())
		++m_itConfig;
}

}
