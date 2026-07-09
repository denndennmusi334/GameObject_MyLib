#include "stdafx.h"
#include "Collider.h"

#if COLLIDER_DEBUG
void BoxCollider::DebugDraw()
{
    DrawBoxAA(GetWorldPosition().x - halfSize.x, GetWorldPosition().y - halfSize.y, GetWorldPosition().x + halfSize.x, GetWorldPosition().y + halfSize.y, GetColor(255, 0, 0), FALSE);
}

void CircleCollider::DebugDraw()
{
    DrawCircleAA(GetWorldPosition().x, GetWorldPosition().y, radius, 32, GetColor(255, 0, 0), FALSE);
}

#endif
