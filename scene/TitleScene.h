#pragma once
#include "BaseScene.h"

#include "../Engine/light/LightGroup.h"

#include "../Game/2D/BaseSprites.h"
#include "../Game/3D/BaseObjObject.h"

#include "../Game/System/GameManager.h"

class TitleScene : public BaseScene
{
private:
	//シーン遷移
	const float FadeSecond = 1.f;

public:
	
	//コンストラクタ
	TitleScene(DirectXCommon* dxCommon, Window* window);

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
	void DebugSceneChange();
	void SceneChange();

private:
	//プレイヤーObj
	std::unique_ptr<BaseObjObject> playerobj;

	//エネミー
	std::unique_ptr<BaseObjObject> enemyobj;

	//ライト
	LightGroup* lightGroup = nullptr;

	//シーン遷移
	std::unique_ptr<BaseSprites> fade;
	DirectX::XMFLOAT4 fadeColor = {0,0,0,1};
	Vector2 fadeInSize = {};
	float fadeCurrentFrame = 0;
	//Prev
	bool IsPrevSceneChange = true;
	//Next
	bool IsNextSceneChange = false;

	bool IsDebugScene= false;

};

