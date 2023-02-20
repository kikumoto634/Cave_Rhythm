#pragma once
#include "../Engine/base/DirectXCommon.h"
#include "../Engine/base/Window.h"
#include "../Engine/input/Input.h"
#include "../camera/Camera.h"

//#include "SceneManager.h"

#ifdef _DEBUG
#include "../Engine/debugProcess/DebugText.h"
#include "../Engine/base/imguiManager.h"

#include <imgui.h>
#endif // _DEBUG


//�O���錾
class SceneManager;

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

#ifdef _DEBUG
	void SetDebugText(DebugText* text)	{this->debugText = text;}
	void SetImGui(imguiManager* imgui)	{this->imgui = imgui;}

	DebugText* GetDebugText()	{return debugText;}
	imguiManager* GetImGui()	{return imgui;}
#endif // _DEBUG

	//Setter
	virtual void SetSceneManager(SceneManager* lsceneManager)	{sceneManager = lsceneManager;}

//�����o�ϐ�
protected:

#pragma region �ėp�@�\
	//�؂����
	DirectXCommon* dxCommon = nullptr;
	Window* window = nullptr;
	
	//�쐬
	Input* input = nullptr;
	Camera* camera = nullptr;

	//�؂蕨
	//�V�[���}�l�[�W���[
	SceneManager* sceneManager = nullptr;

#ifdef _DEBUG
	//�؂蕨
	DebugText* debugText = nullptr;
	imguiManager* imgui = nullptr;
	bool show_demo_window = false;
#endif // _DEBUG

};

