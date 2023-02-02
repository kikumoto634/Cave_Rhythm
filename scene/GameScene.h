#pragma once
#include "BaseGameScene.h"

#include "../Game/3D/Enemy/Enemy.h"
#include "../Game/3D/Enemy/Enemy2.h"
#include "../Game/3D/Coins/Coins.h"


class GameScene : public BaseGameScene
{
public:
	GameScene(DirectXCommon* dxCommon, Window* window, int saveHP = 5);

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
	//POP関数
	void EnemyInitPop();
	void EnemyPop(Vector2 pos, Vector2 dir);
	void Enemy2Pop(Vector2 pos);

	void CoinInitPop();

private:
	//敵
	std::list<std::unique_ptr<Enemy>> enemy;
	std::list<std::unique_ptr<Enemy2>> enemy2;
	const int IniCreateEnemyNum = 10;

	//コイン
	const int IniCreateCoinNum = 5;
	std::list<std::unique_ptr<Coins>> coin;

#ifdef _DEBUG
	//カメラ移動、回転変更フラグ
	bool IsCameraMovementChange = true;

	//敵POP情報
	int popPosition[2] = {5,5};
	int popDirection[2] = {1,0};
#endif // _DEBUG
};
