#pragma once
#include <memory>
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Collider/MeshCollider.h"

class Enemy
	: public BaseGameObject {
public:

	Enemy();
	~Enemy();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void OnCollision(MeshCollider& other);

#ifdef _DEBUG
	void DeBug_Gui() {};
#endif

	MeshCollider* GetMeshCollider() { return meshCollider_.get(); }

	const Vector3 GetWorldPos() const;

	const bool GetIsAlive() const { return isAlive_; }
	
private:

	std::unique_ptr<MeshCollider> meshCollider_;

	bool isAlive_;

};

