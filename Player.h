#pragma once
#include "GameObject.h"
class Player :
    public GameObject
{
private:
public:
	Player() = default;;

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw(const Camera& camera) override;
};

