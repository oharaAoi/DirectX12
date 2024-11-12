#pragma once
#include <memory>
#include <vector>
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Collider/MeshCollider.h"
#include "Game/GameObject/Player.h"
#include "Game/UI/KnockDownEnemy.h"

class Boss : public BaseGameObject {
public:

	Boss();
	~Boss();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void OnCollision(MeshCollider& other);

#ifdef _DEBUG
	void Debug_Gui();
#endif

	MeshCollider* GetMeshCollider() { return meshCollider_.get(); }

	//const Vector3 GetWorldPos() const;

	const bool GetIsFinish() const { return isFinish_; }

	const bool GetIsMove() const { return isMove_; }
	void SetIsMove(bool isMove) { isMove_ = isMove; }

private:

	std::unique_ptr<MeshCollider> meshCollider_;

	float buttleTime_;
	float buttleLimite_;

	std::vector<Vector3> controlPoints_;

	float t_;

	bool isFinish_;
	bool isMove_;
	bool isAlive_;

	float hitedTime_;
};

