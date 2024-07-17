#pragma once
#include "BaseCSResource.h"

class GrayScale :
    public BaseCSResource {

    struct GrayScaleData {
        float grayScaleAmount;// grayScaleの強さ
    };

public:

    GrayScale();
    ~GrayScale();

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
    void Draw(ID3D12GraphicsCommandList* commandList) override;

private:

    GrayScaleData* data_ = nullptr;

};

