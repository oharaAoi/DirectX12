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
    void SetResource(ID3D12GraphicsCommandList* commandList);

    void SetResultResource(ID3D12GraphicsCommandList* commandList);

    const DescriptorHeap::DescriptorHandles GetSRVHandle() const {return uavBuffers_[0].srvAddress;}

    void TransitionResource(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES beforState, D3D12_RESOURCE_STATES afterState);

private:

    GrayScaleData* data_ = nullptr;

};

