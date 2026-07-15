#include "stdafx.h"
#include "Player.h"
#include "ColliderManager.h"
#include "RigidbodyManager.h"
#include "InputManager.h"

void Player::Initialize()
{
	ColliderManager::GetInstance().Create<BoxCollider>(this, Vec2f{ 0,0 }, Vec2f{ 50,50 });
	RigidbodyManager::GetInstance().Create<Rigidbody>(this, true)->SetGravityScale(15.0f)->SetVelocityThreshold(Vec2f{40.0f})->SetLinearDamping(0.99f);

	CreateAnimation();
	animator.ChangeAnimation(playerAnimation, true);
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

	if (InputManager::GetInstance().IsPress(KEY_INPUT_E))
	{
		angle += 1.0f;
	}
	else if (InputManager::GetInstance().IsPress(KEY_INPUT_Q))
	{
		angle -= 1.0f;
	}

}

void Player::Draw(const Camera& camera)
{
	Vec2f screenPos = camera.ToScreen(pos);
	animator.Draw(screenPos, 1.0f, angle, true);
	//DrawBoxAA(screenPos.x + 50, screenPos.y - 50, screenPos.x - 50, screenPos.y + 50, GetColor(255, 0, 0), TRUE);
}

void Player::CreateAnimation()
{
	std::vector<Frame> frames;

	frames.push_back(MakeFrame(-1, { Rect(Vec2f{100,100},50,50,GetColor(100,100,100),true),
									 Circle(Vec2f{20,20},25,GetColor(255,0,0),true   )}));

	playerAnimation.SetFrames(frames, 100);
}