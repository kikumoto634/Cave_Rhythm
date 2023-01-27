#include "TitleScene.h"

#include "SceneManager.h"
#include "GameScene.h"
#include "HomeScene.h"

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
	enemyobj->SetPosition({7.f, 0.f, 0.f});
	enemyobj->SetRotation({0.f, XMConvertToRadians(220), 0.f});
	enemyobj->SetScale({3,3,3});


	//ライト
	lightGroup = LightGroup::Create();
	//色設定
	lightGroup->SetAmbientColor({0.15f, 0.15f, 0.15f});
	//3Dオブジェクト(.obj)にセット
	ObjModelObject::SetLight(lightGroup);

	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, true);

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

	if(input->Trigger(DIK_Z))IsNextSceneChange = true;

#pragma region _3DObj Update
	playerobj->Update(camera);
	enemyobj->Update(camera);
#pragma endregion

#pragma region _2DObj Update
	SceneChange();
	fade->Update();
#pragma endregion

#pragma region Common Update
	lightGroup->Update();
#pragma endregion

#ifdef _DEBUG
	//プレイヤー
	{
		//座標
		ImGui::SetNextWindowPos(ImVec2{0,100});
		//サイズ
		ImGui::SetNextWindowSize(ImVec2{300,100});
		ImGui::Begin("Player");
		{
			//座標
			Vector3 lpos = playerobj->GetPosition();
			float pos[3] = {lpos.x, lpos.y, lpos.z};
			ImGui::DragFloat3("Position", pos, 0.05f);
			lpos = {pos[0],pos[1],pos[2]};
			playerobj->SetPosition(lpos);
		}
		{
			//回転
			Vector3 lrot = playerobj->GetRotation();
			float rot[3] = {XMConvertToDegrees(lrot.x), XMConvertToDegrees(lrot.y), XMConvertToDegrees(lrot.z)};
			ImGui::DragFloat3("Rotation", rot);
			lrot = {XMConvertToRadians(rot[0]),XMConvertToRadians(rot[1]),XMConvertToRadians(rot[2])};
			playerobj->SetRotation(lrot);
		}
		{
			//スケール
			Vector3 lscale = playerobj->GetScale();
			float scale[3] = {lscale.x, lscale.y, lscale.z};
			ImGui::InputFloat3("Scale", scale);
			lscale = {scale[0],scale[1],scale[2]};
			playerobj->SetScale(lscale);
		}
		ImGui::End();
	}

	//エネミー
	{
		//座標
		ImGui::SetNextWindowPos(ImVec2{0,200});
		//サイズ
		ImGui::SetNextWindowSize(ImVec2{300,100});
		ImGui::Begin("Enemy");
		{
			//座標
			Vector3 lpos = enemyobj->GetPosition();
			float pos[3] = {lpos.x, lpos.y, lpos.z};
			ImGui::DragFloat3("Position", pos, 0.05f);
			lpos = {pos[0],pos[1],pos[2]};
			enemyobj->SetPosition(lpos);
		}
		{
			//回転
			Vector3 lrot = enemyobj->GetRotation();
			float rot[3] = {XMConvertToDegrees(lrot.x), XMConvertToDegrees(lrot.y), XMConvertToDegrees(lrot.z)};
			ImGui::DragFloat3("Rotation", rot);
			lrot = {XMConvertToRadians(rot[0]),XMConvertToRadians(rot[1]),XMConvertToRadians(rot[2])};
			enemyobj->SetRotation(lrot);
		}
		{
			//スケール
			Vector3 lscale = enemyobj->GetScale();
			float scale[3] = {lscale.x, lscale.y, lscale.z};
			ImGui::InputFloat3("Scale", scale);
			lscale = {scale[0],scale[1],scale[2]};
			enemyobj->SetScale(lscale);
		}
		ImGui::End();
	}

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
#pragma endregion

#pragma region _2D_UIDraw
	Sprite::SetPipelineState();
	fade->Draw();
#pragma endregion

	BaseScene::EndDraw();
}

void TitleScene::Finalize()
{
	fade->Finalize();

	delete lightGroup;
	lightGroup = nullptr;

	enemyobj->Finalize();
	playerobj->Finalize();

	BaseScene::Finalize();
}

void TitleScene::NextSceneChange()
{
	sceneManager->SetNextScene(new HomeScene(dxCommon,window));
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
	else if(IsNextSceneChange){

		if(fadeColor.w >= 1){
			NextSceneChange();
		}

		fadeColor.w = 
			Easing_Linear_Point2(0,1,Time_OneWay(fadeCurrentFrame, FadeSecond));
		fade->SetColor(fadeColor);
	}
}
