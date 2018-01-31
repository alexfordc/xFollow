#ifndef IFollowCenter_H
#define IFollowCenter_H

class IFollowCenter
{
public:
	virtual bool init() = 0;
	virtual bool start() = 0;
	virtual bool stop() = 0;
};

#endif // IFollowCenter_H
