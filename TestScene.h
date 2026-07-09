#pragma once
#include "BaseScene.h"
#include "GameObjectManager.h"
#include "ColliderManager.h"
#include "RigidbodyManager.h"

class TestScene :
    public BaseScene
{
private:
	GameObjectManager* gameObjectMgr	= nullptr;
	ColliderManager* colliderMgr		= nullptr;
	RigidbodyManager* rigidbodyMgr		= nullptr;
	GameObject* player = nullptr;
public:
	TestScene(SceneController* changer);
	~TestScene() override;
	void Initialize() override; //‰Šú‰».
	void Finalize() override; //I—¹ˆ—.
	void Update() override; //XV.
	void Draw() override; //•`‰æ.
	void Kill() override;
};

