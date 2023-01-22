#pragma once
#include "BaseScene.h"

#include "../Engine/light/LightGroup.h"

#include "../Game/3D/BaseObjObject.h"
#include "../Game/System/GameManager.h"

class TitleScene : public BaseScene
{
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
	//プレイヤーObj
	std::unique_ptr<BaseObjObject> playerobj;

	//エネミー
	std::unique_ptr<BaseObjObject> enemyobj;

	//ライト
	LightGroup* lightGroup = nullptr;
};

