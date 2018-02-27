#ifndef IStrategy_H
#define IStrategy_H

#include <string>

class IStrategy
{
public:
	virtual ~IStrategy() {}

	virtual std::string getStrategy() = 0;
	virtual std::string strategyType() = 0;
};

#endif // IStrategy_H
