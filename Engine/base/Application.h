#pragma once
#include "Window.h"
#include "DirectXCommon.h"

#include "TextureManager.h"

#include "ParticleManager.h"

#include "../audio/Audio.h"

#include "../loader/FbxLoader.h"
#include "FbxModelManager.h"
#include "../3D/FbxModelObject.h"

#include "../base/ObjModelManager.h"
#include "../3D/ObjModelObject.h"

#include "../light/DirectionalLight.h"


#include "../../Game/scene/BaseScene.h"
#include "../../Game/scene/SceneManager.h"
#include "../2D/Sprite.h"

#ifdef _DEBUG

#include "../base/imguiManager.h"
#include <imgui.h>
#endif // _DEBUG

class Application
{
private:
	//シングルトン
	static Application* app;

	Application();
	~Application();

public:
	//シングルトン(Applicationを複数持つ意味はないため、単一の存在)
	static Application* GetInstance();
	static void Delete();

	void Initialize();
	void Run();
	void Finalize();

private:
	void Update();
	void Draw();

private:
#pragma region 汎用機能
	Window* window;
	DirectXCommon* dxCommon;
#pragma endregion
	SceneManager* sceneManager = nullptr;

#ifdef _DEBUG
	DebugText* debugText = nullptr;

	imguiManager* imgui = nullptr;
	bool IsSceneChange_ImGui = false;
#endif // _DEBUG

};