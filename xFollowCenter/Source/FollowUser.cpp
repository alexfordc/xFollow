#include "FollowUser.h"

#include "../../Include/X_IRelation.h"
#include "RelationRepository.h"

CFollowUser::CFollowUser( int id, int system_ID, std::string& accountID, std::string& password )
	: IUser(id, system_ID, accountID, password)
{
}

CFollowUser::~CFollowUser()
{

}

bool CFollowUser::isFollow()
{
	return true;
}

bool CFollowUser::isTarget()
{
	return false;
}

void CFollowUser::rtnTrade( const char* productID, const char* instrumentID, bool isBuy, bool isOpen, char hedgeFlag, int volume )
{
	// ¸úËæÕËºÅÃ»ÓÃ
}

void CFollowUser::rtnPositionTotal( const char* productID, const char* instrumentID, bool isBuy, char hedgeFlag, int volume )
{
	for (auto relationID : m_relationID)
	{
		IRelation* relation = CRelationRepository::relationRepository().getRelation(relationID);
		relation->rtnFollowPositionTotal(m_id, productID, instrumentID, isBuy, hedgeFlag, volume);
	}
}
