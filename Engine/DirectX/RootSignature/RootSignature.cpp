#include "RootSignature.h"

RootSignature::RootSignature(ID3D12Device* device, const RootSignatureType& type) {
	Initialize(device, type);
}

RootSignature::~RootSignature() {
}

void RootSignature::Initialize(ID3D12Device* device, const RootSignatureType& type) {
	device_ = device;

	switch (type) {
	case RootSignatureType::Normal:
		rootSignature_ = CreateRootSignature();
		break;

	case RootSignatureType::TextureLess:
		rootSignature_ = CreateTexturelessRootSignature();
		break;

	case RootSignatureType::Primitive:
		rootSignature_ = CreatePrimitiveRootSignature();
		break;
	}
}

void RootSignature::Finalize() {
	rootSignature_.Reset();
}

//////////////////////////////////////////////////////////////////////////////////////
// 基本のRootSignature
//////////////////////////////////////////////////////////////////////////////////////
Microsoft::WRL::ComPtr<ID3D12RootSignature> RootSignature::CreateRootSignature() {
	HRESULT hr = S_FALSE;
	D3D12_ROOT_SIGNATURE_DESC desc{};
	desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// 複数のtextureを読むための
	D3D12_DESCRIPTOR_RANGE descriptorRange[1]{};
	descriptorRange[0].BaseShaderRegister = 0;
	descriptorRange[0].NumDescriptors = 1;
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// rootparameter
	D3D12_ROOT_PARAMETER rootParameters[4] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	// PixelShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;					// レジスタ番号とバインド

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;// VertexShaderで使う
	rootParameters[1].Descriptor.ShaderRegister = 0;					// レジスタ番号とバインド

	// light用
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// CBVを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;// VertexShaderで使う
	rootParameters[2].Descriptor.ShaderRegister = 1;					// レジスタ番号とバインド

	// DescriptorTable(textureに使う)
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		// DescriptorTableを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;					// PixelShaderで使う
	rootParameters[3].DescriptorTable.pDescriptorRanges = descriptorRange;				// Tableの中身の配列を指定
	rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);	// Tableで利用する数

	desc.pParameters = rootParameters;
	desc.NumParameters = _countof(rootParameters);

	// Samplerの設定(Textureで使っている) -------------------------------------------
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	staticSamplers[0].ShaderRegister = 0;
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	desc.pStaticSamplers = staticSamplers;
	desc.NumStaticSamplers = _countof(staticSamplers);

	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	Comptr<ID3DBlob> signatureBlob = nullptr;
	Comptr<ID3DBlob> errorBlob = nullptr;
	// シリアライズしてバイナリにする
	hr = D3D12SerializeRootSignature(&desc,
									 D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	// バイナリを元に生成
	hr = device_->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));

	return rootSignature.Get();
}

//////////////////////////////////////////////////////////////////////////////////////
// textureがないRootSignature
//////////////////////////////////////////////////////////////////////////////////////
Microsoft::WRL::ComPtr<ID3D12RootSignature> RootSignature::CreateTexturelessRootSignature() {
	HRESULT hr = S_FALSE;
	D3D12_ROOT_SIGNATURE_DESC desc{};
	desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// rootParameterの作成
	D3D12_ROOT_PARAMETER rootParameters[3] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	// PixelShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;					// レジスタ番号とバインド

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].Descriptor.ShaderRegister = 0;

	// Light
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[2].Descriptor.ShaderRegister = 1;

	desc.pParameters = rootParameters;
	desc.NumParameters = _countof(rootParameters);

	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	Comptr<ID3DBlob> signatureBlob = nullptr;
	Comptr<ID3DBlob> errorBlob = nullptr;
	// シリアライズしてバイナリにする
	hr = D3D12SerializeRootSignature(&desc,
									 D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	// バイナリを元に生成
	hr = device_->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));

	return rootSignature.Get();
}

//////////////////////////////////////////////////////////////////////////////////////
// primitiveRootSignatureの作成
//////////////////////////////////////////////////////////////////////////////////////
Microsoft::WRL::ComPtr<ID3D12RootSignature> RootSignature::CreatePrimitiveRootSignature() {
	HRESULT hr = S_FALSE;
	D3D12_ROOT_SIGNATURE_DESC desc{};
	desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// rootParameterの作成
	D3D12_ROOT_PARAMETER rootParameters[2] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	// PixelShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;					// レジスタ番号とバインド

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].Descriptor.ShaderRegister = 0;

	desc.pParameters = rootParameters;
	desc.NumParameters = _countof(rootParameters);

	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	Comptr<ID3DBlob> signatureBlob = nullptr;
	Comptr<ID3DBlob> errorBlob = nullptr;
	// シリアライズしてバイナリにする
	hr = D3D12SerializeRootSignature(&desc,
									 D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	// バイナリを元に生成
	hr = device_->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));

	return rootSignature.Get();
}
