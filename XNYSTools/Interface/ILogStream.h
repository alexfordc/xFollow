#ifndef ILogStream_H
#define ILogStream_H


class ILogStream
{
public:
	virtual void logBuffer(int logType, const char* buf, int len) = 0;
};

#endif // ILogStream_H
