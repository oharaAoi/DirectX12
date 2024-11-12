#include "Pipeline.h"

Pipeline::Pipeline() {}
Pipeline::~Pipeline() {}

void Pipeline::Initialize(ID3D12Device* device, DirectXCompiler* dxCompiler,
						  const Shader::ShaderData& shaderData, const RootSignatureType& rootSignatureType,
						  const std::vector<D3D12_INPUT_ELEMENT_DESC>& desc, const Blend::BlendMode& blendMode, 
						  bool isCulling, bool isDepth) {
	device_ = device;
	dxCompiler_ = dxCompiler;

	rootSignature_ = std::make_unique<RootSignature>();
	rootSignature_->Initialize(device, rootSignatureType);
	elementDescs = desc;
	ShaderCompile(shaderData);

	CreatePSO(blendMode, isCulling, isDepth);
}

void Pipeline::Draw(ID3D12GraphicsCommandList* commandList) {
	commandList->SetGraphicsRootSignature(rootSignature_->GetRootSignature());
	commandList->SetPipelineState(graphicsPipelineState_.Get());
}

void Pipeline::Finalize() {
	rootSignature_->Finalize();
	vertexShaderBlob_.Reset();
	pixelShaderBlob_.Reset();
	graphicsPipelineState_.Reset();
}

//------------------------------------------------------------------------------------------------------
// ↓Iputlayoutの生成
//------------------------------------------------------------------------------------------------------
D3D12_INPUT_LAYOUT_DESC Pipeline::CreateInputLayout(const std::vector<D3D12_INPUT_ELEMENT_DESC>& elementDesc) {
	D3D12_INPUT_LAYOUT_DESC result{};
	result.pInputElementDescs = elementDesc.data();
	result.NumElements = static_cast<UINT>(elementDesc.size());
	return result;
}

//------------------------------------------------------------------------------------------------------
// ↓shaderを読む
//------------------------------------------------------------------------------------------------------
void Pipeline::ShaderCompile(const Shader::ShaderData& shaderData) {
	vertexShaderBlob_ = dxCompiler_->VsShaderCompile(shaderData.vsShader);
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = dxCompiler_->PsShaderCompile(shaderData.psShader);
	assert(pixelShaderBlob_ != nullptr);
}

//------------------------------------------------------------------------------------------------------
// ↓ラスタライズの設定
//------------------------------------------------------------------------------------------------------
D3D12_RASTERIZER_DESC Pipeline::SetRasterizerState(bool isCulling) {
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏面を表示しない
	if (isCulling) {
		rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	} else {
		rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	}
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	return rasterizerDesc;
}

/// <summary>
/// DepthStencilStateの設定
/// </summary>
/// <returns></returns>
D3D12_DEPTH_STENCIL_DESC Pipeline::SetDepthStencilState(bool isDepth) {
	D3D12_DEPTH_STENCIL_DESC desc{};
	// Depthの機能を有効化する
	desc.DepthEnable = true;
	// 書き込み
	//desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	if(isDepth){
		desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	} else{
		desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	}
	
	// 地下駆ければ描画
	desc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	return desc;
}

//------------------------------------------------------------------------------------------------------
// ↓PSOの追加
//------------------------------------------------------------------------------------------------------
void Pipeline::CreatePSO(const Blend::BlendMode& blendMode, bool isCulling, bool isDepth) {
	// PSOの生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
	desc.pRootSignature = rootSignature_->GetRootSignature();
	desc.InputLayout = CreateInputLayout(elementDescs);
	desc.VS = { vertexShaderBlob_->GetBufferPointer(), vertexShaderBlob_->GetBufferSize() };
	desc.PS = { pixelShaderBlob_->GetBufferPointer(), pixelShaderBlob_->GetBufferSize() };
	desc.BlendState = blend_.SetBlend(blendMode);
	desc.RasterizerState = SetRasterizerState(isCulling);
	desc.DepthStencilState = SetDepthStencilState(isDepth);
	desc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// 書き込むRTVの情報
	desc.NumRenderTargets = renderTargetNum_;
	for (uint32_t oi = 0; oi < renderTargetNum_; ++oi) {
		desc.RTVFormats[oi] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	}
	// 利用するトポロジ(形状)のタイプ
	desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定
	desc.SampleDesc.Count = 1;
	desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// 実際に生成
	HRESULT hr = device_->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&graphicsPipelineState_));
	assert(SUCCEEDED(hr));
}
