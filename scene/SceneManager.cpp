#include "SceneManager.h"
#include "../Engine/audio/Audio.h"

SceneManager* SceneManager::instance = nullptr;

SceneManager::~SceneManager()
{
	scene->Finalize();
	delete scene;
}

SceneManager *SceneManager::GetInstance()
{
	if(!instance)
	{
		instance = new SceneManager();
	}
	return instance;
}

void SceneManager::Delete()
{
	if(instance){
		delete instance;
		instance = nullptr;
	}
}

void SceneManager::Update()
{
	if(nextScene){
#ifdef _DEBUG
		DebugText* text = nextScene->GetDebugText();
		imguiManager* imgui = nextScene->GetImGui();
#endif // _DEBUG
		if(scene){
#ifdef _DEBUG
			text = scene->GetDebugText();
			imgui = scene->GetImGui();
#endif // _DEBUG
			scene->Finalize();
			delete scene;
		}

		//シーン切り替え
		scene = nextScene;
#ifdef _DEBUG
		scene->SetDebugText(text);
		scene->SetImGui(imgui);
#endif // _DEBUG
		nextScene = nullptr;

		//シーンマネージャーセット
		scene->SetSceneManager(this);

		//初期化
		scene->Application();
		scene->Initialize();
		Audio::GetInstance()->Initialize();
	}

	scene->Update();
	scene->EndDraw();
}

void SceneManager::Draw()
{
	scene->Draw();
}
