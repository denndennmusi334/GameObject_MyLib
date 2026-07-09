#include "stdafx.h"
#include "RigidbodyManager.h"
#include "TimeManager.h"

void RigidbodyManager::Update()
{
	dTime = TimeManager::GetInstance().GetDeltaTime();
	for (auto rb : rigidbodies)
	{
		if (rb && !rb->IsDestroyed())
		{
			rb->Update(dTime);
			rb->SetIsGround(false);
		}
	}
}

#if RB_DEBUG
void RigidbodyManager::DebugDraw(const Camera& camera) const
{
	for (auto rb : rigidbodies)
	{
		if (rb && !rb->IsDestroyed())
		{
			rb->DebugDraw(camera);
		}
	}
}
#endif // RB_DEBUG