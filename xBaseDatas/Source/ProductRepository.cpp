#include "ProductRepository.h"

#include "Product.h"

IProductRepository& IProductRepository::productRepository()
{
	return CProductRepository::productRepository();
}
//////////////////////////////////////////////////////////////////////////
CProductRepository* CProductRepository::s_instance = nullptr;

CProductRepository& CProductRepository::productRepository()
{
	if (s_instance == nullptr)
	{
		s_instance = new CProductRepository;
	}

	return *s_instance;
}

CProductRepository::CProductRepository()
{

}

CProductRepository::~CProductRepository()
{

}

void CProductRepository::setProduct( char marketType, const char* productID, const char* productName, const char* exchangeID )
{
	std::string key = &marketType;
	key += "|";
	key += productID;
	auto it = m_products.find(key);
	if (it == m_products.end())
	{
		IProduct* product = new CProduct(marketType, productID, productName, exchangeID);
		m_products[key] = product;
	}
}

IProduct* CProductRepository::getProduct( char marketType, const char* productID )
{
	std::string key = &marketType;
	key += "|";
	key += productID;
	auto it = m_products.find(key);
	return it == m_products.end() ? nullptr : it->second;
}

void CProductRepository::clear()
{
	for (auto& e : m_products)
	{
		if (e.second != nullptr)
			delete e.second;
	}
	m_products.clear();
}
