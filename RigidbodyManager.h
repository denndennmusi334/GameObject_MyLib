#pragma once
#include "Rigidbody.h"
#include "GameObject.h"
#include "Camera.h"

class RigidbodyManager
{
private:
	std::vector<Rigidbody*> rigidbodies;
	float dTime = 0.0f;
public:
	void Update();
#if RB_DEBUG
	void DebugDraw(const Camera& camera) const;
#endif // RB_DEBUG
	void DestroyedRigidbodyCheck()
	{
		rigidbodies.erase(std::remove_if(rigidbodies.begin(), rigidbodies.end(),
            [](Rigidbody* rb) { return !rb || rb->IsDestroyed(); }),
			rigidbodies.end());
	}
    template<typename T, typename... Args>
    T* Create(GameObject* parent, Args&&... args)
    {
        static_assert(std::is_base_of<Rigidbody, T>::value,
            "T must derive from BaseCollider ñÛ:Ç±ÇÍBaseCollideråpè≥ÇµÇƒÇ»Ç¢ÇÊ...");

        auto obj = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = obj.get();
        parent->SetRigidbody(std::move(obj));
        rigidbodies.push_back(ptr);

        return ptr;
    }

    static RigidbodyManager& GetInstance()
    {
        static RigidbodyManager instance;
        return instance;
    }

};

