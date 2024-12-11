#pragma once
#include <functional>

enum CollisionFlags {
	NONE = 0b00,
	ENTER = 0b01,
	EXIT = 0b10,
	STAY = 0b11,
};

/// <summary>
/// Colliderの基底クラス
/// </summary>
class ICollider {
public:

	ICollider() = default;
	virtual ~ICollider() = default;

	virtual void Init() {};
	virtual void Update() {};
	virtual void Draw() const {};

public:

	/// <summary>
	/// 状態の変更
	/// </summary>
	void SwitchCollision();

	/// <summary>
	/// 衝突時にコールバック関数を呼び出す
	/// </summary>
	/// <param name="other"></param>
	void OnCollision(ICollider& other);

	/// <summary>
	/// 最初の衝突時に呼ばれる関数の設定
	/// </summary>
	/// <param name="callback"></param>
	void SetCollisionEnter(std::function<void(ICollider&)> callback) {
		onCollisionEnter_ = callback;
	}

	/// <summary>
	/// 最初の衝突時に呼ばれる関数の設定
	/// </summary>
	/// <param name="callback"></param>
	void SetCollisionStay(std::function<void(ICollider&)> callback) {
		onCollisionStay_ = callback;
	}

	/// <summary>
	/// 最初の衝突時に呼ばれる関数の設定
	/// </summary>
	/// <param name="callback"></param>
	void SetCollisionExit(std::function<void(ICollider&)> callback) {
		onCollisionExit_ = callback;
	}

private:

	/// <summary>
	/// 最初の衝突時に呼ばれる関数
	/// </summary>
	/// <param name="other">: 他の衝突物</param>
	void OnCollisionEnter(ICollider& other);

	/// <summary>
	/// 衝突中に呼ばれる関数
	/// </summary>
	/// <param name="other">: 他の衝突物</param>
	void OnCollisionStay(ICollider& other);

	/// <summary>
	/// 衝突しなくなったら呼ばれる関数
	/// </summary>
	/// <param name="other">: 他の衝突物</param>
	void OnCollisionExit(ICollider& other);

protected:

	int collisionState_;

	// 衝突用のコールバック
	std::function<void(ICollider&)> onCollisionEnter_;
	std::function<void(ICollider&)> onCollisionStay_;
	std::function<void(ICollider&)> onCollisionExit_;
};

