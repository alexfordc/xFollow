#ifndef IUser_H
#define IUser_H

class IUser
{
public:
	virtual int apiID() = 0;
	virtual const char* ip() = 0;
	virtual int port() = 0;
	virtual const char* accountID() = 0;
	virtual const char* password() = 0;
};

#endif // IUser_H
