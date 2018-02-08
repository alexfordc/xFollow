#ifndef IUser_H
#define IUser_H

class ITargetStrategyGroup;

class IUser
{
public:
	IUser(int id, int system_ID, std::string& accountID, std::string& password)
		: m_id(id), m_system_ID(system_ID), m_accountID(accountID), m_password(password) {}
	virtual ~IUser() {}

	virtual bool isFollow() = 0;
	virtual bool isTarget() = 0;
	virtual int  id() { return m_id; }
	virtual int  system_ID() { return m_system_ID; }
	virtual const char* accountID() { return m_accountID.c_str(); }
	virtual const char* password() { return m_password.c_str(); }

public:
	virtual void rtnTrade(const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume) = 0;
	virtual void rtnPositionTotal(const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume) = 0;

private:
	int             m_id;
	int             m_system_ID;
	std::string     m_accountID;
	std::string     m_password;
};

#endif // IUser_H
