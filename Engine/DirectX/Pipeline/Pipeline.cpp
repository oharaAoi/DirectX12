#include "Pipeline.h"

Pipeline::Pipeline(ID3D12Device* device) {
	Initialize(device);
}

Pipeline::~Pipeline() {
}

void Pipeline::Initialize(ID3D12Device* device) {
	device_ = device;

	InitializeDXC();

	rootSignature_ = std::make_unique<RootSignature>(device_);

	ShaderCompile("Engine/HLSL/Object3d.VS.hlsl", "Engine/HLSL/Object3d.PS.hlsl");

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

D3D12_INPUT_LAYOUT_DESC Pipeline::CreateInputLayout(const std::vector<D3D12_INPUT_ELEMENT_DESC>& elementDesc) {
	D3D12_INPUT_LAYOUT_DESC result{};
	result.pInputElementDescs = elementDesc.data();
	result.NumElements = static_cast<UINT>(elementDesc.size());
	return result;
}

void Pipeline::ShaderCompile(const std::string& vertexShader, const std::string& pixelShader) {
	vertexShaderBlob_ = CompilerShader(ConvertWString(vertexShader), L"vs_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = CompilerShader(ConvertWString(pixelShader), L"ps_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(pixelShaderBlob_ != nullptr);
}

D3D12_BLEND_DESC Pipeline::SetBlendState() {
	D3D12_BLEND_DESC blendDesc{};
	// すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return blendDesc;
}

D3D12_RASTERIZER_DESC Pipeline::SetRasterizerState() {
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏面を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	return rasterizerDesc;
}

void Pipeline::CreatePSO() {
	// InputLayoutの生成
	std::vector<D3D12_INPUT_ELEMENT_DESC> elementDescs = {};
	D3D12_INPUT_ELEMENT_DESC elementDesc{};
	elementDesc.SemanticName = "POSITION";
	elementDesc.SemanticIndex = 0;
	elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
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
