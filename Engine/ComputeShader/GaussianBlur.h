#pragma once
#include "BaseCSResource.h"

class GaussianBlur :
    public BaseCSResource {

    struct GaussianBlurData {

    };

public:

    GaussianBlur();
    ~GaussianBlur();

    void Finalize();

    /// <summary>
    /// 初期化関数
    /// </summary>
    /// <param name="device"></param>
    /// <param name="dxHeap"></param>
    void Init(ID3D12Device* device, DescriptorHeap* dxHeap) override;

    /// <summary>
    /// Resourceをコマンドにセットする
    /// </summary>
    /// <param name="commandList"></param>
    void Draw(ID3D12GraphicsCommandList* commandList);

private:
    // 横ブラーをかけるResource。これもUAV→SRVなどの手順を踏む必要がある
    ComPtr<ID3D12Resource> horizontalResource_;
    DescriptorHeap::DescriptorHandles uavHorizontalAddress_;
    DescriptorHeap::DescriptorHandles srvHorizontalAddress_;

    GaussianBlurData* data_;

};

