#pragma once
#include "BaseScene.h"

#include "../Game/3D/SampleFbxObject/SampleFbxObject.h"
#include "../Game/3D/SampleObjObject/SampleObjObject.h"
#include "../Engine/3D/TouchableObject.h"

#include "../Game/3D/Player/Player.h"
#include "../Game/3D/Exit/Exit.h"

#include "../Game/Collision/CollisionSystem/CollisionPrimitive.h"
#include "../Game/Collision/CollisionSystem/CollisionManager.h"

#include "../Game/System/RhythmManager.h"
#include "../Game/System/GameManager.h"
#include "../Game/System/AreaManager.h"

class BaseBattleScene : public BaseScene
{
private:
	//�V�[���J��
	const float FadeSecond = 1.f;

public:
	
	//�R���X�g���N�^
	BaseBattleScene(DirectXCommon* dxCommon, Window* window, int saveHP = 5);

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

protected:
	//�V�[���J��
	virtual void NextSceneChange() = 0;

	//������
	virtual void AreaManagerInitialize();
	virtual void AddCommonInitialize() = 0;
	virtual void AddObject3DInitialize() = 0;
	virtual void AddObject2DInitialize() = 0;

	//�X�V
	virtual void AddCommonUpdate() = 0;
	virtual void AddObject3DUpdate() = 0;
	virtual void AddObject2DUpdate() = 0;
	virtual void AddBeatEndUpdate() = 0;

	//�`��
	virtual void AddObject3DDraw() = 0;
	virtual void AddParticleDraw() = 0;
	virtual void AddUIDraw() = 0;

	//�㏈��
	virtual void AddObjectFinalize() = 0;
	virtual void AddCommonFinalize() = 0;

protected:
	//����
	//�Փ˃}�l�[�W���[
	CollisionManager* collisionManager = nullptr;

	//player
	std::unique_ptr<Player> player;
	//�Z�[�u�̗�
	int saveHP = 0;
	//���Y���J�E���g
	RhythmManager* rhythmManager = nullptr;
	//�Q�[���}�l�[�W���[
	GameManager* gameManager = nullptr;
	//���Y������
	bool IsRhythmInput = false;
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
	bool IsBGMStart = true;
};