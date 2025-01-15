#pragma once
#include <string>
#include <unordered_map>
#include "Engine/Assets/Meshes/Mesh.h"
#include "Engine/Assets/Materials/Material.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Vector4.h"
#include "Engine/Math/Matrix4x4.h"
#include "Engine/Utilities/DirectXUtils.h"
#include "Engine/DirectX/Resource/ShaderResource.h"

class GpuParticles {
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

	GpuParticles();
	~GpuParticles();

	void Finalize();

	void Init(uint32_t instanceNum);
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList);

	void InitBindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex);

	void EmitBindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex);

	void SetViewProjection(const Matrix4x4& viewProjection);
	
private:

	uint32_t kInstanceNum_;

	std::unique_ptr<ShaderResource> particleResource_;
	std::unique_ptr<ShaderResource> freeListIndexResource_;
	std::unique_ptr<ShaderResource> freeListResource_;

	ComPtr<ID3D12Resource> perViewBuffer_;
	PerView* perView_;

	ComPtr<ID3D12Resource> perFrameBuffer_;
	PerFrame* perFrame_;

	std::vector<std::unique_ptr<Mesh>> meshArray_;
	std::unordered_map<std::string, std::unique_ptr<Material>> materialArray_;
};

