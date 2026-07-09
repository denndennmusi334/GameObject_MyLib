#include "stdafx.h"
#include "TestScene.h"
#include "Player.h"

TestScene::TestScene(SceneController* changer) :
	BaseScene(changer)
{
	gameObjectMgr = &GameObjectManager::GetInstance();
	colliderMgr = &ColliderManager::GetInstance();
	rigidbodyMgr = &RigidbodyManager::GetInstance();
}

TestScene::~TestScene()
{
}

void TestScene::Initialize()
{
	player = gameObjectMgr->Create<Player>();
}

void TestScene::Finalize()
{
}

void TestScene::Update()
{
	rigidbodyMgr->Update();
	gameObjectMgr->Update();

	Vec2f currentPos = player->GetPosition();
	currentPos.y -= Config::SCREEN_HEIGHT / 2;
	currentPos.x -= Config::SCREEN_WIDTH / 2;
	camera.SetCameraPos(currentPos);

	colliderMgr->CheckAllCollisions();
}

void TestScene::Draw()
{
	drawMgr.DrawAll(camera);

#if RB_DEBUG
	rigidbodyMgr->DebugDraw(camera);
#endif // RB_DEBUG
}

void TestScene::Kill()
{
	drawMgr.DestroyedDrawableCheck();
	rigidbodyMgr->DestroyedRigidbodyCheck();
	colliderMgr->DestroyedColliderCheck();

	gameObjectMgr->CheckDestroyedObjects();
}