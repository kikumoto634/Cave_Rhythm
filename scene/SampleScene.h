#pragma once
#include "BaseScene.h"
#include "../Engine/light/LightGroup.h"

#include "../Game/3D/SampleFbxObject/SampleFbxObject.h"
#include "../Game/3D/SampleObjObject/SampleObjObject.h"
#include "../Engine/3D/TouchableObject.h"

#include "../Game/3D/Player/Player.h"
#include "../Game/3D/Enemy/Enemy.h"
#include "../Game/3D/Plane/Planes.h"

#include "../Game/2D/SampleSprite/SampleSprite.h"

#include "../Game/Collision/CollisionSystem/CollisionPrimitive.h"
#include "../Game/Collision/CollisionSystem/CollisionManager.h"

#include "../Engine/audio/Audio.h"

#include "../Game/System/RhythmManager.h"

#include <list>
#include <time.h>


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
	//float time = 0.f;

	//ライト
	LightGroup* lightGroup = nullptr;

	//丸影
	float circleShadowDir[3] = { 0,-1,0 };
	float circleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle[2] = { 0.0f, 0.5f };

	//衝突マネージャー
	CollisionManager* collisionManager = nullptr;


	//オーディオ
	Audio* audio = nullptr;

	//3Dオブジェクト

	//player
	std::unique_ptr<Player> player;

	//Plane
	static const int DIV_NUM = 13;
	static const float Plane_Size;
	std::unique_ptr<Planes> plane[DIV_NUM][DIV_NUM];

	//skydome
	std::unique_ptr<SampleObjObject> skydome;

	//リズムカウント
	RhythmManager* rhythmManager = nullptr;
	 

	//クラス化未実装枠

	//BGM
	bool IsBGM = false;

	int combo = 0;
	bool IsRhythmInput = false;

	//敵
	std::list<std::unique_ptr<Enemy>> enemy;
	//POP関数
	void EnemyPop(Vector3 pos, Vector3 dir);
};
