#include <Windows.h>
#include <cstdint>
#include <d3d12.h>
#include <wrl.h>
#include <string>
// DirectX
#include "DirectXUtils.h"
// math
#include "MyMath.h"

template<typename T>
using Comptr = Microsoft::WRL::ComPtr <T>;

class Mesh final {
public: // 構造体

	struct VertexData {
		Vector4 pos;
	};

	struct Vertices {
		Vector4 vertex1;// 左下
		Vector4 vertex2;// 上
		Vector4 vertex3;// 右下
	};

public:

	Mesh();
	~Mesh();

	void Init(ID3D12Device* device, const uint32_t& vBSize);

	void Draw(ID3D12GraphicsCommandList* commandList);

	void Finalize();

public:

	ID3D12Resource* GetVertexBuffer() { return vertexBuffer_.Get(); }

	VertexData* GetVertexData() { return vertexData_; }

private:

	Comptr<ID3D12Resource> vertexBuffer_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};
	VertexData* vertexData_ = nullptr;
	
};