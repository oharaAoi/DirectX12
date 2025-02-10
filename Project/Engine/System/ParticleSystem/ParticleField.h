#pragma once
#include "Render.h"
#include "Engine/Math/MathStructures.h"
#include "Engine/Components/Collider/BoxCollider.h"
#include "Engine/Utilities/DrawUtils.h"
#include "Engine/Lib/GameTimer.h"


class ParticleField {
public: // データ構造体

	/// <summary>
	/// 加速度を与えるField
	/// </summary>
	struct AccelerationField {
		Vector3 acceleration; // 加速度
		AABB area = { {0,0,0}, 1.5f };			  // 範囲
	};

public:

	ParticleField();
	~ParticleField();

	void Init();

	void Update();

	void Draw(const Matrix4x4& vpMatrix) const;

#ifdef _DEBUG
	void Debug_Gui();
#endif

public:

private:

	AccelerationField accelerationField_;

	bool onField_;

};

