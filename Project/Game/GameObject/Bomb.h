#pragma once
#include "Engine/GameObject/BaseGameObject.h"

class Bomb : public BaseGameObject {
public:

	Bomb();
	~Bomb();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void Pop(const Vector3& pos, const Vector3& acceleration = Vector3::ZERO());

	void ExecuteFunc(void (Bomb::* func)()) { (this->*func)(); }
	void NormalMove();
	void PullwMove();

#ifdef _DEBUG
	void Debug_Draw();
#endif
public:

	MeshCollider* GetMeshCollider() { return meshCollider_.get(); }

	// 生存フラグの取得・設定
	const bool GetIsAlive() const { return isAlive_; }
	void SetIsAlive(bool isAlive) { isAlive_ = isAlive; }

	/// <summary>
	/// ボムが持つプレイヤーの情報を更新
	/// </summary>
	/// <param name="isNeglect">:Player->GetWireTip()->GetNeglect()で取得</param>
	/// <param name="isPull">:Player->GetPull()で取得</param>
	/// <param name="wireTipPos">:Player->GetWireTip()->GetTransform()->GetTranslation()で取得</param>
	void SetPlayerInfoHeldByBomb(bool isNeglect, bool isPull, const Vector3& wireTipPos);

private:

	void OnCollisionEnter([[maybe_unused]] MeshCollider& other);
	void OnCollisionStay([[maybe_unused]] MeshCollider& other);
	void OnCollisionExit([[maybe_unused]] MeshCollider& other);

private:

	void (Bomb::*pFunc_)();

	bool isAlive_ = false;
	bool isPulling_ = false;
	bool playerNeglect_ = false;
	bool isPlayerPull_ = false;

	Vector3 wireTipPos_;
	Vector3 velocity_;
	Vector3 acceleration_;
};

