#pragma once
#include "BaseCollider.h"
#include "Collider.h"
#include "GameObject.h"
#include <unordered_set>

struct CollisionPair
{
    BaseCollider* a;
    BaseCollider* b;

    CollisionPair(BaseCollider* a_, BaseCollider* b_)
        : a(a_), b(b_)
    {
        if (a > b)
        {
            std::swap(a, b);
        }
    }

    bool operator==(const CollisionPair& other) const
    {
        return a == other.a &&
            b == other.b;
    }
};

struct CollisionPairHash
{
    size_t operator()(const CollisionPair& p) const
    {
        size_t h1 = std::hash<BaseCollider*>{}(p.a);
        size_t h2 = std::hash<BaseCollider*>{}(p.b);

        return h1 ^ (h2 << 1);
    }
};

class ColliderManager
{
private:
    std::vector<BaseCollider*> colliders;
    ColliderManager() = default;

    std::unordered_set<CollisionPair, CollisionPairHash> currentPairs;
    std::unordered_set<CollisionPair, CollisionPairHash> previousPairs;

    CollisionInfo CheckPair(BaseCollider* a, BaseCollider* b);
    CollisionInfo CircleVsCircle(CircleCollider* a, CircleCollider* b);
    CollisionInfo BoxVsBox(BoxCollider* a, BoxCollider* b);
    CollisionInfo CircleVsBox(CircleCollider* c, BoxCollider* b);
    CollisionInfo BoxVsCircle(BoxCollider* b, CircleCollider* c);

    void ResolveCollision(BaseCollider* a, BaseCollider* b, CollisionInfo& info);

public:
    static ColliderManager& GetInstance()
    {
        static ColliderManager instance;
        return instance;
    }

    template<typename T, typename... Args>
    T* Create(GameObject* parent, Args&&... args)
    {
        static_assert(std::is_base_of<BaseCollider, T>::value,
            "T must derive from BaseCollider 訳:これBaseCollider継承してないよ...");

        auto obj = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = obj.get();
        parent->SetCollider(std::move(obj));
        colliders.push_back(ptr);

        return ptr;
    }

	// 登録解除関数 SceneのKIllで必ず毎フレーム呼ぶ.
	void DestroyedColliderCheck()
	{
		colliders.erase(std::remove_if(colliders.begin(), colliders.end(),
			[](BaseCollider* col) { return !col || col->IsDestroyed(); }),
			colliders.end());
	}

    // 衝突判定
    void CheckAllCollisions();
#if COLLIDER_DEBUG
    void DebugDraw();
#endif 

};
