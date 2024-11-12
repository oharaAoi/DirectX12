#pragma once
#include <cstdint>

class DescriptorSize{
private:

	const uint32_t descriptorSizeSRV_;
	const uint32_t descriptorSizeRTV_;
	const uint32_t descriptorSizeDSV_;

public:

	DescriptorSize(uint32_t srvSize, uint32_t rtvSize, uint32_t dsvSize) :
		descriptorSizeSRV_(srvSize),
		descriptorSizeRTV_(rtvSize),
		descriptorSizeDSV_(dsvSize)
	{}
	
	uint32_t GetSRV() const { return descriptorSizeSRV_; }
	uint32_t GetRTV() const { return descriptorSizeRTV_; }
	uint32_t GetDSV() const { return descriptorSizeDSV_; }
	
};

