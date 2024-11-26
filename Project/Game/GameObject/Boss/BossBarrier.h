#pragma once
#include "Engine/Math/Easing.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/GameObject/BaseGameObject.h"

/// <summary>
/// Bossをまもる障壁
/// </summary>
class BossBarrier : public BaseGameObject {
public:

	struct TimeCount {
		float time;
		float totalTime;
		float pad[2];
		Vector3 cameraPos;
	};

	struct BarrierAction {
		bool isAct;
		float time;
		float timeLimit;
		int easingType;
	};

public:

	BossBarrier();
	~BossBarrier();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void AdaptAdjustment();

private:

	/// <summary>
	/// 展開する
	/// </summary>
	void Expand();

	/// <summary>
	/// 割れる
	/// </summary>
	void Break();

	void OnCollisionEnter([[maybe_unused]] MeshCollider& other);
	void OnCollisionStay([[maybe_unused]] MeshCollider& other);
	void OnCollisionExit([[maybe_unused]] MeshCollider& other);

public:

#ifdef _DEBUG
	void Debug_Gui();

	void Debug_Draw();
#endif

	MeshCollider* GetMeshCollider() { return meshCollider_.get(); }

	// バリアを貼る
	void SetBarrier() { expand_.isAct = true; }

	// バリアを機能させる
	void SetEnableFunction(bool isWork) { enableFunction_ = isWork; }
	const bool GetEnableFunction() const { return enableFunction_; }
	const bool GetBreakBarrier()const { return break_.isAct; }
	const bool GetIsBreak()const { return isBreak_; }
	const bool GetIsExpand()const;

private:

	std::string groupName_ = "boss_barrier";

	ComPtr<ID3D12Resource> timeBuffer_;
	TimeCount* timeData_;

	BarrierAction expand_;
	BarrierAction break_;

	uint32_t hp_;
	const uint32_t Durability_ = 3; // 耐久度

	bool enableFunction_ = false;
	bool isBreak_ = false;
};

