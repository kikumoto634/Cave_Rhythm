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
//�V�[���J��
	void NextSceneChange() override;

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
	void ActorCreateInitialize();

private:
	//�_���W�����e�L�X�g
	std::unique_ptr<TutorialSp> exitText;
	const Vector3 exitTextPos = {-2,-3,-9};

	std::unique_ptr<TutorialSp> trainingText;
	const Vector3 trainingTextPos = {-10,-3,-2};

	int slimePopNumMax = 1;
	std::vector<std::unique_ptr<BlueSlime>> slime;
};

