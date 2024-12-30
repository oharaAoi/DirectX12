#pragma once
#define DIRECTINPUT_VERSION 0x0800
#define NOMINMAX
#include <dinput.h>
#include <wrl.h>
#include <memory>
#include <cassert>
#include <Xinput.h>
#include <limits>
#include "Engine/Math/Vector2.h"
#include "Engine/Math/MyMath.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "Xinput.lib")

enum XInputButtons{
	DPAD_UP = 0x0001,
	DPAD_DOWN = 0x0002,
	DPAD_LEFT = 0x0004,
	DPAD_RIGHT = 0x0008,
	START = 0x0010,
	BACK = 0x0020,
	L_THUMB = 0x0040,
	R_THUMB = 0x0080,
	L_SHOULDER = 0x0100,
	R_SHOULDER = 0x0200,
	BUTTON_A = 0x1000,
	BUTTON_B = 0x2000,
	BUTTON_X = 0x4000,
	BUTTON_Y = 0x8000
};

// デッドゾーン
const float DEADZONE = 0.1f;

class Input {
public:

	Input();
	~Input();
	Input(const Input&) = delete;
	const Input& operator=(const Input&) = delete;

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init(WNDCLASS wCalss, HWND hwnd);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

public:

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static  Input* GetInstance();

	static BYTE GetKey(uint8_t keyNum) { return key_[keyNum]; }

public:
	/// <summary>
	/// キーボードの初期化
	/// </summary>
	/// <param name="wCalss"></param>
	/// <param name="hwnd"></param>
	void KeyboardInitialize(HWND hwnd);

	/// <summary>
	/// マウスの初期化
	/// </summary>
	/// <param name="wCalss"></param>
	/// <param name="hwnd"></param>
	void MouseInitialize();

	/// <summary>
	/// ゲームパッドの初期化
	/// </summary>
	void GamePadInitialize();

public: // 入力

	// ---------------------------------------------------------------
	// ↓　キーボード
	// ---------------------------------------------------------------
	// キーを押した瞬間か
	static bool IsTriggerKey(uint8_t keyNum);
	// キーを離した瞬間か
	static bool IsReleaseKey(uint8_t keyNum);
	// キーを押した状態か
	static bool IsPressKey(uint8_t keyNum);
	// キーを離した状態か
	static bool IsUnPressKey(uint8_t keyNum);

	// ---------------------------------------------------------------
	// ↓　マウス
	// ---------------------------------------------------------------
	// キーを押した瞬間か
	static bool IsTriggerMouse(uint8_t keyNum);
	// キーを離した瞬間か
	static bool IsReleaseMouse(uint8_t keyNum);
	// キーを押した状態か
	static bool IsPressMouse(uint8_t keyNum);
	// キーを離した状態か
	static bool IsUnPressMouse(uint8_t keyNum);

	static int GetWheel();

	/// <summary>
	/// マウスの座標を返す
	/// </summary>
	/// <returns></returns>
	static Vector2 GetMousePosition() { return { static_cast<float>(mousePoint_.x),static_cast<float>(mousePoint_.y) }; }

	/// <summary>
	/// マウスの移動量を返す
	/// </summary>
	/// <returns></returns>
	static Vector2 GetMouseVelocity() {
		return Vector2((static_cast<float>(currentMouse_.lX), static_cast<float>(currentMouse_.lY)));
	}

	// ---------------------------------------------------------------
	// ↓　ゲームパッド
	// ---------------------------------------------------------------
	// ゲームパッドのボタンを取得
	static bool GetIsPadTrigger(const XInputButtons& bottons);
	// ゲームパッドのボタンを取得(長押し)
	static bool GetPressPadTrigger(const XInputButtons& bottons);
	// 左ジョイスティックの値の取得
	static Vector2 GetLeftJoyStick();
	// 右ジョイスティックの値の取得
	static Vector2 GetRightJoyStick();
	// コントローラーと繋がっているか
	static bool IsControllerConnected();

private:
	// DirectInputオブジェクトの生成
	Microsoft::WRL::ComPtr<IDirectInput8> directInput_ = nullptr;
	// keyboardデバイスの生成
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard_ = nullptr;
	// mouseデバイスの生成
	Microsoft::WRL::ComPtr<IDirectInputDevice8> mouse_ = nullptr;
	// padデバイスの生成
	Microsoft::WRL::ComPtr<IDirectInputDevice8> gamepad = nullptr;

	// keyboard
	static BYTE key_[256];
	static BYTE preKey_[256];

	// mouse 
	static DIMOUSESTATE currentMouse_;
	static DIMOUSESTATE preMouse_;
	static POINT mousePoint_;

	// pad
	static XINPUT_STATE gamepadState_;
	static XINPUT_STATE preGamepadState_;

};
