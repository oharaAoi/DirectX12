#pragma once
#include <cstdint>

namespace ColliderTags {
	namespace Player {
		enum Tag : uint32_t {
			DEFAULT = 1 << 0,
			ATTACK = 1 << 1,
			DAMAGE = 1 << 2,
		};
	}

	namespace Enemy {
		enum Tag : uint32_t {
			DEFAULT = 1 << 3,
			ATTACK = 1 << 4,
		};
	}
}