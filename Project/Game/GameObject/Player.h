#pragma once
#include "Engine/GameObject/BaseGameObject.h"
#include "Game/GameObject/ClutchWire.h"
#include "WireTip.h"
#include "Engine/Collider/MeshCollider.h"
#include "Engine/Math/Easing.h"


enum class PlayerState {
	Default = 0,
	Attack = 1,
};


class Player
	: public BaseGameObject {
public:

	Player();
	~Player();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void OnCollision(MeshCollider& other);

	void SetInverMatrix(const Matrix4x4& inver);
	void SetCameraZDis(float z);

	WireTip* GetWireTip() { return wireTip_.get(); }



#ifdef _DEBUG
	void Debug_Gui();

	void Debug_Draw();
#endif

	MeshCollider* GetMeshCollider() { return meshCollider_.get(); }

	// objectを投げるように
	Vector3 GetThrowVelo()const;
	bool GetThrow() { return isThrow_; }
	void SetThrow(bool is) { isThrow_ = is; }


	bool GetPull() { return isPull_; }
	void SetPull(bool is) { isPull_ = is; }


	Collider* GetWireTipCollider() { return wireTip_.get(); }

	const Vector3 GetForward() const { return TransformNormal(Vector3(0,0,1), transform_->GetWorldMatrix()); }
	const Vector3 GetWorldPos() const { return Transform(Vector3(0, 0, 0), transform_->GetWorldMatrix()); }

private:

	/// ==========================================
	/// プライベート関数
	/// ==========================================
	void Move();
	void Clutch();

	void BaseClutch();
	void FirstClutch();
	void Stretching();


	/// ==========================================
	/// 変数
	/// ==========================================

	// 移動
	Vector3 velocity_{};
	float moveSpeed_ = 6.0f;
	float gravity_ = -0.8f;
	float rightRotate = 1.59f;
	float leftRotate = -1.58f;
	float targetRotate = 0.0f;
	float nowRotate = 0.0f;

	// クラッチ
	std::unique_ptr<ClutchWire> wire_;
	float maxClutchLength_ = 7.0f;
	float stretchSpeed_ = 30.0f;
	float camerazDis_ = 0.0f;
	Vector2 clutchEnd_{};
	float clutchLerpTime_ = 0.0f;
	int easingIndex_ = int(EasingType::Out::Cubic);

	float returnSpeed_ = 0.3f;
	bool isReturnClutch_ = false;// 最大まで伸びて戻るか
	bool isStretchClutch_ = false;// 伸びてる状態か
	bool isStretching_ = false;
	bool isRekey_ = true;// 一回ボタンから手離したかった
	bool isSnagged_ = false;
	bool isPull_ = false;
	float pullWeight_ = 0.0f;
	Matrix4x4 inverMat_;



	// ワイヤー先端
	std::unique_ptr<WireTip> wireTip_;


	// 投げる
	bool isThrow_ = false;


	/// ==========================================
	/// Collider
	/// ==========================================

	std::unique_ptr<MeshCollider> meshCollider_;
};

