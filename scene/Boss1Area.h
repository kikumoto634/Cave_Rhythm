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
//�V�[���J��
	void SceneGameEnd()override;
	void NextSceneChange() override;
	void SceneChange() override;

	//������
	void AddCommonInitialize() override;
	void AddObject3DInitialize() override;
	void AddObject2DInitialize() override;

	//�X�V
	void AddCommonUpdate() override;
	void AddObject3DUpdate() override;
	void AddObject2DUpdate() override;
	void AddBeatEndUpdate() override;

	//�`��
	void AddObject3DDraw() override;
	void AddParticleDraw() override;
	void AddUIDraw() override;

	//�㏈��
	void AddObjectFinalize() override;
	void AddCommonFinalize() override;

private:
	std::unique_ptr<Boss1> boss;

	float Block_Size = 2.0f;
	//�����J��
	bool IsEnterClose = false;
	int enterBorderLineZ = 0;
	//-2~2, -3, -2
	std::unique_ptr<IndestructibleWall> enterWall[3];
	Vector3 enterWallBasePos = {-2,-3,-2};

	//�o���J��
	bool IsExitOpen = false;
	std::unique_ptr<IndestructibleWall> exitWall[3];
	Vector3 exitWallBasePos = {-2,-3,24};

	ObjModelManager* IndestructibleWallModel = nullptr;
	ObjModelManager* IndestructibleWallColliderModel = nullptr;
};

