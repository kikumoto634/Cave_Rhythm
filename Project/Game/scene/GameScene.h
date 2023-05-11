#pragma once
#include "BaseBattleScene.h"

#include "BlueSlime.h"
#include "Skelton.h"
#include "Coins.h"

#include <queue>
#include <list>

class GameScene : public BaseBattleScene
{
public:
	struct FloorInfo{
		int needCoin;

		//エネミー生成数
		int slimne;
		int skelton;
	};

private:
	template <class T> using list = std::list<T>;
	template <class T> using queue = std::queue<T>;

//定数
private:
	FloorInfo one	= {3,  11,4};
	FloorInfo two	= {5,  8, 7};
	FloorInfo three = {7,  5,10};
	FloorInfo four	= {10, 2,13};

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
	int needCoin_ = 10;

	int slimePopNumMax_ = 5;
	int skeltonPopNumMax_ = 5;
	list<unique_ptr<BaseEnemy>> enemys_;

	int coinPopNumMax_ = 10;
	queue<Vector3> coinDropPos_ = {};
	list<unique_ptr<BaseObjObject>> obj_;


	unique_ptr<BaseSprites> floorDepth_;
	int numberTextBase_ = number0_tex.number;
	unique_ptr<BaseSprites> floorValueTex_;

	//ToDo
	//・アイテム(BaseObjObject)
	//・敵 (BaseEnemyObject)
	//・Player
};

