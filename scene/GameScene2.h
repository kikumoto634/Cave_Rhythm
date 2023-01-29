#pragma once
#include "BaseScene.h"

#include "../Game/3D/SampleFbxObject/SampleFbxObject.h"
#include "../Game/3D/SampleObjObject/SampleObjObject.h"
#include "../Engine/3D/TouchableObject.h"

#include "../Game/3D/Player/Player.h"
#include "../Game/3D/Enemy/Enemy.h"
#include "../Game/3D/Enemy/Enemy2.h"
#include "../Game/3D/Plane/Planes.h"
#include "../Game/3D/AreaOut/AreaOutRock.h"
#include "../Game/3D/Coins/Coins.h"
#include "../Game/3D/Exit/Exit.h"

#include "../Game/Collision/CollisionSystem/CollisionPrimitive.h"
#include "../Game/Collision/CollisionSystem/CollisionManager.h"

#include "../Game/System/RhythmManager.h"
#include "../Game/System/GameManager.h"

#include <list>
#include <time.h>

class GameScene2 : public BaseScene
{
private:
	//�V�[���J��
	const float FadeSecond = 1.f;

public:
	
	//�R���X�g���N�^
	GameScene2(DirectXCommon* dxCommon, Window* window, int saveHP = 5);

	/// <summary>
	/// �N����
	/// </summary>
	void Application() override;

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �㏈��
	/// </summary>
	void Finalize() override;

private:
	//�V�[���J��
	void NextSceneChange();
	void SceneGameEnd();
	void SceneChange();

	//POP�֐�
	void EnemyInitPop();
	void EnemyPop(Vector2 pos, Vector2 dir);
	void Enemy2Pop(Vector2 pos);

	void CoinInitPop();

private:
	//�Փ˃}�l�[�W���[
	CollisionManager* collisionManager = nullptr;

	//3D�I�u�W�F�N�g

	//player
	std::unique_ptr<Player> player;
	//�Z�[�u�̗�
	int saveHP = 0;

	//Plane
	static const int DIV_NUM = 11;
	static const float Plane_Size;
	std::unique_ptr<Planes> plane[DIV_NUM][DIV_NUM];
	bool IsComboColorChange = false;

	//skydome
	std::unique_ptr<SampleObjObject> skydome;

	//���Y���J�E���g
	RhythmManager* rhythmManager = nullptr;

	//�Q�[���}�l�[�W���[
	GameManager* gameManager = nullptr;

	//���Y������
	bool IsRhythmInput = false;

	//�G
	std::list<std::unique_ptr<Enemy>> enemy;
	std::list<std::unique_ptr<Enemy2>> enemy2;
	const int IniCreateEnemyNum = 10;

	//��
	std::unique_ptr<AreaOutRock> rock;

	//�R�C��
	const int IniCreateCoinNum = 5;
	std::list<std::unique_ptr<Coins>> coin;

	//�o��
	std::unique_ptr<Exit> exit;

#ifdef _DEBUG
	//�J�����ړ��A��]�ύX�t���O
	bool IsCameraMovementChange = true;

	//�GPOP���
	int popPosition[2] = {5,5};
	int popDirection[2] = {1,0};
#endif // _DEBUG

	//�V�[���J��
	std::unique_ptr<BaseSprites> fade;
	DirectX::XMFLOAT4 fadeColor = {0,0,0,1};
	Vector2 fadeInSize = {};
	float fadeCurrentFrame = 0;
	//Prev
	bool IsPrevSceneChange = true;
	//Next
	bool IsNextSceneChange = false;

	//GameOver
	bool IsGameEnd = false;
};
