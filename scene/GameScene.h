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
	//POP�֐�
	void EnemyInitPop();
	void EnemyPop(Vector2 pos, Vector2 dir);
	void Enemy2Pop(Vector2 pos);

	void CoinInitPop();

private:
	//�G
	std::list<std::unique_ptr<Enemy>> enemy;
	std::list<std::unique_ptr<Enemy2>> enemy2;
	const int IniCreateEnemyNum = 10;

	//�R�C��
	const int IniCreateCoinNum = 5;
	std::list<std::unique_ptr<Coins>> coin;

#ifdef _DEBUG
	//�J�����ړ��A��]�ύX�t���O
	bool IsCameraMovementChange = true;

	//�GPOP���
	int popPosition[2] = {5,5};
	int popDirection[2] = {1,0};
#endif // _DEBUG
};
