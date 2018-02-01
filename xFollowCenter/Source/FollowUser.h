#ifndef FollowUser_H
#define FollowUser_H

#include "IUser.h"

class CFollowUser : public IUser
{
public:
	virtual int apiID();
	virtual const char* ip();
	virtual int port();
	virtual const char* accountID();
	virtual const char* password();

};

#endif // FollowUser_H