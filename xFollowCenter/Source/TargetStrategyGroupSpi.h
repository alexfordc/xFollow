#ifndef TargetStrategyGroupSpi_H
#define TargetStrategyGroupSpi_H

class ITargetStrategyGroupSpi
{
public:
	virtual void reqPlaceOrder(int id, const char* instrumentID, char direction, char offerset, char hedgeFlag, int volume) = 0;

};

#endif // TargetStrategyGroupSpi_H
