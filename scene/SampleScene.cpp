#include "SampleScene.h"

#include "../Engine/math//Easing/Easing.h"

#include "../Game/Collision/CollisionSystem/Collision.h"
#include "../Game/Collision/SphereCollider.h"
#include "../Game/Collision/MeshCollider.h"

#include <sstream>
#include <iomanip>

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
	audio->LoadWave(0, "Resources/sound/rhythm.wav");
	audio->LoadWave(1, "Resources/sound/miss.wav");
	audio->LoadWave(2, "Resources/sound/kari01.wav");

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

	//リズム
	rhythmManager->InitializeMeasurement(clock());
}

void SampleScene::Update()
{
	//リズム計測
	rhythmManager->StartMeasurement(clock());
	//計測開始時
	if(rhythmManager->GetTimer() == 0){
		audio->PlayWave(2, 0.15f, true);
	}
	//リズム繰り上がり
	rhythmManager->BeatMoveUp();

	BaseScene::Update();

#pragma region 入力処理

#ifdef _DEBUG
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


	//敵出現
	if(input->Trigger(DIK_SPACE)){
		EnemyPop({0,-4,12.5}, {-1,0,0});
	}
#endif // _DEBUG

	if(player->GetIsMovement()){
		rhythmManager->InputRhythm();
		IsRhythmInput = true;
	}

#pragma endregion 入力処理

	//リズム判別
	if(IsRhythmInput){
		IsRhythmInput = false;
		if(rhythmManager->JudgeRhythm()){
			debugText->Print("YES", 0,560,1.2f);
			combo += 1;
		}
		else{
			debugText->Print("NO", 0,560,1.2f);
			combo = 0;
			audio->PlayWave(1,0.5f);
		}
	}

	//リズム終了時処理
	if(rhythmManager->GetIsRhythmEnd()){
		
		//SE
		audio->PlayWave(0,0.5f);

		//各オブジェクト処理
		player->SetIsBeatEnd(true);
		for(auto it = enemy.begin(); it != enemy.end(); it++){
			(*it)->SetIsBeatEnd(true);
		}
		for(int i = 0; i < DIV_NUM; i++){
			for(int j = 0; j < DIV_NUM; j++){
				plane[i][j]->SetIsBeatEnd(true);
			}
		}
	}

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


	debugText->Printf(0,580, 1.f,  "Time				: %lf[ms]", rhythmManager->GetTimer());
	debugText->Printf(0, 600, 1.f, "JudgeTimeBase		: %lf[ms]", rhythmManager->GetJudgeTimeBase());
	debugText->Printf(0, 620, 1.f, "InputJudgeTimeBase	: %lf[ms]", rhythmManager->GetInputJudgeTime());
	
	debugText->Printf(200, 660, 1.f, "COMBO	: %d", combo);


	//debugText->Printf(0, 640, 1.f, "Combo : %d", combo);
	debugText->Printf(0, 660, 1.f, "HP : %d", player->GetHP());

	debugText->Printf(1000,0,1.0f, "EnemyNum : %d", enemy.size());

#endif // _DEBUG
	BaseScene::EndDraw();
#pragma endregion _2D_UI描画

}

void SampleScene::Finalize()
{

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
