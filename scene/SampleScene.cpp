#include "SampleScene.h"

#include "../Engine/math//Easing/Easing.h"

#include "../Game/Collision/CollisionSystem/Collision.h"
#include "../Game/Collision/SphereCollider.h"
#include "../Game/Collision/MeshCollider.h"

#include <sstream>
#include <iomanip>

#include <time.h>

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG


using namespace std;
using namespace DirectX;

const float SampleScene::Plane_Size = 2.5f;

SampleScene::SampleScene(DirectXCommon *dxCommon, Window *window)
		: BaseScene(
		dxCommon,
		window)
{
}

void SampleScene::Application()
{
	BaseScene::Application();
}

void SampleScene::Initialize()
{
	BaseScene::Initialize();

#pragma region 汎用初期化
	//ライト
	lightGroup = LightGroup::Create();
	//色設定
	lightGroup->SetAmbientColor({1,1,1});
	//3Dオブジェクト(.obj)にセット
	ObjModelObject::SetLight(lightGroup);

	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, true);

	//丸影
	lightGroup->SetCircleShadowActive(0, true);

	//衝突マネージャー
	collisionManager = CollisionManager::GetInstance();

	//パーティクル
	particle = ParticleManager::GetInstance();
	particleWorld.Initialize();

	//オーディオ
	audio = new Audio();
	audio->Initialize();
	audio->LoadWave(0, "Resources/rhythm.wav");
	audio->LoadWave(1, "Resources/MEDIUM-040_T120B07A03_e46HaR52rcg.wav");

	//リズムマネージャー
	rhythmManager = new RhythmManager();

	camera->SetEye(Vector3(0.f, 9.f, -18.f));

#pragma endregion 汎用初期化

#pragma region _3D初期化

	player = make_unique<Player>();
	player->Initialize("chr_sword");
	player->SetPosition({0, -4, 0});

	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			plane[i][j] = make_unique<Planes>();
			plane[i][j]->Initialize("Block1x1x1");
			plane[i][j]->SetPosition({ float(-((DIV_NUM/2)*Plane_Size) + (i*Plane_Size)) ,-5 ,float(j*Plane_Size)});
		}
	}

	skydome = make_unique<SampleObjObject>();
	skydome->Initialize("skydome");

#pragma endregion _3D初期化

#pragma region _2D初期化

#pragma endregion _2D初期化

#ifdef _DEBUG
	imgui = new imguiManager();
	imgui->Initialize(window, dxCommon);
#endif // _DEBUG
}

void SampleScene::Update()
{
	//入力

	rhythmManager->StartMeasurement(clock());
	rhythmManager->Update();
	//拍切り替え時
	if(rhythmManager->GetIsRhythm()){

		//成否処理
		if(rhythmManager->GetHighTime() >= inputTime && inputTime >= rhythmManager->GetLowTime()){
			IsOutSafe = true;
			combo+=1;
			inputTime = 0;
		}
		else{
			IsOutSafe = false;
			combo = 0;
			inputTime = 0;
		}
		//効果音
		audio->PlayWave(0);
		//オブジェクトScale遷移
		player->SetIsBeatEnd(true);
		for(auto it = enemy.begin(); it != enemy.end(); it++){
			(*it)->SetIsBeatEnd(true);
		}
		for(int i = 0; i < DIV_NUM; i++){
			for(int j = 0; j < DIV_NUM; j++){
				plane[i][j]->SetIsBeatEnd(true);
			}
		}
		//BGM再生
		if(!IsBGM){
			audio->PlayWave(1,0.1f); 
			IsBGM = true;
		}
	}

	BaseScene::Update();

#ifdef _DEBUG
	imgui->Begin();
#endif // _DEBUG

#pragma region 入力処理
	if(input->Push(DIK_A)){
		camera->RotVector({0.f, XMConvertToRadians(3.f), 0.f});
	}
	else if(input->Push(DIK_D)){
		camera->RotVector({0.f,XMConvertToRadians(-3.f), 0.f});
	}

	if(input->Push(DIK_W)){
		camera->RotVector({XMConvertToRadians(-3.f), 0.f, 0.f});
	}
	else if(input->Push(DIK_S)){
		camera->RotVector({XMConvertToRadians(3.f), 0.f, 0.f});
	}

	//ビート入力
	if(input->Trigger(DIK_UP) || input->Trigger(DIK_DOWN) || input->Trigger(DIK_RIGHT) || input->Trigger(DIK_LEFT)){
		
		inputClock = clock();
		inputTime = static_cast<double>(inputClock)/CLOCKS_PER_SEC;
	}

	//敵出現
	if(input->Trigger(DIK_SPACE)){
		EnemyPop({0,-4,12.5}, {-1,0,0});

	}

#pragma endregion 入力処理

#pragma region _3D更新
	player->Update(camera);

	for(auto it = enemy.begin(); it != enemy.end(); it++){
		(*it)->Update(camera);
	}
	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			plane[i][j]->Update(camera);
		}
	}

	skydome->Update(camera);
#pragma endregion _3D更新

#pragma region _2D更新

#pragma endregion _2D更新

#pragma region 汎用更新
	{
		//丸影
		lightGroup->SetCircleShadowDir(0, DirectX::XMVECTOR({circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0}));
		lightGroup->SetCircleShadowAtten(0, Vector3(circleShadowAtten[0], circleShadowAtten[1], circleShadowAtten[2]));
		lightGroup->SetCircleShadowFactorAngle(0, Vector2(circleShadowFactorAngle[0], circleShadowFactorAngle[1]));

		//プレイヤー、丸影座標
		lightGroup->SetCircleShadowCasterPos(0, player->GetPosition());
	}
	lightGroup->Update();


	//すべての衝突をチェック
	collisionManager->CheckAllCollisions();

	//パーティクル
	particle->Update(particleWorld, camera);
	particleWorld.UpdateMatrix();

#pragma endregion 汎用更新

#ifdef _DEBUG
	if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

	//ウィンドウサイズ
	ImGui::SetNextWindowSize(ImVec2{500,55});
	//ウィンドウ座標
	ImGui::SetNextWindowPos(ImVec2{0,130});
	//開始、タイトル名設定
	ImGui::Begin("PlayerPos && SpotLightPos && CircleShadowPos");
	ImGui::SetNextWindowPos(ImVec2{0,0});
	ImGui::DragFloat3("circlePos", (float*)&player->GetPosition(), 0.1f);
	//終了
	ImGui::End();

	//ウィンドウサイズ
	ImGui::SetNextWindowSize(ImVec2{500,100});
	//ウィンドウ座標
	ImGui::SetNextWindowPos(ImVec2{0,0});
	//開始、タイトル名設定
	ImGui::Begin("CircleShadow");
	ImGui::DragFloat3("circleShadowDir", circleShadowDir, 0.1f);
	ImGui::DragFloat3("circleShadowAtten", circleShadowAtten, 0.1f);
	ImGui::DragFloat2("circleShadowFactorAngle", circleShadowFactorAngle, 0.1f);
	//終了
	ImGui::End();

	imgui->End();
#endif // _DEBUG


	BaseScene::EndUpdate();
}

void SampleScene::Draw()
{
	BaseScene::Draw();

#pragma region _2D_背景描画
	
#pragma endregion _2D_背景描画

#pragma region _3D描画
	player->Draw();

	for(auto it = enemy.begin(); it != enemy.end(); it++){
		(*it)->Draw();
	}
	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			plane[i][j]->Draw();
		}
	}
	skydome->Draw();

#pragma region パーティクル
	particle->Draw();
#pragma endregion パーティクル

#pragma endregion _3D描画

#pragma region _2D_UI描画
	Sprite::SetPipelineState();

#ifdef _DEBUG
	debugText->Printf(0,0,1.f,"Camera Target  X:%f, Y:%f, Z:%f", camera->GetTarget().x, camera->GetTarget().y, camera->GetTarget().z);
	debugText->Printf(0,16,1.f,"Camera Eye  X:%f, Y:%f, Z:%f", camera->GetEye().x, camera->GetEye().y, camera->GetEye().z);

	if(!IsOutSafe) debugText->Print("NO", 0, 540), debugText->Printf(100, 540, 1.f, "MissRhythm : %f[ms]", rhythmManager->GetMessureTime() - inputTime);
	if(IsOutSafe) debugText->Print("YES", 0, 540);

	debugText->Printf(0,520, 1.f, "SubRhythm : %f[ms]", rhythmManager->GetSubRhythm());
	debugText->Printf(0,560, 1.f, "Input : %lf[ms]", inputTime);
	debugText->Printf(0,580, 1.f, "GoodTime : %lf[ms]", rhythmManager->GetMessureTime());
	debugText->Printf(0,600, 1.f, "%lf[ms]", rhythmManager->GetTimer());

	debugText->Printf(0, 640, 1.f, "Combo : %d", combo);
	debugText->Printf(0, 660, 1.f, "HP : %d", player->GetHP());

	debugText->Printf(1000,0,1.0f, "EnemyNum : %d", enemy.size());

#endif // _DEBUG
	BaseScene::EndDraw();
#pragma endregion _2D_UI描画

#ifdef _DEBUG
	imgui->Draw();
#endif // _DEBUG

}

void SampleScene::Finalize()
{
#ifdef _DEBUG
	imgui->Finalize();
	delete imgui;
#endif // _DEBUG

#pragma region _3D解放
	player->Finalize();

	for(auto it = enemy.begin(); it != enemy.end(); it++){
		(*it)->Finalize();
	}
	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			plane[i][j]->Finalize();
		}
	}

	skydome->Finalize();
#pragma endregion _3D解放

#pragma region _2D解放

#pragma endregion _2D解放

#pragma region 汎用解放

	delete rhythmManager;
	rhythmManager = nullptr;

	delete audio;
	audio=nullptr;

	delete lightGroup;
	lightGroup = nullptr;

	particleWorld = {};

#pragma endregion 汎用解放

	BaseScene::Finalize();
}

void SampleScene::EnemyPop(Vector3 pos, Vector3 dir)
{
	unique_ptr<Enemy> newObj = make_unique<Enemy>();
	newObj->Initialize("chr_sword");
	newObj->SetPosition(pos);
	newObj->SetDirection(dir);

	enemy.push_back(move(newObj));
}
