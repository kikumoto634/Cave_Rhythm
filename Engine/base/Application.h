#pragma once
#include "Window.h"
#include "DirectXCommon.h"

#include "TextureManager.h"

#include "ParticleManager.h"

#include "../Engine/loader/FbxLoader.h"
#include "FbxModelManager.h"
#include "../Engine/3D/FbxModelObject.h"

#include "../Engine/base/ObjModelManager.h"
#include "../Engine/3D/ObjModelObject.h"

#include "../light/DirectionalLight.h"


#include "../../scene/BaseScene.h"
#include "../../scene/SceneManager.h"
#include "../Engine/2D/Sprite.h"

#ifdef _DEBUG

#include "../Engine/base/imguiManager.h"
#include <imgui.h>
#endif // _DEBUG

class Application
{
private:
	//�V���O���g��
	static Application* app;

	Application();
	~Application();

public:
	//�V���O���g��(Application�𕡐����Ӗ��͂Ȃ����߁A�P��̑���)
	static Application* GetInstance();
	static void Delete();

	void Initialize();
	void Run();
	void Finalize();

private:
	void Update();
	void Draw();

private:
#pragma region �ėp�@�\
	Window* window;
	DirectXCommon* dxCommon;
#pragma endregion
	SceneManager* sceneManager = nullptr;

#ifdef _DEBUG
	DebugText* debugText;

	imguiManager* imgui;
	bool IsSceneChange_ImGui = false;
#endif // _DEBUG

};