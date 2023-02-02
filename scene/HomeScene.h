#pragma once
#include "BaseGameScene.h"

#include "../Game/3D/Coins/Coins.h"
#include "../Game/3D/Enemy/TrainingDummy.h"

class HomeScene : public BaseGameScene
{
public:
	HomeScene(DirectXCommon* dxCommon, Window* window, int saveHP = 5);

private:
	//シーン遷移
	void NextSceneChange() override;

	///追加

	//初期化
	void AddCommonInitialize()  override;
	void AddObject3DInitialize()  override;
	void AddObject2DInitialize() override;

	//更新
	void AddInputUpdate() override;
	void AddObject3DUpdate() override;
	void AddObject2DUpdate() override;
	void AddCommonUpdate() override;

	void AddBeatEndUpdate() override;

	//描画
	void AddObject3DDraw() override;
	void AddObject2DDraw() override;
	void AddParticleDraw() override;
	void AddUIDraw() override;

	//後処理
	void AddObjectFinalize() override;
	void AddCommonFinalize() override;

#ifdef _DEBUG
	void AddDebugUpdate() override;
	void AddDebugDraw() override;
	void AddDebugFinalize() override;
#endif // _DEBUG

private:

	//コイン
	std::unique_ptr<Coins> coin;

	std::unique_ptr<TrainingDummy> dummy;

	//スプライト
	std::unique_ptr<BaseSprites> buttonSp[4];
	bool IsButtonPush[4] = {};
};
