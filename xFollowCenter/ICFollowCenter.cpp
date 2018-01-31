#include "ICFollowCenter.h"

#include "FollowCenter.h"

void* CreateFollowCenter()
{
	IFollowCenter* followCenter = nullptr;

	static bool isCreate = false;
	if (!isCreate) {
		try {
			followCenter = new CFollowCenter;
			isCreate = true;
		} catch (...) {

		}
	}
	return followCenter;
}
