#pragma once
#include "Camera.h"
class GameObject;

class Rigidbody
{
private:
    Vector2D<float> velocity{ 0,0 };

    float gravityScale = 1.0f;
    bool useGravity = true;	//重力を使うかどうか

	GameObject* owner = nullptr;

	bool isDestroyed = false;
	bool isStatic = false;
	bool isGround = false;

	Vector2D<float> velocityThreshold{ 0.01f, 0.01f }; // 速度の閾値

	float linearDamping = 0.1f; // 速度減衰の係数
public:

	Rigidbody() = default;
	Rigidbody(bool _useGravity) : useGravity(_useGravity) {}

#pragma region ゲッターセッター

	Rigidbody* SetVelocity(const Vector2D<float>& v) { 
		velocity = v; 
		return this;
	}
	Vector2D<float> GetVelocity() const { return velocity; }

	Rigidbody* SetGravityScale(float scale) {
		gravityScale = scale;
		return this;
	}
	float GetGravityScale() const { return gravityScale; }

	Rigidbody* SetUseGravity(bool use) {
		useGravity = use;
		return this;
	}
	bool IsUsingGravity() const { return useGravity; }

	GameObject* GetOwner() const { return owner; }
	void SetOwner(GameObject* obj) { owner = obj; }

	Rigidbody* SetIsStatic(bool s) { 
		isStatic = s; 
		return this;
	}
	bool IsStatic() const { return isStatic; }

	Rigidbody* SetIsGround(bool g) {
		isGround = g;
		return this;
	}
	bool IsGround() const { return isGround; }

	void Destroy() { isDestroyed = true; }
	bool IsDestroyed() const { return isDestroyed; }

	/// <summary>
	/// 空気抵抗のような速度を減衰させる効果を設定する関数.
	/// </summary>
	/// <param name="damping">1.0 ～ 0.0</param>
	Rigidbody* SetLinearDamping(float damping) {
		linearDamping = damping;
		return this;
	}
	float GetLinearDamping() const { return linearDamping; }

	Rigidbody* SetVelocityThreshold(const Vector2D<float>& threshold) {
		velocityThreshold = threshold;
		return this;
	}
	Vector2D<float> GetVelocityThreshold() const { return velocityThreshold; }
#pragma endregion

	void Update(const  float &dTime);
#if RB_DEBUG
	void DebugDraw(const Camera& camera) const;
#endif // RB_DEBUG
};

