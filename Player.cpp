#include "stdafx.h"
#include "Player.h"
#include "ColliderManager.h"
#include "RigidbodyManager.h"
#include "InputManager.h"

void Player::Initialize()
{
	ColliderManager::GetInstance().Create<BoxCollider>(this, Vec2f{ 0,0 }, Vec2f{ 50,50 });
	RigidbodyManager::GetInstance().Create<Rigidbody>(this, true)->SetGravityScale(15.0f)->SetVelocityThreshold(Vec2f{40.0f})->SetLinearDamping(0.99f);
}

void Player::Finalize()
{
	
}

void Player::Update()
{
	if (InputManager::GetInstance().IsPress(KEY_INPUT_LEFT))
	{
		Vec2f vec = GetRigidbody()->GetVelocity();
		vec.x = -100.0f;
		GetRigidbody()->SetVelocity(vec);
	}
	if (InputManager::GetInstance().IsPress(KEY_INPUT_RIGHT))
	{
		Vec2f vec = GetRigidbody()->GetVelocity();
		vec.x = 100.0f;
		GetRigidbody()->SetVelocity(vec);
	}

	if (InputManager::GetInstance().IsTrigger(KEY_INPUT_SPACE))
	{
		Vec2f vec = GetRigidbody()->GetVelocity();
		vec.y += -1000.0f;
		GetRigidbody()->SetVelocity(vec);
	}

}

void Player::Draw(const Camera& camera)
{
	Vec2f screenPos = camera.ToScreen(pos);
	DrawBoxAA(screenPos.x + 50, screenPos.y - 50, screenPos.x - 50, screenPos.y + 50, GetColor(255, 0, 0), TRUE);
}