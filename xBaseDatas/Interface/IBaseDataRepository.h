#ifndef IExchangeRepository_H
#define IExchangeRepository_H

#include "X_DllExport.h"
#include <list>

#ifdef	XBASEDATAS_EXPORTS
#define XBASEDATAS_EXPORTS_C  DLL_EXPORT_CLASS_DECL
#define XBASEDATAS_EXPORTS_A  DLL_EXPORT_C_DECL
#else
#define XBASEDATAS_EXPORTS_C  DLL_IMPORT_CLASS_DECL
#define XBASEDATAS_EXPORTS_A  DLL_IMPORT_C_DECL
#endif

class IExchange
{
public:
	virtual ~IExchange() {};

	virtual char marketType() = 0;
	virtual const char* exchangeID() = 0;
	virtual const char* exchangeName() = 0;
};

class XBASEDATAS_EXPORTS_C IExchangeRepository
{
public:
	static IExchangeRepository& exchangeRepository();
	virtual ~IExchangeRepository() {};

	virtual void setExchange(char marketType, const char* exchangeID, const char* exchangeName) = 0;
	virtual IExchange* getExchange(char marketType, const char* exchangeID) = 0;

	virtual void clear() = 0;
};

class IProduct
{
public:
	virtual ~IProduct() {};

	virtual char marketType() = 0;
	virtual const char* productID() = 0;
	virtual const char* productName() = 0;
	virtual const char* exchangeID() = 0;
};

class XBASEDATAS_EXPORTS_C IProductRepository
{
public:
	static IProductRepository& productRepository();
	virtual ~IProductRepository() {};

	virtual void setProduct(char marketType, const char* productID, const char* productName, const char* exchangeID) = 0;
	virtual IProduct* getProduct(char marketType, const char* productID) = 0;

	virtual void clear() = 0;
};

class XBASEDATAS_EXPORTS_C IInstrument
{
public:
	virtual ~IInstrument() {};

	virtual void setData(double priceTick) = 0;

	virtual char marketType() = 0;
	virtual const char* instrumentID() = 0;
	virtual const char* instrumentName() = 0;
	virtual const char* productID() = 0;
	virtual const char* exchangeID() = 0;
	virtual double priceTick() = 0;

	// 行情数据
	virtual void lastPrice(double value) = 0;
	virtual double lastPrice() = 0;

	virtual double bestPrice(bool isBuy) = 0;
};

class XBASEDATAS_EXPORTS_C IInstrumentRepository
{
public:
	static IInstrumentRepository& instrumentRepository();
	virtual ~IInstrumentRepository() {};

	virtual void setInstrument(char marketType, const char* instrumentID, const char* instrumentName, const char* productID, const char* exchangeID, double priceTick) = 0;
	virtual IInstrument* getInstrument(char marketType, const char* instrumentID) = 0;
	virtual std::list<IInstrument*> getInstruments(char marketType) = 0;

	virtual void clear() = 0;
};


#endif // IExchangeRepository_H
