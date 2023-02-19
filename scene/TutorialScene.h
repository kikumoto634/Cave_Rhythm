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
	int slimePopNumMax = 10;
	std::vector<std::unique_ptr<BlueSlime>> slime;

	int coinPopNumMax = 10;
	std::vector<std::unique_ptr<Coins>> coin;

	//�X�v���C�g
	//�ړ�	(-25,-3,26)
	//�U��	(7,-3,16)
	std::unique_ptr<TutorialSp> moveSp;
	const Vector3 moveSpPos = {-25,-3,30};

	std::unique_ptr<TutorialSp> attackSp;
	const Vector3 attackSpPos = {7,-3,18};
};

