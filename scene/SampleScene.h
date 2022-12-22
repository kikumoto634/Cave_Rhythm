#pragma once
#include "BaseScene.h"
#include "../Engine/light/LightGroup.h"

#include "../Game/3D/SampleFbxObject/SampleFbxObject.h"
#include "../Game/3D/SampleObjObject/SampleObjObject.h"
#include "../Engine/base/ParticleManager.h"
#include "../Engine/3D/TouchableObject.h"

#include "../Game/3D/Player/Player.h"

#include "../Game/2D/SampleSprite/SampleSprite.h"

#include "../Game/Collision/CollisionSystem/CollisionPrimitive.h"
#include "../Game/Collision/CollisionSystem/CollisionManager.h"

#include "../Engine/audio/Audio.h"

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
	float time = 0.f;

	//���C�g
	LightGroup* lightGroup = nullptr;

	//�ۉe
	float circleShadowDir[3] = { 0,-1,0 };
	float circleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle[2] = { 0.0f, 0.5f };

	//�Փ˃}�l�[�W���[
	CollisionManager* collisionManager = nullptr;

	//�p�[�e�B�N��
	ParticleManager* particle = nullptr;
	WorldTransform particleWorld = {};

	//�I�[�f�B�I
	Audio* audio = nullptr;

	//3D�I�u�W�F�N�g

	//player
	std::unique_ptr<Player> player;

	//Plane
	static const int DIV_NUM = 13;
	static const int Plane_Size = 3;
	std::unique_ptr<TouchableObject> plane[DIV_NUM][DIV_NUM];

	//skydome
	std::unique_ptr<SampleObjObject> skydome;

#ifdef _DEBUG
	imguiManager* imgui;

	bool show_demo_window = false;
#endif // _DEBUG


	//���Y���J�E���g
	clock_t end;
	double Init_time;
	double count = 5;
	bool IsBGM = false;

	double subRhyrhm = 0;

	//���̓��Y��
	bool IsRhythmInput = false;
	bool IsOutSafe = false;
	clock_t inputClock;
	double inputTime;
	double GameTime;

	//BPM�v�Z�p
	double BPM = 60;
	double Debug = 60;

	//���Y���ł̃I�u�W�F�N�g�X�P�[��
	float max = 1;
	float min = 0.5f;
};
