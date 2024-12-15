#pragma once
#include <functional>
#include <string>
#include <variant>
#include "Engine/Math/MathStructures.h"

enum CollisionFlags {
	NONE = 0b00,
	ENTER = 0b01,
	EXIT = 0b10,
	STAY = 0b11,
};

enum class ColliderShape {
	SPHERE,
	AABB,
	OBB
};

/// <summary>
/// Colliderの基底クラス
/// </summary>
class ICollider {
public:

	ICollider() = default;
	virtual ~ICollider() = default;

	virtual void Init(const std::string& tag, ColliderShape shape) = 0;
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

	// --------------- tagの設定・取得 -------------- //
	void SetTag(const std::string& tag) { tag_ = tag; }
	const std::string GetTag() const { return tag_; }

	// --------------- shapeの設定・取得 -------------- //
	void SetShape(const std::variant<Sphere, AABB, OBB>& shape) { shape_ = shape; }
	const std::variant<Sphere, AABB, OBB>& GetShape() const { return shape_; }

	// --------------- stateの設定・取得 -------------- //
	void SetCollisionState(int stateBit) { collisionState_ = stateBit; }
	const int GetCollisionState() const { return collisionState_; }

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

	// タグ
	std::string tag_;
	// 形状
	std::variant<Sphere, AABB, OBB> shape_;
	// 当たり判定の状態
	int collisionState_;

	// 衝突時のcallBack
	std::function<void(ICollider&)> onCollisionEnter_;
	std::function<void(ICollider&)> onCollisionStay_;
	std::function<void(ICollider&)> onCollisionExit_;
};

