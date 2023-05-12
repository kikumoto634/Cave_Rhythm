#pragma once
#include "BaseBattleScene.h"

#include "BlueSlime.h"
#include "Coins.h"

#include "TutorialSp.h"

class TutorialScene : public BaseBattleScene
{
private:
	template <class T> using queue = std::queue<T>;

private:
	//テキストの情報
	const Vector2 TextSize = {384,64};
	const Vector2 TextAnc = {0.5f,0.5f};

	//生成数
	int SlimePopNumMax_ = 2;
	int CoinPopNumMax_ = 2;

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
	list<unique_ptr<BaseEnemy>> enemy_;

	queue<Vector3> coinDropPos_ = {};
	list<unique_ptr<BaseObjObject>> obj_;

	//スプライト
	unique_ptr<TutorialSp> moveSp_;
	const Vector3 moveSpPos_ = {-25,-3,30};
};

