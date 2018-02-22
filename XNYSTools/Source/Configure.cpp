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
	close();
}

void CConfigure::parser(std::string& content)
{
	std::regex r("(\\S*)\\s*=\\s*(\\S*)");
	std::smatch m;
	regex_match(content, m, r);

	std::string key;
	while(regex_search(content, m, r)) {
		key = m[1];
		std::transform(key.begin(), key.end(), key.begin(), ::toupper);
		m_config[key] = m[2];
		content = m.suffix();
	}
}

bool CConfigure::openFile( const char* path )
{
	if (nullptr != m_file) {
		int rtn = fclose(m_file);
		m_file = nullptr;
		if (rtn != 0) return false;
		m_config.clear();
		m_itConfig = m_config.begin();
	}

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

bool CConfigure::close()
{
	if (nullptr != m_file) {
		int rtn = fclose(m_file);
		m_file = nullptr;
		if (rtn != 0) return false;
	}
	m_config.clear();
	m_itConfig = m_config.begin();
	return true;
}

const char* CConfigure::getValue( const char* key )
{
	std::string gkey = key;
	std::transform(gkey.begin(), gkey.end(), gkey.begin(), ::toupper);
	auto it = m_config.find(gkey);
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
