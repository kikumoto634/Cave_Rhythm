#pragma once
#include "BaseScene.h"

#include "../Engine/light/LightGroup.h"

#include "../Game/2D/BaseSprites.h"
#include "../Game/3D/BaseObjObject.h"

#include "../Game/System/GameManager.h"

class TitleScene : public BaseScene
{
private:
	//�V�[���J��
	const float FadeSecond = 1.f;

public:
	
	//�R���X�g���N�^
	TitleScene(DirectXCommon* dxCommon, Window* window);

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
	void DebugSceneChange();
	void SceneChange();

private:
	//�v���C���[Obj
	std::unique_ptr<BaseObjObject> playerobj;

	//�G�l�~�[
	std::unique_ptr<BaseObjObject> enemyobj;

	//���C�g
	LightGroup* lightGroup = nullptr;

	//�V�[���J��
	std::unique_ptr<BaseSprites> fade;
	DirectX::XMFLOAT4 fadeColor = {0,0,0,1};
	Vector2 fadeInSize = {};
	float fadeCurrentFrame = 0;
	//Prev
	bool IsPrevSceneChange = true;
	//Next
	bool IsNextSceneChange = false;

	bool IsDebugScene= false;

};

