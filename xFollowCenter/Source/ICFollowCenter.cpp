#include "../Interface/ICFollowCenter.h"

#include "FollowHandle.h"

void* CreateFollowCenter()
{
	return CFollowHandle::followHandle();
}
