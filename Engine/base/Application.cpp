#include "Application.h"
#include "../../scene/GameScene.h"
#include "../../scene/TitleScene.h"

Application* Application::app = nullptr;

using namespace std;

Application *Application::GetInstance()
{
	if(!app)
	{
		app = new Application();
	}
	return app;
}

void Application::Delete()
{
	if(app){
		delete app;
		app = nullptr;
	}
}

Application::Application()
{
	window = Window::GetInstance();
	dxCommon = DirectXCommon::GetInstance();
}

Application::~Application()
{
}

void Application::Run()
{
	MSG msg{};//���b�Z�[�W
	while (true)
	{
		if(PeekMessage(&msg, nullptr,0, 0,PM_REMOVE)){
			TranslateMessage(&msg);	//�L�[���̓��b�Z�[�W�̏���
			DispatchMessage(&msg);	//�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
		}
		if(msg.message == WM_QUIT) {
			break;
		}

		Update();
		Draw();
	}
}

void Application::Initialize()
{
#pragma region �ėp�@�\������
	//Window����
	window->Create("GiliraEngine", 1280, 720);

	//DirectXCommon
	dxCommon->Initialize(window);

	//�e�N�X�`��
	TextureManager::GetInstance()->Initialize(dxCommon);
	TextureManager::Load(0, "texfont.png");
	TextureManager::Load(1, "white1x1.png");
	TextureManager::Load(2, "2DText/combo.png");
	TextureManager::Load(3, "2DText/number0.png");
	TextureManager::Load(4, "2DText/number1.png");
	TextureManager::Load(5, "2DText/number2.png");
	TextureManager::Load(6, "2DText/number3.png");
	TextureManager::Load(7, "2DText/number4.png");
	TextureManager::Load(8, "2DText/number5.png");
	TextureManager::Load(9, "2DText/number6.png");
	TextureManager::Load(10, "2DText/number7.png");
	TextureManager::Load(11, "2DText/number8.png");
	TextureManager::Load(12, "2DText/number9.png");
	TextureManager::Load(13, "2DText/Coin.png");

#pragma endregion

	//�X�v���C�g�ÓI������
	Sprite::StaticInitialize(dxCommon, window->GetWindowWidth(), window->GetWindowHeight());
	
	//FBX
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());
	FbxModelObject::StaticInitialize(dxCommon);

	//OBJ
	ObjModelManager::StaticInitialize(dxCommon);
	ObjModelObject::StaticInitialize(dxCommon);

	//Light
	LightGroup::StaticInitialize(dxCommon->GetDevice());

	// �p�[�e�B�N���}�l�[�W��������
	ParticleManager::GetInstance()->Initialize(dxCommon);

#ifdef _DEBUG
	debugText = new DebugText();
	debugText->Initialize(0);

	imgui = new imguiManager();
	imgui->Initialize(window, dxCommon);
#endif // _DEBUG

	sceneManager = SceneManager::GetInstance();
	BaseScene* scene = new TitleScene(dxCommon, window);
#ifdef _DEBUG
	scene->SetDebugText(debugText);
	scene->SetImGui(imgui);
#endif // _DEBUG
	sceneManager->SetNextScene(scene);
}

void Application::Update()
{
#ifdef _DEBUG
	imgui->Begin();

#endif // _DEBUG

	sceneManager->Update();

#ifdef _DEBUG
	imgui->End();
#endif // _DEBUG
}

void Application::Draw()
{
	//�`��O����
	dxCommon->BeginDraw();

	Sprite::SetPipelineState();
	//scene->Draw();
	sceneManager->Draw();
#ifdef _DEBUG
	debugText->DrawAll();
	imgui->Draw();
#endif // _DEBUG

	//�`��㏈��
	dxCommon->EndDraw();
}

void Application::Finalize()
{
#ifdef _DEBUG
	imgui->Finalize();
	delete imgui;

	delete debugText;
	debugText = nullptr;
#endif // _DEBUG

	ObjModelObject::StaticFinalize();
	FbxModelObject::StaticFinalize();
	FbxLoader::GetInstance()->Finalize();
	Sprite::StaticFinalize();

	SceneManager::Delete();
	
	window->Finalize();
}
