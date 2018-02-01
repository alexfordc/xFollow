#ifndef IFollowCenter_H
#define IFollowCenter_H

class IFollowCenterSpi
{
public:
	virtual void initRsp(bool successed) = 0;
	virtual void startRsp(bool successed) = 0;
	virtual void stopRsp(bool successed) = 0;
};

class IFollowCenter
{
public:
	virtual void registerSpi(IFollowCenterSpi* spi) = 0;
	virtual void init() = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
};

#endif // IFollowCenter_H