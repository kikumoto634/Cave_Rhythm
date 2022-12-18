#pragma once
#include "../Engine/base/DirectXCommon.h"
#include "../Engine/base/Window.h"
#include "../Engine/input/Input.h"
#include "../camera/Camera.h"

#ifdef _DEBUG
#include "../Engine/debugProcess/DebugText.h"
#endif // _DEBUG



class BaseScene
{
//�����o�֐�
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	BaseScene(DirectXCommon* dxCommon, Window* window);

	/// <summary>
	/// �N����
	/// </summary>
	virtual void Application();

	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update();

	/// <summary>
	/// ��X�V
	/// </summary>
	void EndUpdate();

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// ��`��
	/// </summary>
	void EndDraw();

	/// <summary>
	/// �㏈��
	/// </summary>
	virtual void Finalize();

	//Getter
	bool GetIsSceneChange()	{return IsSceneChange;}
	//Reset
	void ResetIsSceneChange()	{IsSceneChange = false;}

//�����o�ϐ�
protected:

#pragma region �ėp�@�\
	//�؂����
	DirectXCommon* dxCommon = nullptr;
	Window* window = nullptr;
	
	//�쐬
	Input* input = nullptr;
	Camera* camera = nullptr;

	//�V�[���J��
	bool IsSceneChange = false;

#ifdef _DEBUG
	std::unique_ptr<DebugText> debugText;
#endif // _DEBUG

};

