#include "stdafx.h"
#include "ColliderManager.h"

using namespace MyStd;

void ColliderManager::CheckAllCollisions()
{
    currentPairs.clear();
    std::vector<BaseCollider*> snap = colliders;
    const size_t count = snap.size();
    for (size_t i = 0; i < count; ++i)
    {
        for (size_t j = i + 1; j < count; ++j)
        {
            BaseCollider* a = snap[i];
            BaseCollider* b = snap[j];

            if (!a || !b) continue;

            CollisionInfo info =
                CheckPair(a, b);

            CollisionInfo infoA = info;
			infoA.self = a;
			infoA.other = b;

            CollisionInfo infoB = info;
			infoB.self = b;
			infoB.other = a;

			infoB.normal = -info.normal;

            if (info.hit)
            {
                CollisionPair pair(a, b);
				currentPairs[pair] = info;
				// 前回は衝突していなかったペアに対して OnCollisionEnter を呼び出す
                if (previousPairs.count(pair) == 0)
                {
					if (a->GetOwner())
						a->GetOwner()->OnCollisionEnter(infoA);
					if (b->GetOwner())
						b->GetOwner()->OnCollisionEnter(infoB);
                }
				// 前回も衝突していたペアに対して OnCollisionStay を呼び出す
                else
                {
					if (a->GetOwner())
						a->GetOwner()->OnCollisionStay(infoA);
					if (b->GetOwner())
						b->GetOwner()->OnCollisionStay(infoB);
                }

                if (!a->IsTrigger() && !b->IsTrigger())
                {
					ResolveCollision(a, b, info);
                }
            }
        }
    }

	// 前回は衝突していたが今回は衝突していないペアに対して OnCollisionExit を呼び出す
    for (const auto& [pair, info] : previousPairs)
    {
        if (currentPairs.find(pair) == currentPairs.end())
        {
            CollisionInfo infoA = info;

            CollisionInfo infoB = info;
            infoB.self = info.other;
            infoB.other = info.self;
            infoB.normal = -info.normal;

            if (pair.a->GetOwner())
                pair.a->GetOwner()->OnCollisionExit(infoA);

            if (pair.b->GetOwner())
                pair.b->GetOwner()->OnCollisionExit(infoB);
        }
    }
	//ペアを更新
	previousPairs = currentPairs;
}

void ColliderManager::ResolveCollision(BaseCollider* a, BaseCollider* b, const CollisionInfo& info)
{
	//const TCHAR* aText = 0;
	//const TCHAR* bText = 0;
	//aText = a->GetColliderType() == ColliderType::CIRCLE ? L"Circle" : L"Box";
	//bText = b->GetColliderType() == ColliderType::CIRCLE ? L"Circle" : L"Box";

 //   printfDx(
 //       L"A=%s B=%s normal=(%.2f %.2f)\n",
 //       aText,
 //       bText,
 //       info.normal.x,
 //       info.normal.y
 //   );

    Rigidbody* rbA = a->GetOwner()->GetRigidbody();
    Rigidbody* rbB = b->GetOwner()->GetRigidbody();

    // 両方とも無いなら何もしない
    if (!rbA && !rbB)
        return;

	//Aが動いてBが動いていない場合、Aを押し戻す
    if (rbA && !rbA->IsStatic() && (!rbB || rbB->IsStatic()))
    {
        if (info.normal.y < -0.9f)
        {
            rbA->SetIsGround(true);
            if (rbA->GetVelocity().y > 0) {
                rbA->SetVelocity({ rbA->GetVelocity().x, 0 });
            }
        }
        a->GetOwner()->SetPosition(a->GetOwner()->GetPosition() + info.normal * info.penetration);
    }
	//Bが動いてAが動いていない場合、Bを押し戻す
    else if ((!rbA || rbA->IsStatic()) && rbB && !rbB->IsStatic())
    {
        if (info.normal.y > 0.9f)
        {
            rbB->SetIsGround(true);
            if (rbB->GetVelocity().y > 0) {
                rbB->SetVelocity({ rbB->GetVelocity().x, 0 });
            }
        }
        b->GetOwner()->SetPosition(b->GetOwner()->GetPosition() - info.normal * info.penetration);
    }
	//両方動いている場合、両方を半分ずつ押し戻す
    else if (rbA && !rbA->IsStatic() && rbB && !rbB->IsStatic())
    {
		if (info.normal.y < -0.9f)
		{
			rbA->SetIsGround(true);
            if (rbA->GetVelocity().y > 0){
                rbA->SetVelocity({ rbA->GetVelocity().x, 0 });
            }
		}
		else if (info.normal.y > 0.9f)
		{
			rbB->SetIsGround(true);
            if (rbB->GetVelocity().y > 0) {
                rbB->SetVelocity({ rbB->GetVelocity().x, 0 });
            }
		}
        a->GetOwner()->SetPosition(a->GetOwner()->GetPosition() + info.normal * (info.penetration * 0.5f));
        b->GetOwner()->SetPosition(b->GetOwner()->GetPosition() - info.normal * (info.penetration * 0.5f));
    }
}


CollisionInfo ColliderManager::CheckPair(BaseCollider* a, BaseCollider* b)
{

	CollisionInfo info;

    switch (a->GetColliderType())
    {
    case ColliderType::CIRCLE:
        switch (b->GetColliderType())
        {
        case ColliderType::CIRCLE:
            info = CircleVsCircle(static_cast<CircleCollider*>(a),
                static_cast<CircleCollider*>(b));
            break;
        case ColliderType::BOX:
            info = CircleVsBox(static_cast<CircleCollider*>(a),
                static_cast<BoxCollider*>(b));
            break;
        default:
            break;
        }
        break;

    case ColliderType::BOX:
        switch (b->GetColliderType())
        {
        case ColliderType::CIRCLE:
            info = BoxVsCircle(static_cast<BoxCollider*>(a),
                static_cast<CircleCollider*>(b));
            break;
        case ColliderType::BOX:
            info = BoxVsBox(static_cast<BoxCollider*>(a),
                static_cast<BoxCollider*>(b));
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

	if (info.hit)
    {
        info.self = a;
        info.other = b;
    }

    return info;
}

CollisionInfo ColliderManager::CircleVsCircle(
     CircleCollider* a,
     CircleCollider* b)
{
	//diffはbの中心からaの中心へのベクトル
    Vector2D<float> diff =
        a->GetWorldPosition() - b->GetWorldPosition();

    float r = a->GetRadius() + b->GetRadius();
    if (diff.LengthSq() <= r * r)
    {
		CollisionInfo info;
        float dist = FCast(diff.Length());
		if (dist == 0)
		{
			info.normal = { 0, -1 };
			info.penetration = r;
		}
		else
		{
			info.normal = diff / dist;
			info.penetration = r - dist;
		}
		info.hit = true;
		return info;
    }
	else
	{
		return {};
	}
}

CollisionInfo ColliderManager::BoxVsBox(
     BoxCollider* a,
     BoxCollider* b)
{
    Vector2D<float> pa = a->GetWorldPosition();
    Vector2D<float> pb = b->GetWorldPosition();
    Vector2D<float> ha = a->GetHalfSize();
    Vector2D<float> hb = b->GetHalfSize();

    if (Abs(pa.x - pb.x) <= (ha.x + hb.x) &&
        Abs(pa.y - pb.y) <= (ha.y + hb.y)){
        CollisionInfo info;

        float overlapX =
            (ha.x + hb.x) - Abs(pa.x - pb.x);

        float overlapY =
            (ha.y + hb.y) - Abs(pa.y - pb.y);

        if (overlapX < overlapY)
        {
            info.penetration = overlapX;

            if (pa.x < pb.x) info.normal = { -1, 0 };
            else info.normal = { 1, 0 };
        }
        else
        {
            info.penetration = overlapY;

            if (pa.y < pb.y) info.normal = { 0, -1 };
            else info.normal = { 0, 1 };
        }
		info.hit = true;
		return info;
    }
    else{
        return {};
    }
}

CollisionInfo ColliderManager::BoxVsCircle(
    BoxCollider* box,
    CircleCollider* circle)
{
    CollisionInfo info = CircleVsBox(circle, box);

    if (info.hit)
    {
        info.normal = -info.normal;
    }

    return info;
}
CollisionInfo ColliderManager::CircleVsBox(
    CircleCollider* c,
    BoxCollider* b)
{

    Vector2D<float> cp = c->GetWorldPosition();
    Vector2D<float> bp = b->GetWorldPosition();
    Vector2D<float> h = b->GetHalfSize();

    float x = Clamp(cp.x, bp.x - h.x, bp.x + h.x);
    float y = Clamp(cp.y, bp.y - h.y, bp.y + h.y);

    Vector2D<float> closest{ x, y };

    Vector2D<float> diff = cp - closest;
    float distSq = FCast(diff.LengthSq());
    float radius = c->GetRadius();

    if (distSq <= radius * radius)
    {
        CollisionInfo info;
        info.hit = true;
        float dist = sqrtf(distSq);
        if (dist == 0)
        {
            float left = cp.x - (bp.x - h.x);
            float right = (bp.x + h.x) - cp.x;
            float top = cp.y - (bp.y - h.y);
            float bottom = (bp.y + h.y) - cp.y;

            float minDist = left;
            info.normal = { -1,0 };

            if (right < minDist)
            {
                minDist = right;
                info.normal = { 1,0 };
            }

            if (top < minDist)
            {
                minDist = top;
                info.normal = { 0,-1 };
            }

            if (bottom < minDist)
            {
                minDist = bottom;
                info.normal = { 0,1 };
            }

            info.penetration = radius + minDist;
        }
        else
        {
            info.normal = diff / dist;
            info.penetration = radius - dist;
        }

        return info;
    }
    else
    {
        return {};
    }

}

void ColliderManager::RemoveCollider(BaseCollider* collider)
{
    if (!collider) return;

    colliders.erase(
        std::remove(colliders.begin(), colliders.end(), collider),
        colliders.end());

    auto removePairs = [collider](auto& pairs)
        {
            for (auto it = pairs.begin(); it != pairs.end();)
            {
                if (it->first.a == collider || it->first.b == collider)
                {
                    it = pairs.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        };

    removePairs(currentPairs);
    removePairs(previousPairs);
}

void ColliderManager::DestroyedColliderCheck()
{
    for (auto it = colliders.begin(); it != colliders.end();)
    {
        BaseCollider* collider = *it;

        if (!collider || collider->IsDestroyed())
        {
            RemoveCollider(collider);

            // RemoveCollider内でeraseされるので先頭からやり直す
            it = colliders.begin();
        }
        else
        {
            ++it;
        }
    }
}

#if COLLIDER_DEBUG
void ColliderManager::DebugDraw() 
{
    for (auto& a : colliders) {
        a->DebugDraw();
    }
}
#endif
