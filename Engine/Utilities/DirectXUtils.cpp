#include "DirectXUtils.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxcompiler.lib")

ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(Microsoft::WRL::ComPtr<ID3D12Device> device,
	const D3D12_DESCRIPTOR_HEAP_TYPE& heapType,
	const UINT& numDescriptor,
	const bool& shaderVisible){

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Type = heapType;
	desc.NumDescriptors = numDescriptor;
	desc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	HRESULT hr = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptorHeap));
	assert(SUCCEEDED(hr));

	return descriptorHeap;
}

ComPtr<ID3D12Resource> CreateBufferResource(Microsoft::WRL::ComPtr<ID3D12Device> device, const size_t& sizeInBytes) {
	HRESULT hr = S_FALSE;
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	// 頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc = {};
	// バッファリソース。テクスチャの場合はまた別の設定をする
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeInBytes;
	// バッファの場合がこれらは1にする決まり
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	vertexResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	// バッファの場合はこれにする決まり
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 実際に頂点リソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = nullptr;
	hr = device->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexResource));
	assert(SUCCEEDED(hr));

	return vertexResource;
}

/// <summary>
/// 深度情報を格納するリソースの生成
/// </summary>
/// <param name="device"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <returns></returns>
ComPtr<ID3D12Resource> CreateDepthStencilTextureResource(ComPtr<ID3D12Device> device, const int32_t& width, const int32_t& height){
	// 生成するResourceの設定
	D3D12_RESOURCE_DESC desc{};
	desc.Width = width;										// textureの幅
	desc.Height = height;									// textureの高さ
	desc.MipLevels = 1;										// mipmapの数
	desc.DepthOrArraySize = 1;								// 奥行or配列textureの配列数
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;			// DepthStencilとして利用可能なフォーマット
	desc.SampleDesc.Count = 1;								// サンプリングカウント
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;	// 2次元
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;	// DepthStencilとして使う通知

	// 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	// 深度地のクリア設定
	D3D12_CLEAR_VALUE value{};
	value.DepthStencil.Depth = 1.0f;				// 最大値(1.0)でクリア
	value.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	// フォーマット。Resourceと合わせる

	ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,					// Heapの設定
		D3D12_HEAP_FLAG_NONE,				// Heapの特殊な設定。
		&desc,								// Resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	// 深度値を書き込む状態にしておく
		&value,								// Clear最適地
		IID_PPV_ARGS(&resource)				// 作成するResourceポインタへのポインタ
	);

	assert(SUCCEEDED(hr));

	return resource;
}

ComPtr<ID3D12Resource> CerateShaderResource(ComPtr<ID3D12Device> device, 
						D3D12_RESOURCE_DESC* resourceDesc, D3D12_HEAP_PROPERTIES* heapProperties,
						const D3D12_HEAP_FLAGS& heapFlags, const D3D12_RESOURCE_STATES& resourceState) {
	
	ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		heapProperties,					// Heapの設定
		heapFlags,				// Heapの特殊の設定
		resourceDesc,								// Resourceの設定
		resourceState,		// 初回のResourceState Textureは木基本読むだけ
		nullptr,							// 初回のResourceState Textureは木基本読むだけ
		IID_PPV_ARGS(&resource)				// 作成するResourceポインタへのポインタ
	);
	assert(SUCCEEDED(hr));

	return resource;
}

/// <summary>
/// CompileShader
/// </summary>
/// <param name=""></param>
ComPtr<IDxcBlob> CompilerShader(
	const std::wstring& filePath,
	const wchar_t* entryPoint,
	const wchar_t* profile,
	ComPtr<IDxcUtils> dxcUtils,
	ComPtr<IDxcCompiler3> dxcCompiler,
	ComPtr<IDxcIncludeHandler> includeHandler) {

	// 1.-----------------------------------------------------------------------------------------
	// これからシェーダーをコンパイルする旨えおログに出す
	Log(ConvertString(std::format(L"Begin compileShader, path:{}\n", filePath, profile)));
	// hlslファイルを読む
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	// 読めなかったら止める
	assert(SUCCEEDED(hr));
	DxcBuffer shaderSourceBuffer{};
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	// 2.-----------------------------------------------------------------------------------------
	LPCWSTR arguments[] = {
		filePath.c_str(),			// コンパイル対象のhlslファイル
		L"-E", entryPoint,				// エントリーポイントの指定。基本的にmian以外にしない
		L"-T", profile,				// shaderProfileの設定
		L"-Zi", L"-Qembed_debug",	// デバック用に情報を埋め込む
		L"-Od",						// 最適化を外して置く
		L"-Zpr",					// メモリレイアウトは行優先
	};
	// 実際にshaderをコンパイルする
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler->Compile(
		&shaderSourceBuffer,		// 読み込んだファイル
		arguments,					// コンパイルオプション
		_countof(arguments),		// コンパイルオプションの数
		includeHandler.Get(),				// includeが含まれた諸々
		IID_PPV_ARGS(&shaderResult)	// コンパイル結果
	);
	// コンパイルエラーではなくdxcが起動できないなど致命的な状況
	assert(SUCCEEDED(hr));

	// 3.-----------------------------------------------------------------------------------------
	// 警告,エラーが出たらログに出して止める
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		Log(shaderError->GetStringPointer());
		assert(false);
	}

	// 4.-----------------------------------------------------------------------------------------
	// コンパイル結果から実行用のバイナリ部分を取得
	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	// 成功したらログを出す
	Log(ConvertString(std::format(L"Compile Succeeded, path:{}\n", filePath, profile)));
	// もう使わないリソースを解放
	shaderSource->Release();
	shaderResult->Release();

	return shaderBlob;
}

void TransitionResourceState(ID3D12GraphicsCommandList* commandList, ID3D12Resource* resource, D3D12_RESOURCE_STATES beforState, D3D12_RESOURCE_STATES afterState) {
	D3D12_RESOURCE_BARRIER barrier;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

	barrier.Transition.pResource = resource;
	// 遷移前のリソース
	barrier.Transition.StateBefore = beforState;
	// 遷移後のResourceState
	barrier.Transition.StateAfter = afterState;
	// サブリソースのインデックス
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	// 張る
	commandList->ResourceBarrier(1, &barrier);
}

D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index){
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index){
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}

void Log(const std::string& message) {
	OutputDebugStringA(message.c_str());
}