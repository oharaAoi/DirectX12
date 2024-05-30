#include "Pipeline.h"

Pipeline::Pipeline(ID3D12Device* device, const PipelineType& type) {
	Initialize(device, type);
}

Pipeline::~Pipeline() {
}

void Pipeline::Initialize(ID3D12Device* device, const PipelineType& type) {
	device_ = device;

	InitializeDXC();

	switch (type) {
	case NormalPipeline:
		rootSignature_ = std::make_unique<RootSignature>(device_, RootSignatureType::Normal);
		ShaderCompile("Engine/HLSL/Object3d.VS.hlsl", "Engine/HLSL/Object3d.PS.hlsl");
		break;

	case TextureLessPipeline:
		rootSignature_ = std::make_unique<RootSignature>(device_, RootSignatureType::TextureLess);
		ShaderCompile("Engine/HLSL/Object3d.VS.hlsl", "Engine/HLSL/Textureless.PS.hlsl");
		break;
	}

	CreatePSO();
}

void Pipeline::Draw(ID3D12GraphicsCommandList* commandList) {
	commandList->SetGraphicsRootSignature(rootSignature_->GetRootSignature());
	commandList->SetPipelineState(graphicsPipelineState_.Get());
}

void Pipeline::Finalize() {
	dxcUtils_.Reset();
	dxcCompiler_.Reset();
	includeHandler_.Reset();
	rootSignature_->Finalize();
	vertexShaderBlob_.Reset();
	pixelShaderBlob_.Reset();
	graphicsPipelineState_.Reset();
}

//------------------------------------------------------------------------------------------------------
// ↓HLSLからDXILに変換するCompilerの初期化
//------------------------------------------------------------------------------------------------------
void Pipeline::InitializeDXC() {
	HRESULT hr = S_FALSE;
	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));

	// includeに対応するための設定
	hr = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
	assert(SUCCEEDED(hr));
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
void Pipeline::ShaderCompile(const std::string& vertexShader, const std::string& pixelShader) {
	vertexShaderBlob_ = CompilerShader(ConvertWString(vertexShader), L"vs_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = CompilerShader(ConvertWString(pixelShader), L"ps_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(pixelShaderBlob_ != nullptr);
}

//------------------------------------------------------------------------------------------------------
// ↓ブレンドの設定
//------------------------------------------------------------------------------------------------------
D3D12_BLEND_DESC Pipeline::SetBlendState() {
	D3D12_BLEND_DESC blendDesc{};
	// すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return blendDesc;
}

//------------------------------------------------------------------------------------------------------
// ↓ラスタライズの設定
//------------------------------------------------------------------------------------------------------
D3D12_RASTERIZER_DESC Pipeline::SetRasterizerState() {
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏面を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	return rasterizerDesc;
}

/// <summary>
/// DepthStencilStateの設定
/// </summary>
/// <returns></returns>
D3D12_DEPTH_STENCIL_DESC Pipeline::SetDepthStencilState() {
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
// ↓PSOの追加
//------------------------------------------------------------------------------------------------------
void Pipeline::CreatePSO() {
	// InputLayoutの生成
	std::vector<D3D12_INPUT_ELEMENT_DESC> elementDescs = {};
	D3D12_INPUT_ELEMENT_DESC elementDesc{};
	elementDesc.SemanticName = "POSITION";
	elementDesc.SemanticIndex = 0;
	elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	elementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	elementDescs.push_back(elementDesc);

	// texture
	elementDesc.SemanticName = "TEXCOORD";
	elementDesc.SemanticIndex = 0;
	elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
	elementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	elementDescs.push_back(elementDesc);

	// 法線
	elementDesc.SemanticName = "NORMAL";
	elementDesc.SemanticIndex = 0;
	elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	elementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	elementDescs.push_back(elementDesc);

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
	desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定
	desc.SampleDesc.Count = 1;
	desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// 実際に生成
	HRESULT hr = device_->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&graphicsPipelineState_));
	assert(SUCCEEDED(hr));
}
