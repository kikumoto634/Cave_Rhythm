#pragma once
#include "BaseScene.h"

#include "../Game/3D/SampleFbxObject/SampleFbxObject.h"
#include "../Game/3D/SampleObjObject/SampleObjObject.h"
#include "../Engine/3D/TouchableObject.h"

#include "../Game/3D/Player/Player.h"
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

#include "../Game/3D/Enemy/TrainingDummy.h"

class HomeScene : public BaseScene
{
private:
	//�V�[���J��
	const float FadeSecond = 1.f;

public:
	
	//�R���X�g���N�^
	HomeScene(DirectXCommon* dxCommon, Window* window);

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
	bool IsComboColorChange = false;

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

	//�R�C��
	std::unique_ptr<Coins> coin;
	//�o��
	std::unique_ptr<Exit> exit;

#ifdef _DEBUG
	//�J�����ړ��A��]�ύX�t���O
	bool IsCameraMovementChange = true;
#endif // _DEBUG

	std::unique_ptr<TrainingDummy> dummy;

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
