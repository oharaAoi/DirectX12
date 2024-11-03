#pragma once
#include "Engine/GameObject/BaseGameObject.h"
#include "Game/GameObject/ClutchWire.h"
#include "WireTip.h"

class Player
	: public BaseGameObject {
public:

	Player();
	~Player();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void SetInverMatrix(const Matrix4x4& inver);
	void SetCameraZDis(float z);

#ifdef _DEBUG
	void Debug_Gui();
#endif

	Collider* GetWireTipCollider() { return wireTip_.get(); }

	const Vector3 GetForward() const { return TransformNormal(Vector3(0,0,1), transform_->GetWorldMatrix()); }
	const Vector3 GetWorldPos() const { return Transform(Vector3(0, 0, 0), transform_->GetWorldMatrix()); }

private:

	/// ==========================================
	/// プライベート関数
	/// ==========================================
	void Move();
	void Clutch();





	/// ==========================================
	/// 変数
	/// ==========================================

	// 移動
	float moveSpeed_ = 6.0f;

	// クラッチ
	std::unique_ptr<ClutchWire> wire_;
	float maxClutchLength_ = 7.0f;
	float stretchSpeed_ = 30.0f;
	float camerazDis_ = 0.0f;
	Vector2 clutchEnd_{};
	float returnSpeed_ = 0.3f;
	bool isReturnClutch_ = false;// 最大まで伸びて戻るか
	bool isStretchClutch_ = false;// 伸びてる状態か
	bool isStretching_ = false;
	bool isRekey_ = true;// 一回ボタンから手離したかった
	Matrix4x4 inverMat_;



	// ワイヤー先端
	std::unique_ptr<WireTip> wireTip_;




};

