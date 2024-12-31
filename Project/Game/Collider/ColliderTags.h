#pragma once
#include <cstdint>

namespace ColliderTags {
	enum class Player : uint32_t {
		DEFAULT		= 1 << 0,
		DAMAGE		= 1 << 1,
	};

	enum class Enemy : uint32_t {
		DEFAULT		= 1 << 0,
		ATTACK		= 1 << 1,
	};
}