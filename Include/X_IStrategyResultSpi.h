#ifndef IStrategyResultSpi_H
#define IStrategyResultSpi_H

class IStrategyResultSpi
{
public:
	virtual void reqPlaceOrder(int id, const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume) = 0;

};

#endif // IStrategyResultSpi