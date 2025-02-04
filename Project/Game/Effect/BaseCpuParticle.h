#pragma once
#include <string>
#include <unordered_map>
#include "Engine/Assets/Meshes/Mesh.h"
#include "Engine/Assets/Materials/Material.h"
#include "Engine/Assets/GameObject/Model.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Vector4.h"
#include "Engine/Math/Quaternion.h"
#include "Engine/Math/Matrix4x4.h"
#include "Engine/DirectX/Resource/ShaderResource.h"


class BaseCpuParticle {
public:	// メンバ構造体

	struct GpuData {
		Matrix4x4 worldMat;
		Vector4 color;
	};

	struct Particle {
		Vector4 color;			// 色
		Vector3 scale;			// 拡縮
		Quaternion rotate;			// 回転
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

public:

	BaseCpuParticle();
	virtual ~BaseCpuParticle();

	void Finalize();

	void Init(uint32_t instanceNum, const std::string& objFile);
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList);

	virtual void MakeParticle(const Vector3& pos, uint32_t createNum) = 0;

protected:

	uint32_t kInstanceNum_;

	std::vector<Particle> particleArray_;

	std::vector<std::unique_ptr<Mesh>> meshArray_;
	std::vector<std::unique_ptr<Material>> materials_;

	ComPtr<ID3D12Resource> particleResource_;
	DescriptorHeap::DescriptorHandles srvAddress_;
	GpuData* particleData_;

	ComPtr<ID3D12Resource> perViewBuffer_;
	PerView* perView_;

	std::unordered_map<std::string, Model::ModelMaterialData> materialData_;
};

