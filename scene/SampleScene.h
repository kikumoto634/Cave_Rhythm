#pragma once
#include "BaseScene.h"

#include "../Game/3D/SampleFbxObject/SampleFbxObject.h"
#include "../Game/3D/SampleObjObject/SampleObjObject.h"
#include "../Engine/3D/TouchableObject.h"

#include "../Game/3D/Player/Player.h"
#include "../Game/3D/Enemy/Enemy.h"
#include "../Game/3D/Plane/Planes.h"
#include "../Game/3D/AreaOut/AreaOutRock.h"

#include "../Game/2D/SampleSprite/SampleSprite.h"

#include "../Game/Collision/CollisionSystem/CollisionPrimitive.h"
#include "../Game/Collision/CollisionSystem/CollisionManager.h"

#include "../Game/System/RhythmManager.h"
#include "../Game/System/GameManager.h"

#include <list>
#include <time.h>

#ifdef _DEBUG
#include "../Game/3D/Enemy/TrainingDummy.h"
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
	//衝突マネージャー
	CollisionManager* collisionManager = nullptr;

	//3Dオブジェクト

	//player
	std::unique_ptr<Player> player;

	//Plane
	static const int DIV_NUM = 11;
	static const float Plane_Size;
	std::unique_ptr<Planes> plane[DIV_NUM][DIV_NUM];

	//skydome
	std::unique_ptr<SampleObjObject> skydome;

	//リズムカウント
	RhythmManager* rhythmManager = nullptr;

	//ゲームマネージャー
	GameManager* gameManager = nullptr;


	bool IsRhythmInput = false;

	//敵
	std::list<std::unique_ptr<Enemy>> enemy;
	const int IniCreateEnemyNum = 10;
	//POP関数
	void EnemyInitPop();
	void EnemyPop(Vector2 pos, Vector2 dir);

	//岩
	std::unique_ptr<AreaOutRock> rock;

#ifdef _DEBUG
	imguiManager* imgui;
	bool show_demo_window = false;

	//カメラ移動、回転変更フラグ
	bool IsCameraMovementChange = true;

	//敵POP情報
	int popPosition[2] = {5,5};
	int popDirection[2] = {1,0};

	std::unique_ptr<TrainingDummy> dummy;

#endif // _DEBUG


};
