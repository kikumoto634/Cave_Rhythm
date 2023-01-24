#include "SceneManager.h"

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
		
		DebugText* text = nextScene->GetDebugText();
		imguiManager* imgui = nextScene->GetImGui();

		if(scene){
			text = scene->GetDebugText();
			imgui = scene->GetImGui();
			scene->Finalize();
			delete scene;
		}

		//�V�[���؂�ւ�
		scene = nextScene;
		scene->SetDebugText(text);
		scene->SetImGui(imgui);
		nextScene = nullptr;

		//�V�[���}�l�[�W���[�Z�b�g
		scene->SetSceneManager(this);

		//������
		scene->Application();
		scene->Initialize();
	}

	scene->Update();
	scene->EndDraw();
}

void SceneManager::Draw()
{
	scene->Draw();
}
