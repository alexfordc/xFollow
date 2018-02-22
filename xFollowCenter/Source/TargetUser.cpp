#include "TargetUser.h"

#include "../../Include/X_IRelation.h"
#include "RelationRepository.h"

CTargetUser::CTargetUser(int id, int system_ID, std::string& accountID, std::string& password )
	: IUser(id, system_ID, accountID, password)
{
}

CTargetUser::~CTargetUser()
{

}

bool CTargetUser::isFollow()
{
	return false;
}

bool CTargetUser::isTarget()
{
	return true;
}

void CTargetUser::rtnTrade( const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume )
{
	for (auto relationID : m_relationID)
	{
		IRelation* relation = CRelationRepository::relationRepository().getRelation(relationID);
		relation->rtnTrade(m_id, productID, instrumentID, isBuy, isOpen, hedgeFlag, volume);
	}
}

void CTargetUser::rtnPositionTotal(const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume)
{
	for (auto relationID : m_relationID)
	{
		IRelation* relation = CRelationRepository::relationRepository().getRelation(relationID);
		relation->rtnTargetPositionTotal(m_id, productID, instrumentID, isBuy, hedgeFlag, volume);
	}
}
