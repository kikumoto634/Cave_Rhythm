#pragma once
#include "BaseScene.h"

#include "../Game/3D/SampleFbxObject/SampleFbxObject.h"
#include "../Game/3D/SampleObjObject/SampleObjObject.h"
#include "../Engine/3D/TouchableObject.h"

#include "../Game/3D/Player/Player.h"
#include "../Game/3D/AreaOut/AreaOutRock.h"
#include "../Game/3D/Exit/Exit.h"

#include "../Game/Collision/CollisionSystem/CollisionPrimitive.h"
#include "../Game/Collision/CollisionSystem/CollisionManager.h"

#include "../Game/System/RhythmManager.h"
#include "../Game/System/GameManager.h"
#include "../Game/System/AreaManager.h"

#include <time.h>

class SampleScane : public BaseScene
{
private:
	//�V�[���J��
	const float FadeSecond = 1.f;

public:
	
	//�R���X�g���N�^
	SampleScane(DirectXCommon* dxCommon, Window* window, int saveHP = 5);

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

	//������
	void CommonInitialize();
	void Object3DInitialize();
	void Object2DInitialize();

	//�X�V
	void InputUpdate();
	void Object3DUpdate();
	void Object2DUpdate();
	void CommonUpdate();

	//���Y���֘A
	void RhythmMeasure();
	void RhythmJudgeUpdate();
	void BeatEndUpdate();

	//�`��
	void Object3DDraw();
	void ParticleDraw();
	void UIDraw();

	//�V�[���J��
	void SceneGameEnd();
	void SceneChange();

	//�㏈��
	void ObjectFinaize();
	void CommonFinalize();

private:
	//����
	//�Փ˃}�l�[�W���[
	//CollisionManager* collisionManager = nullptr;

	//player
	std::unique_ptr<Player> player;
	//�Z�[�u�̗�
	int saveHP = 0;
	
	//skydome
	std::unique_ptr<SampleObjObject> skydome;
	//���Y���J�E���g
	RhythmManager* rhythmManager = nullptr;
	//�Q�[���}�l�[�W���[
	GameManager* gameManager = nullptr;
	//���Y������
	bool IsRhythmInput = false;
	//��
	std::unique_ptr<AreaOutRock> rock;
	//�o��
	std::unique_ptr<Exit> exit;
	//�G���A�}�l�[�W���[
	std::unique_ptr<AreaManager> areaManager;

#ifdef _DEBUG
	//�J�����ړ��A��]�ύX�t���O
	bool IsCameraMovementChange = true;
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

	//BGM�Đ��t���O
	bool IsBGMStart = false;
};

