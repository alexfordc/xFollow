#ifndef TargetStrategyGroupSpi_H
#define TargetStrategyGroupSpi_H

class ITargetStrategyGroupSpi
{
public:
	virtual void reqPlaceOrder(int id, const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume) = 0;

};

#endif // TargetStrategyGroupSpi_H
