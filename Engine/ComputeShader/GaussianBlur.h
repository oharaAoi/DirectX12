#pragma once
#include "BaseCSResource.h"

class GaussianBlur :
    public BaseCSResource {

    struct GaussianBlurData {
        float blurStrength; // ぼかしの強さ
        float kernelSize;	// カーネルサイズ
        Vector2 direction;
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
    /// 水平方向のResourceをセット
    /// </summary>
    /// <param name="commandList"></param>
    void HorizontalSetResource(ID3D12GraphicsCommandList* commandList);

    /// <summary>
    /// 垂直方向のResourceをセット
    /// </summary>
    /// <param name="commandList"></param>
    void VerticalSetResource(ID3D12GraphicsCommandList* commandList);

private:
    // 横ブラーをかけるResource。これもUAV→SRVなどの手順を踏む必要がある
    ComPtr<ID3D12Resource> horizontalResource_;
    DescriptorHeap::DescriptorHandles uavHorizontalAddress_;
    DescriptorHeap::DescriptorHandles srvHorizontalAddress_;

    GaussianBlurData* data_;

};

