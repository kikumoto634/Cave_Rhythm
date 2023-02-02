#pragma once
#include "BaseGameScene.h"

#include "../Game/3D/Coins/Coins.h"
#include "../Game/3D/Enemy/TrainingDummy.h"

class HomeScene : public BaseGameScene
{
public:
	HomeScene(DirectXCommon* dxCommon, Window* window, int saveHP = 5);

private:
	//�V�[���J��
	void NextSceneChange() override;

	///�ǉ�

	//������
	void AddCommonInitialize()  override;
	void AddObject3DInitialize()  override;
	void AddObject2DInitialize() override;

	//�X�V
	void AddInputUpdate() override;
	void AddObject3DUpdate() override;
	void AddObject2DUpdate() override;
	void AddCommonUpdate() override;

	void AddBeatEndUpdate() override;

	//�`��
	void AddObject3DDraw() override;
	void AddObject2DDraw() override;
	void AddParticleDraw() override;
	void AddUIDraw() override;

	//�㏈��
	void AddObjectFinalize() override;
	void AddCommonFinalize() override;

#ifdef _DEBUG
	void AddDebugUpdate() override;
	void AddDebugDraw() override;
	void AddDebugFinalize() override;
#endif // _DEBUG

private:

	//�R�C��
	std::unique_ptr<Coins> coin;

	std::unique_ptr<TrainingDummy> dummy;

	//�X�v���C�g
	std::unique_ptr<BaseSprites> buttonSp[4];
	bool IsButtonPush[4] = {};
};
