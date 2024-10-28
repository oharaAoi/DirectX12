#include "DescriptorAllocator.h"

DescriptorAllocator::~DescriptorAllocator() {
}

DescriptorHeap::DescriptorHandles DescriptorAllocator::Allocate(ID3D12DescriptorHeap* descriptorHeap) {
	if (!freeStack_.empty()) {
		// 再利用可能なインデックスをスタックから取得
		uint32_t reusedIndex = freeStack_.top();
 		freeStack_.pop();
		/*std::string name = std::to_string(reusedIndex);
		Log("popHeap" + name + "\n");*/
		return GetDescriptorHandle(descriptorHeap, reusedIndex);
	} else if (currentIndex_ < totalDescriptors_) {
		// 新しいディスクリプタを割り当て
		return GetDescriptorHandle(descriptorHeap, currentIndex_++);
	} else {
		throw std::runtime_error("No more descriptors available.");
	}
}

void DescriptorAllocator::Free(uint32_t index) {
	freeStack_.push(index);  // フリースタックに戻す
	std::string name = std::to_string(index);
	Log("pushFreeHeap" + name + "\n");
}

DescriptorHeap::DescriptorHandles DescriptorAllocator::GetDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t index) {
	DescriptorHeap::DescriptorHandles handles;
	handles.handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handles.handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handles.assignIndex_ = index;
	// インデックスに基づきディスクリプタのオフセットを計算
	handles.handleCPU.ptr += descriptorSize_ * index;
	handles.handleGPU.ptr += descriptorSize_ * index;
	return handles;
}
