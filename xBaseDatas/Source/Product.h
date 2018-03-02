#ifndef Product_H
#define Product_H

#include "../Interface/IBaseDataRepository.h"

class CProduct : public IProduct
{
public:
	CProduct(char marketType, const char* productID, const char* productName, const char* exchangeID);
	virtual ~CProduct();

	virtual char marketType();
	virtual const char* productID();
	virtual const char* productName();
	virtual const char* exchangeID();

private:
	char m_marketType;
	char m_productID[32];
	char m_productName[32];
	char m_exchangeID[32];
};

#endif // Product_H
