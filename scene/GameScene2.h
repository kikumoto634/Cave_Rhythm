#pragma once
#include "BaseScene.h"

#include "../Game/3D/SampleFbxObject/SampleFbxObject.h"
#include "../Game/3D/SampleObjObject/SampleObjObject.h"
#include "../Engine/3D/TouchableObject.h"

#include "../Game/3D/Player/Player.h"
#include "../Game/3D/Enemy/Enemy.h"
#include "../Game/3D/Enemy/Enemy2.h"
#include "../Game/3D/Plane/Planes.h"
#include "../Game/3D/AreaOut/AreaOutRock.h"
#include "../Game/3D/Coins/Coins.h"
#include "../Game/3D/Exit/Exit.h"

#include "../Game/Collision/CollisionSystem/CollisionPrimitive.h"
#include "../Game/Collision/CollisionSystem/CollisionManager.h"

#include "../Game/System/RhythmManager.h"
#include "../Game/System/GameManager.h"

#include <list>
#include <time.h>

class GameScene2 : public BaseScene
{
private:
	//シーン遷移
	const float FadeSecond = 1.f;

public:
	
	//コンストラクタ
	GameScene2(DirectXCommon* dxCommon, Window* window, int saveHP = 5);

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
	//シーン遷移
	void NextSceneChange();
	void SceneGameEnd();
	void SceneChange();

	//POP関数
	void EnemyInitPop();
	void EnemyPop(Vector2 pos, Vector2 dir);
	void Enemy2Pop(Vector2 pos);

	void CoinInitPop();

private:
	//衝突マネージャー
	CollisionManager* collisionManager = nullptr;

	//3Dオブジェクト

	//player
	std::unique_ptr<Player> player;
	//セーブ体力
	int saveHP = 0;

	//Plane
	static const int DIV_NUM = 11;
	static const float Plane_Size;
	std::unique_ptr<Planes> plane[DIV_NUM][DIV_NUM];
	bool IsComboColorChange = false;

	//skydome
	std::unique_ptr<SampleObjObject> skydome;

	//リズムカウント
	RhythmManager* rhythmManager = nullptr;

	//ゲームマネージャー
	GameManager* gameManager = nullptr;

	//リズム入力
	bool IsRhythmInput = false;

	//敵
	std::list<std::unique_ptr<Enemy>> enemy;
	std::list<std::unique_ptr<Enemy2>> enemy2;
	const int IniCreateEnemyNum = 10;

	//岩
	std::unique_ptr<AreaOutRock> rock;

	//コイン
	const int IniCreateCoinNum = 5;
	std::list<std::unique_ptr<Coins>> coin;

	//出口
	std::unique_ptr<Exit> exit;

#ifdef _DEBUG
	//カメラ移動、回転変更フラグ
	bool IsCameraMovementChange = true;

	//敵POP情報
	int popPosition[2] = {5,5};
	int popDirection[2] = {1,0};
#endif // _DEBUG

	//シーン遷移
	std::unique_ptr<BaseSprites> fade;
	DirectX::XMFLOAT4 fadeColor = {0,0,0,1};
	Vector2 fadeInSize = {};
	float fadeCurrentFrame = 0;
	//Prev
	bool IsPrevSceneChange = true;
	//Next
	bool IsNextSceneChange = false;

	//GameOver
	bool IsGameEnd = false;
};
