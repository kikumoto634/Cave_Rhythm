#pragma once
#include "BaseBattleScene.h"

#include "../Game/3D/Enemy/BlueSlime.h"
#include "../Game/3D/Coins/Coins.h"

#include "../Game/2D/TutorialSp.h"

class HomeScene : public BaseBattleScene
{
public:
	~HomeScene();

	HomeScene(DirectXCommon* dxCommon, Window* window, int saveHP = 5);

private:
//シーン遷移
	void NextSceneChange() override;

	//初期化
	void AddCommonInitialize() override;
	void AddObject3DInitialize() override;
	void AddObject2DInitialize() override;

	//更新
	void AddCommonUpdate() override;
	void AddObject3DUpdate() override;
	void AddObject2DUpdate() override;
	void AddBeatEndUpdate() override;

	//描画
	void AddObject3DDraw() override;
	void AddParticleDraw() override;
	void AddUIDraw() override;

	//後処理
	void AddObjectFinalize() override;
	void AddCommonFinalize() override;


private:
	void ActorCreateInitialize();

private:
	//ダンジョンテキスト
	std::unique_ptr<TutorialSp> exitText;
	const Vector3 exitTextPos = {-2,-3,-9};

	std::unique_ptr<TutorialSp> trainingText;
	const Vector3 trainingTextPos = {-10,-3,-2};

	int slimePopNumMax = 1;
	std::vector<std::unique_ptr<BlueSlime>> slime;
};

