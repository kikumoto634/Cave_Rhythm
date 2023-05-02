#pragma once
#include "BaseBattleScene.h"

#include "BlueSlime.h"
#include "Skelton.h"
#include "Coins.h"

class GameScene : public BaseBattleScene
{
public:
	struct FloorInfo{
		int needCoin;

		//エネミー生成数
		int slimne;
		int skelton;
	};

//定数
private:
	FloorInfo one	= {3,  0, 1};
	FloorInfo two	= {5,  8, 7};
	FloorInfo three = {7,  5,10};
	FloorInfo four	= {10, 2,13};


	//ポップカウント
	const int PopCount = 3;

public:
	~GameScene();

	GameScene(DirectXCommon* dxCommon, Window* window, int saveHP = 5, int floorValue = 1);

private:
//シーン遷移
	void NextSceneChange() override;
	void SceneGameEnd() override;

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
	//必要コイン
	int needCoin = 10;

	//ポップカウント
	int popCount = 0;

	int slimePopNumMax = 5;
	//std::vector<std::unique_ptr<BlueSlime>> slime;

	int skeltonPopNumMax = 5;
	std::vector<std::unique_ptr<Skelton>> skelton;

	int coinPopNumMax = 5;
	std::vector<std::unique_ptr<Coins>> coin;


	std::unique_ptr<BaseSprites> floorDepth;
	int numberTextBase = 3;
	std::unique_ptr<BaseSprites> floorValueTex;
};

