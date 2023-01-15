#pragma once
#include "../Engine/audio/Audio.h"
#include "../../Engine/light/LightGroup.h"

#include <string>

class GameManager
{
public:
	//初期化
	void Initialize();
	//後処理
	void Finalize();

	//コンボ加算
	void ComboIncrement();
	//コンボリセット
	void ComboReset();
	//コンボ数取得
	int GetComboNum()	{return comboNum;}

	//オーディオ追加
	void AudioAdd(int number, std::string path);
	//オーディオ再生
	void AudioPlay(int number, float volume = 1.f, bool loop = false);

	//ライト更新
	void LightUpdate();
	//プレイヤー丸影の設置
	void PlayerCircleShadowSet(Vector3 pos);

private:
	//オーディオ初期化
	void AudioInitialize();

	//ライト初期化
	void LightInitialize();

private:
	//コンボ
	int comboNum = 0;

	//オーディオ
	Audio* audio = nullptr;

	//ライト
	LightGroup* lightGroup = nullptr;
	//丸影
	//プレイヤー
	Vector3 circleShadowDir = { 0,-1,0 };
	Vector3 circleShadowAtten = { 0.5f,0.6f,0.0f };
	Vector2 circleShadowFactorAngle = { 0.0f, 0.5f };
};

