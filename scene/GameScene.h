#pragma once
#include "BaseBattleScene.h"

#include "../Game/3D/Enemy/BlueSlime.h"
#include "../Game/3D/Enemy/Skelton.h"
#include "../Game/3D/Coins/Coins.h"

class GameScene : public BaseBattleScene
{
public:
	struct FloorInfo{
		int needCoin;

		//�G�l�~�[������
		int slimne;
		int skelton;
	};

//�萔
private:
	FloorInfo one	= {3,  10,5};
	FloorInfo two	= {5,  8, 7};
	FloorInfo three = {7,  5,10};
	FloorInfo four	= {10, 2,13};


	//�|�b�v�J�E���g
	const int PopCount = 3;

public:
	~GameScene();

	GameScene(DirectXCommon* dxCommon, Window* window, int saveHP = 5, int floorValue = 1);

private:
//�V�[���J��
	void NextSceneChange() override;
	void SceneGameEnd() override;

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
	void AddFrontUIDraw() override;
	void AddBackUIDraw() override;

	//�㏈��
	void AddObjectFinalize() override;
	void AddCommonFinalize() override;


private:
	void ActorCreateInitialize();

private:
	//�K�v�R�C��
	int needCoin = 10;

	//�|�b�v�J�E���g
	int popCount = 0;

	int slimePopNumMax = 5;
	std::vector<std::unique_ptr<BlueSlime>> slime;

	int skeltonPopNumMax = 5;
	std::vector<std::unique_ptr<Skelton>> skelton;

	int coinPopNumMax = 5;
	std::vector<std::unique_ptr<Coins>> coin;


	std::unique_ptr<BaseSprites> floorDepth;
	int numberTextBase = 3;
	std::unique_ptr<BaseSprites> floorValueTex;
};

