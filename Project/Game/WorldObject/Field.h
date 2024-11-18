#pragma once
#include "Engine/GameObject/BaseGameObject.h"

/// <summary>
/// 大方となるマップ
/// </summary>
class Field : public BaseGameObject {
public:

	Field();
	~Field();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

#ifdef _DEBUG
	void Debug_Gui();
#endif

	MeshCollider* GetMeshCollider() { return meshCollider_.get(); }

private:

	void OnCollisionEnter([[maybe_unused]] MeshCollider& other);
	void OnCollisionStay([[maybe_unused]] MeshCollider& other);
	void OnCollisionExit([[maybe_unused]] MeshCollider& other);

private:



};