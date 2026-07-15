#pragma once
#include "GameObject.h"
class Player :
    public GameObject
{
private:
	Animation playerAnimation;
public:
	Player() = default;;

	void CreateAnimation() override ;

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw(const Camera& camera) override;
};

