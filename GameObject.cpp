#include "stdafx.h"
#include "GameObject.h"

void GameObject::SetCollider(std::unique_ptr<BaseCollider> col)
{
    col->SetOwner(this);
	if (collider) {
		collider->Destroy();
	}
	collider = std::move(col);
}

void GameObject::SetRigidbody(std::unique_ptr<Rigidbody> rb)
{
	rb->SetOwner(this);
	if (rigidbody) {
		rigidbody->SetOwner(nullptr);
	}
	rigidbody = std::move(rb);
}
