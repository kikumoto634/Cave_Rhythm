#pragma once
#include "BaseScene.h"

#include "../Game/3D/SampleFbxObject/SampleFbxObject.h"
#include "../Game/3D/SampleObjObject/SampleObjObject.h"
#include "../Engine/3D/TouchableObject.h"

#include "../Game/3D/Player/Player.h"
#include "../Game/3D/AreaOut/AreaOutRock.h"
#include "../Game/3D/Exit/Exit.h"

#include "../Game/Collision/CollisionSystem/CollisionPrimitive.h"
#include "../Game/Collision/CollisionSystem/CollisionManager.h"

#include "../Game/System/RhythmManager.h"
#include "../Game/System/GameManager.h"
#include "../Game/System/AreaManager.h"

#include <time.h>

class SampleScane : public BaseScene
{
private:
	//シーン遷移
	const float FadeSecond = 1.f;

public:
	
	//コンストラクタ
	SampleScane(DirectXCommon* dxCommon, Window* window, int saveHP = 5);

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

	//初期化
	void CommonInitialize();
	void Object3DInitialize();
	void Object2DInitialize();

	//更新
	void InputUpdate();
	void Object3DUpdate();
	void Object2DUpdate();
	void CommonUpdate();

	//リズム関連
	void RhythmMeasure();
	void RhythmJudgeUpdate();
	void BeatEndUpdate();

	//描画
	void Object3DDraw();
	void ParticleDraw();
	void UIDraw();

	//シーン遷移
	void SceneGameEnd();
	void SceneChange();

	//後処理
	void ObjectFinaize();
	void CommonFinalize();

private:
	//共通
	//衝突マネージャー
	//CollisionManager* collisionManager = nullptr;

	//player
	std::unique_ptr<Player> player;
	//セーブ体力
	int saveHP = 0;
	
	//skydome
	std::unique_ptr<SampleObjObject> skydome;
	//リズムカウント
	RhythmManager* rhythmManager = nullptr;
	//ゲームマネージャー
	GameManager* gameManager = nullptr;
	//リズム入力
	bool IsRhythmInput = false;
	//岩
	std::unique_ptr<AreaOutRock> rock;
	//出口
	std::unique_ptr<Exit> exit;
	//エリアマネージャー
	std::unique_ptr<AreaManager> areaManager;

#ifdef _DEBUG
	//カメラ移動、回転変更フラグ
	bool IsCameraMovementChange = true;
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

	//BGM再生フラグ
	bool IsBGMStart = false;
};

