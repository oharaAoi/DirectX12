#include "GraphicsPipelines.h"

GraphicsPipelines::GraphicsPipelines() {
}

GraphicsPipelines::~GraphicsPipelines() {
}

void GraphicsPipelines::Finalize() {
	pipelineMap_.clear();
}

void GraphicsPipelines::Init(ID3D12Device* device, DirectXCompiler* dxCompiler, Shader* shaders) {
	assert(device);
	assert(dxCompiler);
	
	device_ = device;
	dxCompiler_ = dxCompiler;
	
	pipelineMap_[PipelineType::NormalPipeline] = std::make_unique<Pipeline>();
	pipelineMap_[PipelineType::TextureLessPipeline] = std::make_unique<Pipeline>();
	pipelineMap_[PipelineType::ParticlePipeline] = std::make_unique<Pipeline>();
	pipelineMap_[PipelineType::SpritePipeline] = std::make_unique<Pipeline>();
	pipelineMap_[PipelineType::PBRPipeline] = std::make_unique<Pipeline>();
	pipelineMap_[PipelineType::SkinningPipeline] = std::make_unique<Pipeline>();

	pipelineMap_[PipelineType::NormalPipeline]	 ->Initialize(device_, dxCompiler_, shaders->GetShaderData(Shader::Normal), RootSignatureType::Normal, inputLayout_.CreateInputLayout(), Blend::kBlendModeNormal);
	pipelineMap_[PipelineType::TextureLessPipeline]->Initialize(device_, dxCompiler_, shaders->GetShaderData(Shader::TextureLess), RootSignatureType::TextureLess, inputLayout_.CreateInputLayout(), Blend::kBlendModeNormal);
	pipelineMap_[PipelineType::ParticlePipeline]	 ->Initialize(device_, dxCompiler_, shaders->GetShaderData(Shader::Particle), RootSignatureType::Particle, inputLayout_.CreateParticleInputLayout(), Blend::kBlendModeNormal);
	pipelineMap_[PipelineType::SpritePipeline]	 ->Initialize(device_, dxCompiler_, shaders->GetShaderData(Shader::Sprite), RootSignatureType::Sprite, inputLayout_.CreateSpriteInputLayout(), Blend::kBlendModeNormal);
	pipelineMap_[PipelineType::PBRPipeline]		 ->Initialize(device_, dxCompiler_, shaders->GetShaderData(Shader::PBR), RootSignatureType::PBR, inputLayout_.CreateInputLayout(), Blend::kBlendModeNormal);
	pipelineMap_[PipelineType::SkinningPipeline]->Initialize(device_, dxCompiler_, shaders->GetShaderData(Shader::Skinning), RootSignatureType::Skinnig, inputLayout_.CreateSkinningInputLayout(), Blend::kBlendModeNormal);
	
}

void GraphicsPipelines::SetPipeline(const PipelineType& type, ID3D12GraphicsCommandList* commandList) {
	pipelineMap_[type]->Draw(commandList);
}
