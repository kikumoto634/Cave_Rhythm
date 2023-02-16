#pragma once
#include "BaseBattleScene.h"

#include "../Game/3D/Enemy/BlueSlime.h"
#include "../Game/3D/Coins/Coins.h"

class TutorialScene : public BaseBattleScene
{
public:
	TutorialScene(DirectXCommon* dxCommon, Window* window, int saveHP = 5);

private:
//�V�[���J��
	void NextSceneChange() override;

	//������
	void AreaManagerInitialize() override;
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
	int slimePopNumMax = 10;
	std::vector<std::unique_ptr<BlueSlime>> slime;

	int coinPopNumMax = 10;
	std::vector<std::unique_ptr<Coins>> coin;
};

