#pragma once
#include "BaseScene.h"
#include "../Engine/light/LightGroup.h"

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

#include "../Engine/audio/Audio.h"

#include "../Game/System/RhythmManager.h"

#include <list>
#include <time.h>

#ifdef _DEBUG
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
	//float time = 0.f;

	//���C�g
	LightGroup* lightGroup = nullptr;

	//�ۉe
	float circleShadowDir[3] = { 0,-1,0 };
	float circleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle[2] = { 0.0f, 0.5f };

	//�Փ˃}�l�[�W���[
	CollisionManager* collisionManager = nullptr;


	//�I�[�f�B�I
	Audio* audio = nullptr;

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
	 

	//�N���X���������g

	//BGM
	bool IsBGM = false;

	int combo = 0;
	bool IsRhythmInput = false;

	//�G
	std::list<std::unique_ptr<Enemy>> enemy;
	//POP�֐�
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
#endif // _DEBUG


};
