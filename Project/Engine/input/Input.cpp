#include "Input.h"
#include <cassert>

#pragma comment(lib, "dinput8.lib")

Input::Input()
{
}

Input *Input::GetInstance()
{
	static Input instance;
	return &instance;
}

void Input::Initialize(HWND hwnd)
{
	HRESULT result;

	hwnd_ = hwnd;

	///DirectInPut
	//初期化 (他入力方法追加でもこのオブジェクトは一つのみ)
	HINSTANCE hInstance = GetModuleHandle(nullptr);
	result = DirectInput8Create(
		hInstance, 
		DIRECTINPUT_VERSION, 
		IID_IDirectInput8,
		(void**)&directInput, 
		nullptr
	);
	assert(SUCCEEDED(result));


	//キーボードデバイスの生成 (GUID_Joystick (ジョイステック)、 GUID_SysMouse (マウス))
	result = directInput->CreateDevice(
		GUID_SysKeyboard,
		&keyboard_,
		NULL
	);
	assert(SUCCEEDED(result));
	//マウス
	result = directInput->CreateDevice(
		GUID_SysMouse,
		&mouse_,
		NULL
	);
	assert(SUCCEEDED(result));


	//入力データ形式のセット (入力デバイスの種類によって、あらかじめ何種類か用意する)
	result = keyboard_->SetDataFormat(&c_dfDIKeyboard);	//標準形式
	assert(SUCCEEDED(result));
	//マウス
	result = mouse_->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(result));


	//排他的制御レベルのセット
	//DISCL_FOREGROUND		画面が手前にある場合のみ入力を受け付ける
	//DISCL_NONEXCLUSIVE	デバイスをこのアプリだけで専有しない
	//DISCL_NOWINKEY		Windowsキーを無効にする
	result = keyboard_->SetCooperativeLevel(
		hwnd_,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
	);
	assert(SUCCEEDED(result));
	//マウス
	result = mouse_->SetCooperativeLevel(
		hwnd_,
		DISCL_NONEXCLUSIVE | DISCL_FOREGROUND
	);
	assert(SUCCEEDED(result));
}

void Input::Update()
{
	HRESULT result;

	//動作開始
	result = keyboard_->Acquire();
	result = mouse_->Acquire();
	result = mouse_->Poll();
	//assert(SUCCEEDED(result));

	//前回のキー入力情報コピー
	for(int i = 0; i <KeyNum; i++){
		prekey_[i] = key_[i];
	}
	mousePreKey_ = mouseKey_;


	//状態取得
	result = keyboard_->GetDeviceState(sizeof(key_), key_); 
	//assert(SUCCEEDED(result));
	result = mouse_->GetDeviceState(sizeof(mouseKey_), &mouseKey_);
}


bool Input::Push(int keyNumber)
{
	if(keyNumber < 0x00) return false;
	if(keyNumber > 0xff) return false;

	if(key_[keyNumber])
	{
		return true;
	}
	return false;
}

bool Input::Trigger(int keyNumber)
{
	if(keyNumber < 0x00) return false;
	if(keyNumber > 0xff) return false;

	if(key_[keyNumber] && !prekey_[keyNumber])
	{
		return true;
	}
	return false;
}


bool Input::MousePush(int keyNumber)
{
	if(mouseKey_.rgbButtons[keyNumber] & (MOUSE_ON_VALUE)){
		return true;
	}
	return false;
}

bool Input::MouseTrigger(int keyNumber)
{
	if((mouseKey_.rgbButtons[keyNumber] & (MOUSE_ON_VALUE)) && !(mousePreKey_.rgbButtons[keyNumber] & (MOUSE_ON_VALUE))){
		return true;
	}
	return false;
}

const Vector2 Input::GetMousePos()
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(hwnd_, &p);

	return Vector2{(float)p.x,(float)p.y};
}

const Vector2 Input::GetMouseVelocity()
{
	return Vector2((float)mouseKey_.lX, (float)mouseKey_.lY);
}
