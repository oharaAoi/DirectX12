#include "Easing.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////


float Ease::In::Sine(float t) {
	return 1.0f - cosf((t * static_cast<float>(M_PI)) / 2.0f);
}

float Ease::In::Cubic(float t) {
	return t * t * t;
}

float Ease::In::Quad(float t) {
	return t * t;
}

float Ease::In::Quart(float t) {
	return t * t * t * t;
}

float Ease::In::Quint(float t) {
	return t * t * t * t * t;
}

float Ease::In::Expo(float t) {
	return t == 0.0f
		? 0.0f
		: powf(2.0f, 10.0f * t - 10.0f);
}

float Ease::In::Circ(float t) {
	return 1.0f - sqrtf(1.0f - powf(t, 2.0f));
}

float Ease::In::Back(float t) {
	const float c1 = 1.70158f;
	const float c2 = c1 + 1.0f;
	return c2 * t * t * t - c1 * t * t;
}

float Ease::In::Elastic(float t) {
	const float c4 = (2.0f * static_cast<float>(M_PI)) / 3.0f;
	return t == 0.0f
		? 0.0f
		: t == 1.0f
		? 1.0f
		: -powf(2.0f, 10.0f * t - 10.0f) * sinf((t * 10.0f - 10.75f) * c4);
}

float Ease::In::Bounce(float t) {
	const float n1 = 7.5625f;
	const float d1 = 2.75f;
	float t2 = 0.0f;
	float t3 = 0.0f;

	t3 = (1.0f - t);

	if (t3 < 1.0f / d1) {
		return 1.0f - (n1 * t3 * t3);

	} else if (t3 < 2.0f / d1) {
		t2 = t3 - 1.5f / d1;
		return 1.0f - (n1 * t2 * t2 + 0.75f);

	} else if (t3 < 2.5f / d1) {
		t2 = t3 - 2.25f / d1;
		return 1.0f - (n1 * t2 * t2 + 0.9375f);

	} else {
		t2 = t3 - 2.625f / d1;
		return 1.0f - (n1 * t2 * t2 + 0.984375f);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

float Ease::Out::Sine(float t) {
	return sinf((t * static_cast<float>(M_PI)) / 2.0f);
}

float Ease::Out::Cubic(float t) {
	return 1.0f - powf(1.0f - t, 3.0f);
}

float Ease::Out::Quad(float t) {
	return 1.0f - (1.0f - t) * (1.0f - t);
}

float Ease::Out::Quart(float t) {
	return 1.0f - powf(1.0f - t, 4.0f);
}

float Ease::Out::Quint(float t) {
	return t == 1.0f
		? 1.0f
		: 1.0f - powf(2.0f, -10.0f * t);
}

float Ease::Out::Expo(float t) {
	return t == 1.0f
		? 1.0f
		: 1.0f - powf(2.0f, -10.0f * t);
}

float Ease::Out::Circ(float t) {
	return sqrtf(1.0f - powf(t - 1.0f, 2.0f));
}

float Ease::Out::Back(float t) {
	const float c1 = 1.70158f;
	const float c2 = c1 + 1.0f;
	return 1.0f + c2 * powf(t - 1.0f, 3.0f) + c1 * powf(t - 1.0f, 2.0f);
}

float Ease::Out::Elastic(float t) {
	const float c4 = (2.0f * static_cast<float>(M_PI)) / 3.0f;
	return t == 0.0f
		? 0.0f
		: t == 1.0f
		? 1.0f
		: powf(2.0f, -10.0f * t) * sinf((t * 10.0f - 0.75f) * c4) + 1.0f;
}

float Ease::Out::Bounce(float t) {
	const float n1 = 7.5625f;
	const float d1 = 2.75f;
	float t2 = 0.0f;

	if (t < 1.0f / d1) {
		return n1 * t * t;

	} else if (t < 2.0f / d1) {
		t2 = t - 1.5f / d1;
		return n1 * t2 * t2 + 0.75f;

	} else if (t < 2.5f / d1) {
		t2 = t - 2.25f / d1;
		return n1 * t2 * t2 + 0.9375f;

	} else {
		t2 = t - 2.625f / d1;
		return n1 * t2 * t2 + 0.984375f;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

float Ease::InOut::Sine(float t) {
	return -(cosf(static_cast<float>(M_PI) * t) - 1.0f) / 2.0f;
}

float Ease::InOut::Cubic(float t) {
	return t < 0.5f
		? 4.0f * t * t * t
		: 1.0f - powf(-2.0f * t + 2.0f, 3.0f) / 2.0f;
}

float Ease::InOut::Quad(float t) {
	return t < 0.5f
		? 2.0f * t * t
		: 1.0f - powf(-2.0f * t + 2.0f, 2.0f) / 2.0f;
}

float Ease::InOut::Quart(float t) {
	return t < 0.5f
		? 8.0f * t * t * t * t
		: 1.0f - powf(-2.0f * t + 2.0f, 4.0f) / 2.0f;
}

float Ease::InOut::Quint(float t) {
	return t < 0.5f
		? 16.0f * t * t * t * t * t
		: 1.0f - powf(-2.0f * t + 2.0f, 5.0f) / 2.0f;
}

float Ease::InOut::Expo(float t) {
	return t == 0.0f
		? 0.0f
		: t == 1.0f
		? 1.0f
		: t < 0.5f
		? powf(2.0f, 20.0f * t - 10.0f) / 2.0f
		: (2.0f - powf(2.0f, -20.0f * t + 10.0f)) / 2.0f;
}

float Ease::InOut::Circ(float t) {
	return t < 0.5f
		? (1.0f - sqrtf(1.0f - powf(2.0f * t, 2.0f))) / 2.0f
		: (sqrtf(1.0f - powf(-2.0f * t + 2.0f, 2.0f)) + 1.0f) / 2.0f;
}

float Ease::InOut::Back(float t) {
	const float c1 = 1.70158f;
	const float c3 = c1 * 1.525f;
	return t < 0.5f
		? (powf(2.0f * t, 2.0f) * ((c3 + 1) * 2.0f * t - c3)) / 2.0f
		: (powf(2.0f * t - 2.0f, 2.0f) * ((c3 + 1.0f) * (t * 2.0f - 2.0f) + c3) + 2.0f) / 2.0f;
}

float Ease::InOut::Elastic(float t) {
	const float c5 = (2.0f * static_cast<float>(M_PI)) / 4.5f;
	return t == 0.0f
		? 0.0f
		: t == 1.0f
		? 1.0f
		: t < 0.5f
		? -(powf(2.0f, 20.0f * t - 10.0f) * sinf((20.0f * t - 11.125f) * c5)) / 2.0f
		: (powf(2.0f, -20.0f * t + 10.0f) * sinf((20.0f * t - 11.125f) * c5)) / 2.0f + 1.0f;
}

float Ease::InOut::Bounce(float t) {
	const float n1 = 7.5625f;
	const float d1 = 2.75f;
	float t2 = 0.0f;
	float t3 = 0.0f;

	if (t < 0.5) {
		t3 = (1.0f - t * 2.0f);

		if (t3 < 1.0f / d1) {
			return (1.0f - (n1 * t3 * t3)) / 2.0f;

		} else if (t3 < 2.0f / d1) {
			t2 = t3 - 1.5f / d1;
			return (1.0f - (n1 * t2 * t2 + 0.75f)) / 2.0f;

		} else if (t3 < 2.5f / d1) {
			t2 = t3 - 2.25f / d1;
			return (1.0f - (n1 * t2 * t2 + 0.9375f)) / 2.0f;

		} else {
			t2 = t3 - 2.625f / d1;
			return (1.0f - (n1 * t2 * t2 + 0.984375f)) / 2.0f;
		}

	} else {
		t3 = (2.0f * t - 1.0f);

		if (t3 < 1.0f / d1) {
			return (1.0f + (n1 * t3 * t3)) / 2.0f;

		} else if (t3 < 2.0f / d1) {
			t2 = t3 - 1.5f / d1;
			return (1.0f + (n1 * t2 * t2 + 0.75f)) / 2.0f;

		} else if (t3 < 2.5f / d1) {
			t2 = t3 - 2.25f / d1;
			return (1.0f + (n1 * t2 * t2 + 0.9375f)) / 2.0f;

		} else {
			t2 = t3 - 2.625f / d1;
			return (1.0f + (n1 * t2 * t2 + 0.984375f)) / 2.0f;
		}
	}
}

float CallEasingFunc(int index, float t) {
	if (index >= 0 && index < easingFuncs.size()) {
		return easingFuncs[index](t);  // 指定の関数を呼び出し
	} else {
		std::cerr << "Invalid index\n";
		return 0.0f;  // 無効なインデックスの場合
	}
}

#ifdef _DEBUG
void ShowEasingDebug(int& easeKind) {
	// 引数から種類を取り出す
	int easeType = (int)(easeKind % 10);    // 0 ~ 9
	int inOrOut = (int)(easeKind / 10);     // 0 ~ 2

	ImGui::Text("Ease::");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(60);
	ImGui::Combo("::", &inOrOut, "In\0Out\0InOut\0");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(60);
	ImGui::Combo("##type", &easeType, "Sine\0Cubic\0Quad\0Quart\0Quint\0Expo\0Cric\0Back\0Elastic\0Bounce\0");

	int result = 0;
	if (inOrOut == 0) {
		result = 0;
	} else if (inOrOut == 1) {
		result = 10;
	} else if (inOrOut == 2) {
		result = 20;
	}

	result += easeType;
	easeKind = result;

	ImGui::SameLine();
	ImGui::Text("number : %d", result);
}
#endif