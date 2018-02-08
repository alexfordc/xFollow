#ifndef IStrategy_H
#define IStrategy_H

#include <string>

class IStrategy
{
public:
	virtual ~IStrategy() {}

	virtual bool isInstrumentValid(const char* productID, const char* instrumentID) = 0;
	virtual std::string getStrategy() = 0;

};

#endif // IStrategy_H
