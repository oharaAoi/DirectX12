#pragma once
#include <unordered_map>
#include <string>
#include "Pipeline.h"

enum class PipelineType{
	NormalPipeline,
	AddPipeline,
	TextureLessPipeline,
	ParticlePipeline,
	PBRPipeline,
	SpritePipeline,
	SkinningPipeline,
	PrimitivePipeline,

	WaterSpacePipeline,
	WaterLightingPipeline,
	NotCullingPipeline,
	AddBlendSpritePipeline,
	NormalBlendSpritePipeline
};

class GraphicsPipelines {
public:

	GraphicsPipelines();
	~GraphicsPipelines();

	void Finalize();

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="dxCompiler">コンパイラー</param>
	/// <param name="shaders">シェーダーファイルパスをまとめたクラス</param>
	void Init(ID3D12Device* device, DirectXCompiler* dxCompiler, Shader* shaders);

	/// <summary>
	/// 描画するパイプラインをリストに積む
	/// </summary>
	/// <param name="type"></param>
	/// <param name="commandList"></param>
	void SetPipeline(const PipelineType& type, ID3D12GraphicsCommandList* commandList);

private:

	std::unordered_map<PipelineType, std::unique_ptr<Pipeline>> pipelineMap_;

	// inputLayout
	InputLayout inputLayout_;

	// DXCで使う
	DirectXCompiler* dxCompiler_ = nullptr;
	// device
	ID3D12Device* device_ = nullptr;

};

