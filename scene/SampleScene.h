#pragma once
#include "BaseScene.h"
#include "../Engine/light/LightGroup.h"

#include "../Game/3D/SampleFbxObject/SampleFbxObject.h"
#include "../Game/3D/SampleObjObject/SampleObjObject.h"
#include "../Engine/base/ParticleManager.h"
#include "../Engine/3D/TouchableObject.h"

#include "../Game/3D/Player/Player.h"

#include "../Game/2D/SampleSprite/SampleSprite.h"

#include "../Game/Collision/CollisionSystem/CollisionPrimitive.h"
#include "../Game/Collision/CollisionSystem/CollisionManager.h"

#include "../Engine/audio/Audio.h"

#include <time.h>

#ifdef _DEBUG
#include "../Engine/base/imguiManager.h"
#endif // _DEBUG


class SampleScene : public BaseScene
{
public:
	
	//コンストラクタ
	SampleScene(DirectXCommon* dxCommon, Window* window);

	/// <summary>
	/// 起動時
	/// </summary>
	void Application() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 後処理
	/// </summary>
	void Finalize() override;

private:
	float time = 0.f;

	//ライト
	LightGroup* lightGroup = nullptr;

	//丸影
	float circleShadowDir[3] = { 0,-1,0 };
	float circleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle[2] = { 0.0f, 0.5f };

	//衝突マネージャー
	CollisionManager* collisionManager = nullptr;

	//パーティクル
	ParticleManager* particle = nullptr;
	WorldTransform particleWorld = {};

	//オーディオ
	Audio* audio = nullptr;

	//3Dオブジェクト

	//player
	std::unique_ptr<Player> player;

	//Plane
	static const int DIV_NUM = 13;
	static const int Plane_Size = 3;
	std::unique_ptr<TouchableObject> plane[DIV_NUM][DIV_NUM];

	//skydome
	std::unique_ptr<SampleObjObject> skydome;

#ifdef _DEBUG
	imguiManager* imgui;

	bool show_demo_window = false;
#endif // _DEBUG


	//リズムカウント
	clock_t end;
	double Init_time;
	double count = 5;
	bool IsBGM = false;

	double subRhyrhm = 0;

	//入力リズム
	bool IsRhythmInput = false;
	bool IsOutSafe = false;
	clock_t inputClock;
	double inputTime;
	double GameTime;

	//BPM計算用
	double BPM = 60;
	double Debug = 60;

	//リズムでのオブジェクトスケール
	float max = 1;
	float min = 0.5f;
};
