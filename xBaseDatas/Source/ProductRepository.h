#ifndef ProductRepository_H
#define ProductRepository_H

#include <map>
#include <string>

#include "../Interface/IBaseDataRepository.h"

class CProductRepository : public IProductRepository
{
public:
	static CProductRepository& productRepository();
	virtual ~CProductRepository();

	virtual void setProduct(char marketType, const char* productID, const char* productName, const char* exchangeID);
	virtual IProduct* getProduct(char marketType, const char* productID);

	virtual void clear();

private:
	CProductRepository();

	static CProductRepository* s_instance;

	// marketType+'|'+productID <==> CProduct
	std::map<std::string, IProduct*>      m_products;
};

#endif // ProductRepository_H
