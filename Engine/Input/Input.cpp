#include "Input.h"

BYTE Input::key_[256];
BYTE Input::preKey_[256];

DIMOUSESTATE Input::currentMouse_;
DIMOUSESTATE Input::preMouse_;

POINT Input::mousePoint_;

XINPUT_STATE Input::gamepadState_;
XINPUT_STATE Input::preGamepadState_;

Input* Input::GetInstance() {
	static Input instance;
	return &instance;
}

Input::Input() {
}

Input::~Input() {
	directInput_.Reset();
	keyboard_.Reset();
	mouse_.Reset();
}

//=================================================================================================================
//	↓　初期化
//=================================================================================================================
void Input::Init(WNDCLASS wCalss, HWND hwnd) {
	HRESULT result;
	// -------------------------- DirectInputの初期化 -------------------------- //
	result = DirectInput8Create(wCalss.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput_, nullptr);
	assert(SUCCEEDED(result));

	// -------------------------- keyboardデバイスの初期化 -------------------------- //
	KeyboardInitialize(hwnd);
	// -------------------------- mouseデバイスの初期化 -------------------------- //
	MouseInitialize();
}

//=================================================================================================================
//	↓　更新
//=================================================================================================================
void Input::Update() {
	// keyboard情報の取得開始
	keyboard_->Acquire();
	mouse_->Acquire();

	// keyの値をpreKeyにコピーする
	memcpy(preKey_, key_, sizeof(key_));
	preMouse_ = currentMouse_;

	// 全キーの入力状況を取得
	keyboard_->GetDeviceState(sizeof(key_), key_);
	mouse_->GetDeviceState(sizeof(DIMOUSESTATE), &currentMouse_);

	GetCursorPos(&mousePoint_);
	ScreenToClient(FindWindowA("CG2", nullptr), &mousePoint_);

	GamePadInitialize();
}

//=================================================================================================================
//	↓　キーボードの初期化
//=================================================================================================================
void Input::KeyboardInitialize(HWND hwnd) {
	HRESULT result;
	
	result = directInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	assert(SUCCEEDED(result));

	// 入力形式のセット
	result = keyboard_->SetDataFormat(&c_dfDIKeyboard); // 標準形式
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result = keyboard_->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
	);
	assert(SUCCEEDED(result));
}

//=================================================================================================================
//	↓　マウスの初期化
//=================================================================================================================
void Input::MouseInitialize() {
	HRESULT result;

	result = directInput_->CreateDevice(GUID_SysMouse, &mouse_, NULL);
	assert(SUCCEEDED(result));

	result = mouse_->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result = mouse_->SetCooperativeLevel(
		FindWindow(L"CG2", nullptr),
		DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	assert(SUCCEEDED(result));
}

//=================================================================================================================
//	↓　ゲームパッドの初期化
//=================================================================================================================
void Input::GamePadInitialize() {
	preGamepadState_ = gamepadState_;
	ZeroMemory(&gamepadState_, sizeof(XINPUT_STATE));
	XInputGetState(0, &gamepadState_);
}

//=================================================================================================================
//	↓　キーボードの判定
//=================================================================================================================
// ---------------------------------------------------------------
// ↓　キーを押した瞬間か
// ---------------------------------------------------------------
bool Input::IsTriggerKey(uint8_t keyNum) {
	if (!preKey_[keyNum] && key_[keyNum]) {
		return true;
	}

	return false;
}

// ---------------------------------------------------------------
// ↓　キーを離した瞬間か
// ---------------------------------------------------------------
bool Input::IsReleaseKey(uint8_t keyNum) {
	if (preKey_[keyNum] && !key_[keyNum]) {
		return true;
	}

	return false;
}

// ---------------------------------------------------------------
// ↓　キーを押した状態か
// ---------------------------------------------------------------
bool Input::IsPressKey(uint8_t keyNum) {
	if (key_[keyNum]) {
		return true;
	}

	return false;
}

// ---------------------------------------------------------------
// ↓　キーを離した状態か
// ---------------------------------------------------------------
bool Input::IsUnPressKey(uint8_t keyNum) {
	if (!key_[keyNum]) {
		return true;
	}

	return false;
}

//=================================================================================================================
//	↓　マウスの判定
//=================================================================================================================
// ---------------------------------------------------------------
// ↓　マウスを押した瞬間か
// ---------------------------------------------------------------
bool Input::IsTriggerMouse(uint8_t keyNum) {
	if (!preMouse_.rgbButtons[keyNum] && (currentMouse_.rgbButtons[keyNum] & (0x80))) {
		return true;
	}

	return false;
}

// ---------------------------------------------------------------
// ↓　マウスを離した瞬間か
// ---------------------------------------------------------------
bool Input::IsReleaseMouse(uint8_t keyNum) {
	if (preMouse_.rgbButtons[keyNum] && !currentMouse_.rgbButtons[keyNum]) {
		return true;
	}

	return false;
}

// ---------------------------------------------------------------
// ↓　マウスを押した状態か
// ---------------------------------------------------------------
bool Input::IsPressMouse(uint8_t keyNum) {
	if (currentMouse_.rgbButtons[keyNum]) {
		return true;
	}

	return false;
}

// ---------------------------------------------------------------
// ↓　マウスを離した状態か
// ---------------------------------------------------------------
bool Input::IsUnPressMouse(uint8_t keyNum) {
	if (!currentMouse_.rgbButtons[keyNum]) {
		return true;
	}

	return false;
}

int Input::GetWheel() {
	int wheel = currentMouse_.lZ;
	return wheel;
}

// ---------------------------------------------------------------
// ↓　ゲームパッドのボタンの取得
// ---------------------------------------------------------------
bool Input::GetIsPadTrigger(int triggerNum) {
	if ((gamepadState_.Gamepad.wButtons & triggerNum) &&
		!(preGamepadState_.Gamepad.wButtons & triggerNum)) {

		return true;
	}

	return false;
}

Vector2 Input::GetLeftJoyStick() {
	float LX = gamepadState_.Gamepad.sThumbLX;
	float LY = gamepadState_.Gamepad.sThumbLY;

	if (std::abs(LX) < DEADZONE) LX = 0;
	if (std::abs(LY) < DEADZONE) LY = 0;

	return Vector2(LX, LY);
}

Vector2 Input::GetRightJoyStick() {
	float RX = gamepadState_.Gamepad.sThumbRX;
	float RY = gamepadState_.Gamepad.sThumbRY;

	if (std::abs(RX) < DEADZONE) RX = 0;
	if (std::abs(RY) < DEADZONE) RY = 0;

	return Vector2(RX, RY);
}
