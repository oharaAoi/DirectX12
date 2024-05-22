#include "RootSignature.h"

RootSignature::RootSignature(ID3D12Device* device) {
	Initialize(device);
}

RootSignature::~RootSignature() {
}

void RootSignature::Initialize(ID3D12Device* device) {
	device_ = device;

	rootSignature_ = CreateRootSignature();
}

Microsoft::WRL::ComPtr<ID3D12RootSignature> RootSignature::CreateRootSignature() {
	HRESULT hr = S_FALSE;
	D3D12_ROOT_SIGNATURE_DESC desc{};
	desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

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
