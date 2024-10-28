#include "PrimitivePipeline.h"

PrimitivePipeline::PrimitivePipeline() {
	rootSignature_ = std::make_unique<RootSignature>();
}

PrimitivePipeline::~PrimitivePipeline() {
}
void PrimitivePipeline::Init(ID3D12Device* device, DirectXCompiler* dxCompiler, const Shader::ShaderData& shader) {
	device_ = device;
	dxCompiler_ = dxCompiler;

	rootSignature_->Initialize(device, RootSignatureType::Primitive);
	elementDescs = inputLayout_.CreatePrimitiveInputLayout();
	ShaderCompile(shader.vsShader, shader.psShader);

	CreatePSO();
}

void PrimitivePipeline::Draw(ID3D12GraphicsCommandList* commandList) {
	commandList->SetGraphicsRootSignature(rootSignature_->GetRootSignature());
	commandList->SetPipelineState(graphicsPipelineState_.Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

void PrimitivePipeline::Finalize() {
	rootSignature_->Finalize();
	vertexShaderBlob_.Reset();
	pixelShaderBlob_.Reset();
	graphicsPipelineState_.Reset();
}

//------------------------------------------------------------------------------------------------------
// ↓Iputlayoutの生成
//------------------------------------------------------------------------------------------------------
D3D12_INPUT_LAYOUT_DESC PrimitivePipeline::CreateInputLayout(const std::vector<D3D12_INPUT_ELEMENT_DESC>& elementDesc) {
	D3D12_INPUT_LAYOUT_DESC result{};
	result.pInputElementDescs = elementDesc.data();
	result.NumElements = static_cast<UINT>(elementDesc.size());
	return result;
}

//------------------------------------------------------------------------------------------------------
// ↓shaderを読む
//------------------------------------------------------------------------------------------------------
void PrimitivePipeline::ShaderCompile(const std::string& vertexShader, const std::string& pixelShader) {
	vertexShaderBlob_ = dxCompiler_->VsShaderCompile(vertexShader);
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = dxCompiler_->PsShaderCompile(pixelShader);
	assert(pixelShaderBlob_ != nullptr);
}

//------------------------------------------------------------------------------------------------------
// ↓ブレンドの設定
//------------------------------------------------------------------------------------------------------
D3D12_BLEND_DESC PrimitivePipeline::SetBlendState() {
	D3D12_BLEND_DESC blendDesc{};
	// すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return blendDesc;
}

//------------------------------------------------------------------------------------------------------
// ↓ラスタライズの設定
//------------------------------------------------------------------------------------------------------
D3D12_RASTERIZER_DESC PrimitivePipeline::SetRasterizerState() {
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏面を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	return rasterizerDesc;
}

//------------------------------------------------------------------------------------------------------
// ↓depthの設定
//------------------------------------------------------------------------------------------------------
D3D12_DEPTH_STENCIL_DESC PrimitivePipeline::SetDepthStencilState() {
	D3D12_DEPTH_STENCIL_DESC desc{};
	// Depthの機能を有効化する
	desc.DepthEnable = true;
	// 書き込み
	desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	// 地下駆ければ描画
	desc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	return desc;
}

//------------------------------------------------------------------------------------------------------
// ↓PSOの設定
//------------------------------------------------------------------------------------------------------
void PrimitivePipeline::CreatePSO() {
	// PSOの生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
	desc.pRootSignature = rootSignature_->GetRootSignature();
	desc.InputLayout = CreateInputLayout(elementDescs);
	desc.VS = { vertexShaderBlob_->GetBufferPointer(), vertexShaderBlob_->GetBufferSize() };
	desc.PS = { pixelShaderBlob_->GetBufferPointer(), pixelShaderBlob_->GetBufferSize() };
	desc.BlendState = SetBlendState();
	desc.RasterizerState = SetRasterizerState();
	desc.DepthStencilState = SetDepthStencilState();
	desc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// 書き込むRTVの情報
	desc.NumRenderTargets = 1;
	desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ(形状)のタイプ
	desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	// どのように画面に色を打ち込むかの設定
	desc.SampleDesc.Count = 1;
	desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// 実際に生成
	HRESULT hr = device_->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&graphicsPipelineState_));
	assert(SUCCEEDED(hr));
}
