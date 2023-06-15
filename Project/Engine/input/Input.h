﻿#pragma once
#include "Vector2.h"

//キーボード入力
#define DIRECTINPUT_VERSION 0x0800	//DirectInputのバージョン指定
#include <dinput.h>
const int KeyNum = 256;

//マウス
#define MOUSE_ON_VALUE 0x80

class Input
{
public:
	Input();
	~Input() = default;

	static Input* GetInstance();

	//初期化
	void Initialize(HWND hwnd);

	//更新
	void Update();

#pragma region キーボード入力
	bool Push(int keyNumber);
	bool Trigger(int keyNumber);
#pragma endregion

#pragma region マウス入力
	/// <summary>
	/// 0:Left, 1;Right, 2:Center
	/// </summary>
	bool MousePush(int keyNumber);
	bool MouseTrigger(int keyNumber);
	
	//マウスの座標
	const Vector2 GetMousePos();
	//マウスの移動量
	const Vector2 GetMouseVelocity();

#pragma endregion

private:
	HWND hwnd_;
	IDirectInput8* directInput = nullptr;

#pragma region キーボード
	//キーボードデバイス
	IDirectInputDevice8* keyboard_ = nullptr;
	//キー情報
	BYTE key_[KeyNum] = {};
	//前フレームキー情報
	BYTE prekey_[KeyNum] = {};
#pragma endregion


#pragma region マウス
	//マウスデバイス
	IDirectInputDevice8* mouse_ = nullptr;
	//マウス情報
	DIMOUSESTATE mouseKey_;
	DIMOUSESTATE mousePreKey_;

	//座標
	Vector2 mousePos_;
#pragma endregion
};

