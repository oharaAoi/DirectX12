#pragma once
#include "Engine/GameObject/BaseGameObject.h"

/// <summary>
/// Bossをまもる障壁
/// </summary>
class BossBarrier : public BaseGameObject {
public:

	BossBarrier();
	~BossBarrier();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

private:

	void OnCollisionEnter([[maybe_unused]] MeshCollider& other);
	void OnCollisionStay([[maybe_unused]] MeshCollider& other);
	void OnCollisionExit([[maybe_unused]] MeshCollider& other);

#ifdef _DEBUG
	void Debug_Gui();
#endif

	MeshCollider* GetMeshCollider() { return meshCollider_.get(); }

private:




};

