#pragma once
#include "BaseBattleScene.h"

#include "../Game/3D/Enemy/Boss1.h"

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
	std::unique_ptr<Boss1> boss;

	float Block_Size = 2.0f;
	//入口開閉
	bool IsEnterClose = false;
	int enterBorderLineZ = 0;
	//-2~2, -3, -2
	std::unique_ptr<IndestructibleWall> enterWall[3];
	Vector3 enterWallBasePos = {-2,-3,-2};

	//出口開閉
	bool IsExitOpen = false;
	std::unique_ptr<IndestructibleWall> exitWall[3];
	Vector3 exitWallBasePos = {-2,-3,24};

	ObjModelManager* IndestructibleWallModel = nullptr;
	ObjModelManager* IndestructibleWallColliderModel = nullptr;
};

