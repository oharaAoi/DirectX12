#pragma once
#include <array>
#include <string>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
#endif

using EasingFunc = float(*)(float);

namespace Ease {
	namespace In {
		float Sine(float t);
		float Cubic(float t);
		float Quad(float t);
		float Quart(float t);
		float Quint(float t);
		float Expo(float t);
		float Circ(float t);
		float Back(float t);
		float Elastic(float t);
		float Bounce(float t);
	}

	namespace Out {
		float Sine(float t);
		float Cubic(float t);
		float Quad(float t);
		float Quart(float t);
		float Quint(float t);
		float Expo(float t);
		float Circ(float t);
		float Back(float t);
		float Elastic(float t);
		float Bounce(float t);
	}

	namespace InOut {
		float Sine(float t);
		float Cubic(float t);
		float Quad(float t);
		float Quart(float t);
		float Quint(float t);
		float Expo(float t);
		float Circ(float t);
		float Back(float t);
		float Elastic(float t);
		float Bounce(float t);
	}
}

namespace EasingType {
	enum class In {
		Sine,
		Cubic,
		Quad,
		Quart,
		Quint,
		Expo,
		Circ,
		Back,
		Elastic,
		Bounce,
	};

	enum class Out {
		Sine = 10,
		Cubic,
		Quad,
		Quart,
		Quint,
		Expo,
		Circ,
		Back,
		Elastic,
		Bounce,
	};

	enum class InOut {
		Sine = 20,
		Cubic,
		Quad,
		Quart,
		Quint,
		Expo,
		Circ,
		Back,
		Elastic,
		Bounce,
	};
}

/// <summary>
/// Easing関数をまとめた配列
/// </summary>
const std::array<EasingFunc, 30> easingFuncs = {
	// ------------------- In ------------------- //
	Ease::In::Sine, Ease::In::Cubic, Ease::In::Quad,
	Ease::In::Quart, Ease::In::Quint, Ease::In::Expo,
	Ease::In::Circ, Ease::In::Back, Ease::In::Elastic, Ease::In::Bounce,

	// ------------------- Out ------------------- //
	Ease::Out::Sine, Ease::Out::Cubic, Ease::Out::Quad,
	Ease::Out::Quart, Ease::Out::Quint, Ease::Out::Expo,
	Ease::Out::Circ, Ease::Out::Back, Ease::Out::Elastic, Ease::Out::Bounce,

	// ------------------- InOut ------------------- //
	Ease::InOut::Sine, Ease::InOut::Cubic, Ease::InOut::Quad,
	Ease::InOut::Quart, Ease::InOut::Quint, Ease::InOut::Expo,
	Ease::InOut::Circ, Ease::InOut::Back, Ease::InOut::Elastic, Ease::InOut::Bounce,

};

/// <summary> 
/// Easingを関数を呼び出す
/// </summary> 
/// <param name="index">: EasingType</param> 
/// <param name="t">: 線形代数</param> 
/// <returns> Easingした結果の線形代数 </returns> 
float CallEasingFunc(int index, float t);

#ifdef _DEBUG
void ShowEasingDebug(int& easeKind);
#endif