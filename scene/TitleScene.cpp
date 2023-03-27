#include "TitleScene.h"

#include "SceneManager.h"
#include "TutorialScene.h"
#include "GameScene.h"
#include "../Engine/math/Easing/Easing.h"

using namespace std;
using namespace DirectX;

TitleScene::TitleScene(DirectXCommon *dxCommon, Window *window)
	: BaseScene(
		dxCommon,
		window)
{
}

void TitleScene::Application()
{
	BaseScene::Application();
}

void TitleScene::Initialize()
{
	BaseScene::Initialize();

	//プレイヤー
	playerobj = make_unique<BaseObjObject>();
	playerobj->Initialize("human2");
	playerobj->SetPosition({-5.5f, -2.f, 0.f});
	playerobj->SetRotation({0.f, XMConvertToRadians(121), 0.f});
	playerobj->SetScale({5,5,5});

	//エネミー
	enemyobj = make_unique<BaseObjObject>();
	enemyobj->Initialize("slime");
	enemyobj->SetPosition({9.f, -3.f, -1.f});
	enemyobj->SetRotation({XMConvertToRadians(5.f), XMConvertToRadians(220), 0.f});
	enemyobj->SetScale({3,3,3});

	enemyobj2 = make_unique<BaseObjObject>();
	enemyobj2->Initialize("Skeleton");
	enemyobj2->SetPosition({7.f, -3.f, 2.f});
	enemyobj2->SetRotation({XMConvertToRadians(5), XMConvertToRadians(230), 0.f});
	enemyobj2->SetScale({5,5,5});

	title = make_unique<BaseSprites>();
	title->Initialize(18);
	title->SetPosition({640, 150});
	title->SetSize({800, 160});
	title->SetAnchorPoint({0.5f,0.5f});

	push = make_unique<BaseSprites>();
	push->Initialize(19);
	push->SetPosition({550, 600});
	push->SetSize({180, 60});
	push->SetAnchorPoint({0.5f,0.5f});

	button = make_unique<BaseSprites>();
	button->Initialize(17);
	button->SetPosition({700, 600});
	button->SetSize({60, 60});
	button->SetAnchorPoint({0.5f,0.5f});

	back = make_unique<BaseObjObject>();
	back->Initialize("wall");
	back->SetPosition({0,0,7});

	audio = Audio::GetInstance();

	//ライト
	lightGroup = LightGroup::Create();
	//色設定
	lightGroup->SetAmbientColor({0.15f, 0.15f, 0.15f});
	//3Dオブジェクト(.obj)にセット
	ObjModelObject::SetLight(lightGroup);

	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, false);
	lightGroup->SetDirLightActive(2, false);

	//シーン遷移(FadeOut)
	fadeInSize = {static_cast<float>(window->GetWindowWidth()), static_cast<float>(window->GetWindowHeight())};
	fade = make_unique<BaseSprites>();
	fade->Initialize(1);
	fade->SetColor(fadeColor);
	fade->SetSize({fadeInSize});
}

void TitleScene::Update()
{
	BaseScene::Update();

	if(input->Trigger(DIK_Z) && !IsNextSceneChange){
		IsNextSceneChange = true;
		audio->PlayWave(7,0.8f);
	}

#pragma region _3DObj Update
	playerobj->Update(camera);
	enemyobj->Update(camera);
	enemyobj2->Update(camera);
	back->Update(camera);
#pragma endregion

#pragma region _2DObj Update
	SceneChange();
	fade->Update();

	title->Update();
	push->Update();
	button->Update();
#pragma endregion

#pragma region Common Update
	lightGroup->Update();
#pragma endregion

#ifdef _DEBUG
	//Scene
	{
		//座標
		ImGui::SetNextWindowPos(ImVec2{1000,40});
		//サイズ
		ImGui::SetNextWindowSize(ImVec2{280,100});
		ImGui::Begin("SCENE");

		ImGui::Text("Now:Title   Next:Home");
		if(!IsPrevSceneChange && ImGui::Button("NextScene")){
			IsNextSceneChange = true;
		}
		ImGui::Text("Now:Title   Next:Debug");
		if(!IsPrevSceneChange && ImGui::Button("DebugScene")){
			IsDebugScene = true;
		}

		ImGui::End();
	}
#endif // _DEBUG


	BaseScene::EndUpdate();
}

void TitleScene::Draw()
{

	BaseScene::Draw();
#pragma region _3DObj Draw
	playerobj->Draw();
	enemyobj->Draw();
	enemyobj2->Draw();
	back->Draw();
#pragma endregion

#pragma region _2D_UIDraw
	Sprite::SetPipelineState();
	title->Draw();
	push->Draw();
	button->Draw();
	fade->Draw();
#pragma endregion

	BaseScene::EndDraw();
}

void TitleScene::Finalize()
{
	audio->Finalize();

	back->Finalize();

	button->Finalize();
	push->Finalize();
	title->Finalize();

	fade->Finalize();

	delete lightGroup;
	lightGroup = nullptr;

	enemyobj2->Finalize();
	enemyobj->Finalize();
	playerobj->Finalize();

	BaseScene::Finalize();
}

void TitleScene::NextSceneChange()
{
	sceneManager->SetNextScene(new TutorialScene(dxCommon,window, 5));
}

void TitleScene::DebugSceneChange()
{
	sceneManager->SetNextScene(new GameScene(dxCommon,window));
}

void TitleScene::SceneChange()
{
	//PrevSceneからの移動後処理
	if(IsPrevSceneChange){
		if(fadeColor.w <= 0){
			IsPrevSceneChange = false;
			fadeCurrentFrame = 0;
			return;
		}

		fadeColor.w = 
			Easing_Linear_Point2(1,0,Time_OneWay(fadeCurrentFrame, FadeSecond));
		fade->SetColor(fadeColor);
	}
	//NextSceneへの移動
	else if(IsNextSceneChange || IsDebugScene){

		if(fadeColor.w >= 1){
			if(IsNextSceneChange)NextSceneChange();
			else if(IsDebugScene)DebugSceneChange();
		}

		fadeColor.w = 
			Easing_Linear_Point2(0,1,Time_OneWay(fadeCurrentFrame, FadeSecond));
		fade->SetColor(fadeColor);
	}
}
