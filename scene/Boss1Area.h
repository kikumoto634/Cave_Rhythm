#pragma once
#include "BaseBattleScene.h"

#include "../Game/3D/Enemy/Boss1.h"
#include "../Game/3D/Enemy/Skelton.h"
#include "../Game/2D/TutorialSp.h"

class Boss1Area : public BaseBattleScene
{
public:
	~Boss1Area();

	Boss1Area(DirectXCommon* dxCommon, Window* window, int saveHP = 5);

private:
//シーン遷移
	void SceneGameEnd()override;
	void NextSceneChange() override;
	void SceneChange() override;

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
	void cutinInitialize();
	void cutinUpdate();
	void cutinDraw();
	void cutinFinalize();


	void ActorCreateInitialize();
	void ActorSummon();

private:
	//エネミー
	std::unique_ptr<Boss1> boss;

	int skeltonPopNumMax = 10;
	std::vector<std::unique_ptr<Skelton>> skelton;

	float Block_Size = 2.0f;
	//入口開閉
	bool IsEnterClose = false;
	int enterBorderLineZ = 0;
	std::unique_ptr<IndestructibleWall> enterWall[3];
	Vector3 enterWallBasePos = {-2,-3,-2};

	//出口開閉
	bool IsExitOpen = false;
	std::unique_ptr<IndestructibleWall> exitWall[3];
	Vector3 exitWallBasePos = {-2,-3,24};

	//モデル
	ObjModelManager* IndestructibleWallModel = nullptr;
	ObjModelManager* IndestructibleWallColliderModel = nullptr;


	//ゲーム開始
	bool IsBossStart = false;


	//カットインSp
	bool IsCutInHide = false;
	bool IsCutInMoveStart = true;
	bool IsCutInMoveEnd = false;

	const float cutinSecond = 0.5f;

	std::unique_ptr<BaseSprites> cutInSpMain;
	Vector2 cutInPos = {1920,360};
	float cutInMoveFrameCur = 0;

	std::unique_ptr<BaseSprites> cutInSpPart1;
	Vector2 cutInPartPos1 = {1680,600};

	std::unique_ptr<BaseSprites> cutInSpPart2;
	Vector2 cutInPartPos2 = {-400,120};

	std::unique_ptr<BaseSprites> bossName;
	Vector2 bossNamePos = {1605,610};
};

