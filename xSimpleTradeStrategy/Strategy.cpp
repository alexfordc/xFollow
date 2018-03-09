#include "Strategy.h"

#include "Json/json.h"

CStrategy::CStrategy()
	: m_hasLimitProduct(true)
	, m_isSameDirection(true)
{

}

CStrategy::~CStrategy()
{

}

void CStrategy::setAuthProductID( std::string authProductID )
{
	if (authProductID.compare("#ALL#") == 0)
	{
		m_hasLimitProduct = false;
	}
	else
	{
		m_hasLimitProduct = true;
		int findb = -1;
		int finda = -1;
		while (authProductID.size() > 0)
		{
			finda = authProductID.find(';', findb + 1);
			if (finda != std::string::npos)
			{
				if (finda - findb - 1 > 0)
				{
					m_authProducts.insert(authProductID.substr(findb + 1, finda - findb - 1));
				}

				findb = finda;
			}
			else
			{
				if (authProductID.size() != findb + 1)
					m_authProducts.insert(authProductID.substr(findb + 1));
				break;
			}
		}
	}
}

void CStrategy::parser( std::string strategyData )
{
	Json::Value jsonRoot; //定义根节点
	//std::string jsonStr = jsonRoot.toStyledString(); //json字符串
	std::string jsonStr = strategyData;
	//jsonRoot.clear();

	Json::CharReaderBuilder builder;
	builder["collectComments"] = false;
	JSONCPP_STRING errs;
	Json::CharReader* reader = builder.newCharReader();
	if (!reader->parse(jsonStr.data(), jsonStr.data() + jsonStr.size(), &jsonRoot, &errs)) //从jsonStr中读取数据到jsonRoot
	{
		return;
	}
	std::string ffff = jsonRoot.toStyledString();
	if (!jsonRoot["IsSameDirection"].isNull())
	{
		m_isSameDirection = jsonRoot["IsSameDirection"].asBool();
	}
/*
	for (auto i = 0; i < jsonRoot.size(); i++) //遍历数组[]
	{
		for (auto sub = jsonRoot[i].begin(); sub != jsonRoot[i].end(); sub++) //遍历对象{}
		{
			if (sub.name().compare("IsSameDirection") == 0)
			{
				m_isSameDirection = (*sub).compare("true") == 0 ? true : false;
			}
			//cout << sub.name() << " : " << jsonRoot[i][sub.name()] << endl; //方法1
			//cout << sub.name() << " : " << (*sub) << endl; //方法2
		}
	}
*/
}

bool CStrategy::isInstrumentValid( std::string productID, std::string instrumentID )
{
	if (!m_hasLimitProduct) return true;

	for (auto& limit : m_authProducts)
	{
		if (productID.compare(limit) == 0)
			return true;
		if (instrumentID.compare(limit) == 0)
			return true;
	}
	return false;
}

bool CStrategy::isSameDirection()
{
	return m_isSameDirection;
}
