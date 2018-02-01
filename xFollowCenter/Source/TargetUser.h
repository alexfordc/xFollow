#ifndef TargetUser_H
#define TargetUser_H

#include "IUser.h"

class CTargetUser : public IUser
{
public:
	virtual int apiID();
	virtual const char* ip();
	virtual int port();
	virtual const char* accountID();
	virtual const char* password();

};

#endif // TargetUser_H
