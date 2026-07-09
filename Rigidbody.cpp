#include "stdafx.h"
#include "Rigidbody.h"
#include "GameObject.h"

void Rigidbody::Update(const float& dTime)
{

	if (isStatic) {
		return;
	}

	if (useGravity)
	{
		velocity.y += Config::GRAVITY * gravityScale * dTime;
	}

	owner->SetPosition(owner->GetPosition() + velocity * dTime);

	if (MyStd::Abs(velocity.x) <= velocityThreshold.x) {
		velocity.x = 0.0f;
	}

	velocity *= (1.0f - linearDamping * dTime); // ‘¬“xŒ¸Š‚Ì“K—p
}

#if RB_DEBUG
void Rigidbody::DebugDraw(const Camera& camera) const
{
	Vec2f screenPos = camera.ToScreen(owner->GetPosition());
	DrawLineAA(screenPos.x, screenPos.y, screenPos.x + velocity.x, screenPos.y + velocity.y, GetColor(0, 255, 0), 2);
}
#endif // RB_DEBUG