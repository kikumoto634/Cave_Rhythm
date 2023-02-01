#pragma once
#include "BaseGameScene.h"

#include "../Game/3D/Enemy/TrainingDummy.h"
#include "../Game/3D/Enemy/Enemy.h"
#include "../Game/3D/Enemy/Enemy2.h"

#include "../Game/3D/Coins/Coins.h"

class SampleGameScene: public BaseGameScene
{
public:
	SampleGameScene(DirectXCommon* dxCommon, Window* window, int saveHP = 5);

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
	void Enemy1Pop(Vector2 pos, Vector2 dir);
	void Enemy2Pop(Vector2 pos);

private:
	std::unique_ptr<TrainingDummy> dummy;
	std::unique_ptr<Enemy> enemy;
	std::unique_ptr<Enemy2> enemy2;

	//コイン
	std::unique_ptr<Coins> coin;

#ifdef _DEBUG
	//敵POP情報
	int popPosition[2] = {5,5};
	int popDirection[2] = {1,0};
#endif // _DEBUG
};

