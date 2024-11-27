#pragma once
#include <memory>
#include <optional>
#include "Engine/GameObject/BaseGameObject.h"
#include "Game/GameObject/Clutch/ClutchWire.h"
#include "Game/GameObject/Clutch/WireTip.h"
#include "Engine/Math/Easing.h"
#include "Game/GameObject/Missile.h"
#include "Game/GameObject/State/PlayerRootState.h"
#include "Game/GameObject/State/PlayerBeAttackedState.h"
#include "Game/GameObject/Player/PlayerAnimator.h"
#include "Engine/Utilities/AdjustmentItem.h"


class TitlePlayer
	: public BaseGameObject {
public:

	TitlePlayer();
	~TitlePlayer();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void TitleSet();
	void TitleEnd();

	void ClearSet();
	void ClearUpdate();

	/// <summary>
	/// 調整項目の適応
	/// </summary>
	void AdaptAdjustment();


	void SetInverMatrix(const Matrix4x4& inver);
	void SetCameraZDis(float z);


#ifdef _DEBUG
	void Debug_Gui();

	void Debug_Draw();
#endif

private:

	/// ==========================================
	/// プライベート関数
	/// 

	void Stretching();

	void OnCollisionEnter([[maybe_unused]] MeshCollider& other);
	void OnCollisionStay([[maybe_unused]] MeshCollider& other);
	void OnCollisionExit([[maybe_unused]] MeshCollider& other);


private:

	/// ==========================================
	/// 変数
	/// ==========================================

	const std::string groupName_ = "player";


	// 移動
	Vector3 velocity_{};
	float moveSpeed_ = 6.0f;
	float gravity_ = -29.4f;
	float rightRotate = 1.59f;
	float leftRotate = -1.58f;
	float targetRotate = 0.0f;
	float nowRotate = 0.0f;
	float groundLine_ = 1.2f;
	bool isShakeBook_ = false;

	// クラッチ
	std::unique_ptr<ClutchWire> wire_;
	const float defaultMaxClutchLength_ = 7.0f;
	float maxClutchLength_ = defaultMaxClutchLength_;
	float stretchSpeed_ = 30.0f;
	float camerazDis_ = 0.0f;
	Vector2 clutchEnd_{};
	float clutchLerpTime_ = 0.0f;
	int easingIndex_ = int(EasingType::Out::Cubic);

	float returnSpeed_ = 0.3f;

	bool isRekey_ = true;// 一回ボタンから手離したかった

	bool isReturnClutch_ = false;// 最大まで伸びて戻るか
	bool isStretchClutch_ = false;// 伸びてる状態か

	bool isStretching_ = false;

	bool isSnagged_ = false;
	bool isPull_ = false;
	float pullWeight_ = 0.0f;
	Matrix4x4 inverMat_;


	// ワイヤー先端
	std::unique_ptr<WireTip> wireTip_;



	/// ==========================================
	/// 他クラスの情報
	/// ==========================================


	AdjustmentItem* adjustItem_;



	/// ==========================================
	/// title用
	/// ==========================================
	float boundTime_ = 0.0f;
	float minBound_ = -1.4f;
	float maxBound_ = 0.7f;

};

