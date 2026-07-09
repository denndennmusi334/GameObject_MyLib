#pragma once
#include "BaseCollider.h"

class BoxCollider : public BaseCollider
{
    Vector2D<float> halfSize;   //箱の縦横幅の半分
public:
    BoxCollider(const Vector2D<float> local, const Vector2D<float> hs) : halfSize(hs) {
        localPos = local;
    }
    Vector2D<float> GetHalfSize() const { return halfSize; }
    ColliderType GetColliderType() const override { return ColliderType::BOX; }

#if COLLIDER_DEBUG
    void DebugDraw() override;
#endif
};


class CircleCollider : public BaseCollider
{
    float radius;       // 円の半径
public:
    CircleCollider(const Vector2D<float> local, float r) : radius(r) {
        localPos = local;
    }
    float GetRadius() const { return radius; }
    ColliderType GetColliderType() const override { return ColliderType::CIRCLE; }

#if COLLIDER_DEBUG
    void DebugDraw() override;
#endif
};
