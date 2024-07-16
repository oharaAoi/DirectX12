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
	
	pipelineMap_[NormalPipeline] = std::make_unique<Pipeline>();
	pipelineMap_[TextureLessPipeline] = std::make_unique<Pipeline>();
	pipelineMap_[ParticlePipeline] = std::make_unique<Pipeline>();
	pipelineMap_[SpritePipeline] = std::make_unique<Pipeline>();
	pipelineMap_[PBRPipeline] = std::make_unique<Pipeline>();

	pipelineMap_[NormalPipeline]	 ->Initialize(device_, dxCompiler_, shaders->GetShaderData(Shader::Normal), RootSignatureType::Normal, inputLayout_.CreateInputLayout());
	pipelineMap_[TextureLessPipeline]->Initialize(device_, dxCompiler_, shaders->GetShaderData(Shader::TextureLess), RootSignatureType::TextureLess, inputLayout_.CreateInputLayout());
	pipelineMap_[ParticlePipeline]	 ->Initialize(device_, dxCompiler_, shaders->GetShaderData(Shader::Particle), RootSignatureType::Particle, inputLayout_.CreateParticleInputLayout());
	pipelineMap_[SpritePipeline]	 ->Initialize(device_, dxCompiler_, shaders->GetShaderData(Shader::Sprite), RootSignatureType::Sprite, inputLayout_.CreateSpriteInputLayout());
	pipelineMap_[PBRPipeline]		 ->Initialize(device_, dxCompiler_, shaders->GetShaderData(Shader::PBR), RootSignatureType::Normal, inputLayout_.CreateInputLayout());
}

void GraphicsPipelines::SetPipeline(const PipelineType& type, ID3D12GraphicsCommandList* commandList) {
	pipelineMap_[type]->Draw(commandList);
}
