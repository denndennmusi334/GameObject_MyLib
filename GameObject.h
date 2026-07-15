#pragma once
#include "BaseCollider.h"
#include "Rigidbody.h"
#include "Drawable.h"

class GameObject : public Drawable
{
private:
    std::unique_ptr<BaseCollider> collider;
    std::unique_ptr<Rigidbody> rigidbody;
protected:
    Vector2D<float> pos = { 0,0 };
	float angle = 0.0f;
public:
    
    Vector2D<float> GetPosition() const { return pos; }
    void SetPosition(const Vector2D<float>& p) { pos = p; }

    void Destroy() { 
        Drawable::Destroy();
        if (collider) {
			collider->Destroy();
        }
        if (rigidbody) {
            rigidbody->Destroy();
        }
    }

    void SetCollider(std::unique_ptr<BaseCollider> col);
	void SetRigidbody(std::unique_ptr<Rigidbody> rb);

    const auto& GetCollider() const { return collider; }
	auto* GetRigidbody() { return rigidbody.get(); }

    virtual void OnCollisionEnter(const CollisionInfo& info) {};
    virtual void OnCollisionStay(const CollisionInfo& info)  {};
    virtual void OnCollisionExit(const CollisionInfo& info)  {};

	void CreateAnimation() override {};

    void Initialize()   override {}; //初期化.
    void Finalize()     override {}; //終了処理.
    void Update()       override {}; //更新.
    void Draw(const Camera& camera) override {};

};
