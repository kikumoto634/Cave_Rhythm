#pragma once
#include "BaseScene.h"

#include "../Game/3D/SampleFbxObject/SampleFbxObject.h"
#include "../Game/3D/SampleObjObject/SampleObjObject.h"
#include "../Engine/3D/TouchableObject.h"

#include "../Game/3D/Player/Player.h"
#include "../Game/3D/Enemy/Enemy.h"
#include "../Game/3D/Plane/Planes.h"
#include "../Game/3D/AreaOut/AreaOutRock.h"

#include "../Game/2D/SampleSprite/SampleSprite.h"

#include "../Game/Collision/CollisionSystem/CollisionPrimitive.h"
#include "../Game/Collision/CollisionSystem/CollisionManager.h"

#include "../Game/System/RhythmManager.h"
#include "../Game/System/GameManager.h"

#include <list>
#include <time.h>

#ifdef _DEBUG
#include "../Game/3D/Enemy/TrainingDummy.h"
#include "../Engine/base/imguiManager.h"
#endif // _DEBUG

class SampleScene : public BaseScene
{
public:
	
	//�R���X�g���N�^
	SampleScene(DirectXCommon* dxCommon, Window* window);

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
	//�Փ˃}�l�[�W���[
	CollisionManager* collisionManager = nullptr;

	//3D�I�u�W�F�N�g

	//player
	std::unique_ptr<Player> player;

	//Plane
	static const int DIV_NUM = 11;
	static const float Plane_Size;
	std::unique_ptr<Planes> plane[DIV_NUM][DIV_NUM];

	//skydome
	std::unique_ptr<SampleObjObject> skydome;

	//���Y���J�E���g
	RhythmManager* rhythmManager = nullptr;

	//�Q�[���}�l�[�W���[
	GameManager* gameManager = nullptr;

	bool IsRhythmInput = false;

	//�G
	std::list<std::unique_ptr<Enemy>> enemy;
	const int IniCreateEnemyNum = 10;
	//POP�֐�
	void EnemyInitPop();
	void EnemyPop(Vector3 pos, Vector3 dir);

	//��
	std::unique_ptr<AreaOutRock> rock;

#ifdef _DEBUG
	imguiManager* imgui;
	bool show_demo_window = false;

	//�J�����ړ��A��]�ύX�t���O
	bool IsCameraMovementChange = true;

	//�GPOP���
	int popPosition[2] = {5,5};
	int popDirection[2] = {1,0};

	std::unique_ptr<TrainingDummy> dummy;

#endif // _DEBUG


};
