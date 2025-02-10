#pragma once
#include <string>
#include <unordered_map>
#include "Engine/Components/Meshes/Mesh.h"
#include "Engine/Components/Materials/Material.h"
#include "Engine/Components/GameObject/Model.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Vector4.h"
#include "Engine/Math/Matrix4x4.h"

/// <summary>
/// CPUで行うparticle
/// </summary>
class CpuParticles {
public:	// メンバ構造体

	struct Particle {
		Vector4 color;			// 色
		Vector3 scale;			// 拡縮
		Vector3 rotate;			// 回転
		Vector3 translate;		// 座標
		Vector3 velocity;		// 速度
		Vector3 acceleration;	// 加速度
		float lifeTime;			// 生存時間
		float currentTime;		// 現在の時間
		float damping;			// 減衰
		float gravity;			// 重力
	};

	struct PerView {
		Matrix4x4 viewProjection;
		Matrix4x4 billboardMat;
	};

	struct PerFrame {
		float time;
		float deltaTime;
	};
public:

	CpuParticles();
	~CpuParticles();

	void Finalize();

	void Init();
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList);

private:

	std::list<Particle> particleList_;

	std::vector<std::unique_ptr<Mesh>> meshArray_;
	std::vector<std::unique_ptr<Material>> materials_;

	std::unordered_map<std::string, Model::ModelMaterialData> materialData_;
};

