#pragma once
#include "BaseScene.h"

#include "../Engine/light/LightGroup.h"

#include "../Game/3D/BaseObjObject.h"
#include "../Game/System/GameManager.h"

class EventScene : public BaseScene
{
public:
	
	//�R���X�g���N�^
	EventScene(DirectXCommon* dxCommon, Window* window);

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
	//�v���C���[Obj
	std::unique_ptr<BaseObjObject> playerobj;

	//���C�g
	LightGroup* lightGroup = nullptr;
};

