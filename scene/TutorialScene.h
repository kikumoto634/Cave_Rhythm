#pragma once
#include "BaseBattleScene.h"

#include "../Game/3D/Enemy/BlueSlime.h"
#include "../Game/3D/Coins/Coins.h"

#include "../Game/2D/TutorialSp.h"

class TutorialScene : public BaseBattleScene
{
public:
	~TutorialScene();

	TutorialScene(DirectXCommon* dxCommon, Window* window, int saveHP = 5);

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
	void AddFrontUIDraw() override;
	void AddBackUIDraw() override;

	//後処理
	void AddObjectFinalize() override;
	void AddCommonFinalize() override;


private:
	void ActorCreateInitialize();

private:
	int slimePopNumMax = 10;
	std::vector<std::unique_ptr<BlueSlime>> slime;

	int coinPopNumMax = 10;
	std::vector<std::unique_ptr<Coins>> coin;

	//スプライト
	std::unique_ptr<TutorialSp> moveSp;
	const Vector3 moveSpPos = {-25,-3,30};

	std::unique_ptr<TutorialSp> attackSp;
	const Vector3 attackSpPos = {7,-3,20};
};

