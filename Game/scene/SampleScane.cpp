﻿#include "SampleScane.h"

#include "../Engine/math//Easing/Easing.h"

#include "../Game/Collision/CollisionSystem/Collision.h"
#include "../Game/Collision/SphereCollider.h"
#include "../Game/Collision/MeshCollider.h"

#include "SceneManager.h"
#include "TitleScene.h"

#include "../Engine/math/Easing/Easing.h"

using namespace std;
using namespace DirectX;

SampleScane::SampleScane(DirectXCommon *dxCommon, Window *window, int saveHP)
		: BaseScene(
		dxCommon,
		window)
{
	this->saveHP = saveHP;
}

void SampleScane::Application()
{
	BaseScene::Application();
}

void SampleScane::Initialize()
{
	BaseScene::Initialize();

	CommonInitialize();

	Object3DInitialize();

	Object2DInitialize();
}

void SampleScane::Update()
{
	//計測、BGM開始
	RhythmMeasure();

	BaseScene::Update();

#pragma region 入力処理

#ifdef _DEBUG
	if(input->Push(DIK_A)){
		if(!IsCameraMovementChange)		camera->RotVector({0.f, XMConvertToRadians(3.f), 0.f});
		else if(IsCameraMovementChange)	camera->MoveVector({-1.f, 0.f, 0.f});
	}
	else if(input->Push(DIK_D)){
		if(!IsCameraMovementChange)		camera->RotVector({0.f,XMConvertToRadians(-3.f), 0.f});
		else if(IsCameraMovementChange)	camera->MoveVector({1.f, 0.f, 0.f});
	}

	if(input->Trigger(DIK_W)){
		if(!IsCameraMovementChange)		camera->RotVector({XMConvertToRadians(-3.f), 0.f, 0.f});
		else if(IsCameraMovementChange)	camera->MoveVector({0.f, 0.f, 1.f});
	}
	else if(input->Trigger(DIK_S)){
		if(!IsCameraMovementChange)		camera->RotVector({XMConvertToRadians(3.f), 0.f, 0.f});
		else if(IsCameraMovementChange)	camera->MoveVector({0.f, 0.f, -1.f});
	}

	if(input->Push(DIK_Q)){
		camera->MoveVector({0.f, 1.f, 0.f});
	}
	else if(input->Push(DIK_E)){
		camera->MoveVector({0.f, -1.f, 0.f});
	}
#endif // _DEBUG

	//シーン更新
	SceneChange();

	//入力
	InputUpdate();

	//リズム
	RhythmJudgeUpdate();
	BeatEndUpdate();

	//更新
	Object3DUpdate();

	Object2DUpdate();

	//シーン、カメラ、汎用
	CommonUpdate();

#ifdef _DEBUG
	{
		//座標
		ImGui::SetNextWindowPos(ImVec2{0,100});
		//サイズ
		ImGui::SetNextWindowSize(ImVec2{300,150});
		ImGui::Begin("Debug");
		//カメラ 回転:false , 移動:true
		ImGui::Text("Camera");
		ImGui::Text("true = transform / false = rotation");
		ImGui::Checkbox("Change", &IsCameraMovementChange);
		if(ImGui::Button("Shake")){
			camera->ShakeStart();
		}
		ImGui::End();
	}

	//Scene
	{
		//座標
		ImGui::SetNextWindowPos(ImVec2{1000,40});
		//サイズ
		ImGui::SetNextWindowSize(ImVec2{280,150});
		ImGui::Begin("SCENE");

		ImGui::Text("Now:Home   Next:Game");
		if(!IsPrevSceneChange && ImGui::Button("NextScene")){
			IsNextSceneChange = true;
		}
		ImGui::Text("Now:Home   Next:Title");
		if(!IsPrevSceneChange && ImGui::Button("GameEnd")){
			IsGameEnd = true;
		}

		ImGui::End();
	}

#endif // _DEBUG

	BaseScene::EndUpdate();
}

void SampleScane::Draw()
{
	BaseScene::Draw();

	Object3DDraw();
	
	ParticleDraw();

	UIDraw();

#ifdef _DEBUG
	debugText->Printf(0,0,1.f,"Camera Target  X:%f, Y:%f, Z:%f", camera->GetTarget().x, camera->GetTarget().y, camera->GetTarget().z);
	debugText->Printf(0,16,1.f,"Camera Eye  X:%f, Y:%f, Z:%f", camera->GetEye().x, camera->GetEye().y, camera->GetEye().z);

	debugText->Printf(0,48,1.f,"Player Pos X:%f, Y:%f, Z:%f", player->GetPosition().x, player->GetPosition().y,player->GetPosition().z);

	debugText->Printf(0,580, 1.f,  "Time				: %lf[ms]", rhythmManager->GetTimer());
	debugText->Printf(0, 600, 1.f, "JudgeTimeBase		: %lf[ms]", rhythmManager->GetJudgeTimeBase());
	debugText->Printf(0, 620, 1.f, "InputJudgeTimeBase	: %lf[ms]", rhythmManager->GetInputJudgeTime());
	
	debugText->Printf(200, 640, 1.f, "COMBO	: %d", gameManager->GetComboNum());
	debugText->Printf(200, 660, 1.f, "COIN	: %d", gameManager->GetCoinNum());


	debugText->Printf(0, 640, 1.f, "IsBeat : %d", rhythmManager->GetIsRhythmEnd());
	debugText->Printf(0, 660, 1.f, "HP : %d", player->GetHP());


#endif // _DEBUG
	BaseScene::EndDraw();
}

void SampleScane::Finalize()
{
	ObjectFinaize();

	CommonFinalize();

#ifdef _DEBUG
	
#endif // _DEBUG


	BaseScene::Finalize();
}


#pragma region 初期化
void SampleScane::NextSceneChange()
{
}
void SampleScane::CommonInitialize()
{
	//衝突マネージャー
	collisionManager = CollisionManager::GetInstance();

	//リズムマネージャー
	rhythmManager = new RhythmManager();

	//ゲームマネージャー
	gameManager = new GameManager();
	gameManager->Initialize();

	areaManager = make_unique<AreaManager>();
	areaManager->RandamAreaInitialize();

	//カメラ
	camera->SetTarget(Vector3(0.f, 2.f, -3.f));
	camera->RotVector({XMConvertToRadians(-60.f), 0.f, 0.f});
	camera->Update();
}

void SampleScane::Object3DInitialize()
{
	//blenderでの保存スケールは 2/10(0.2)でのエクスポート
	player = make_unique<Player>();
	player->Initialize("human1");
	player->SetPosition(areaManager->GetPlayerPosition());
	player->SetWeaponPos({0,0,-2.f});
	player->SetRotation({0, DirectX::XMConvertToRadians(180),0.f});
	player->SetHp(saveHP);
	gameManager->InitializeSetHp(player->GetHP());

	//出口
	exit = make_unique<Exit>();
	exit->SetExitOpenNeedCoin(1);
	exit->Initialize("Exit");
	exit->SetPosition(areaManager->GetExitPosition());

	//スライム
	slime =make_unique<BlueSlime>();
	slime->Initialize("slime");

	coin=make_unique<Coins>();
	coin->Initialize("Coins");
}

void SampleScane::Object2DInitialize()
{
	//シーン遷移(FadeOut)
	fadeInSize = {static_cast<float>(window->GetWindowWidth()), static_cast<float>(window->GetWindowHeight())};
	fade = make_unique<BaseSprites>();
	fade->Initialize(1);
	fade->SetColor(fadeColor);
	fade->SetSize({fadeInSize});
}
#pragma endregion

#pragma region 更新
void SampleScane::InputUpdate()
{
	if(IsPrevSceneChange)return;

	//ToDo: 
	// 同時押しでのコンボ+2の修正(一度正解になったら次の入力可能時間まで入力不可。)
	// ミス入力の時、プレイヤーの移動を不可に。
	if(player->GetIsInputOnce()){
		rhythmManager->InputRhythm();
		IsRhythmInput = true;
	}
}

void SampleScane::Object3DUpdate()
{
	//プレイヤー
	if(player->DamageSound())	{
		gameManager->AudioPlay(2,0.2f);
		gameManager->HpDecrement();
	}
	player->Update(camera);
	player->SetMoveEasingMaxTime(static_cast<float>(rhythmManager->GetBPMTimeSub()));
	if(player->GetIsDead() && player->GetIsDeadAudioOnce())	{
		gameManager->AudioPlay(2,0.5f);
		IsGameEnd = true;
	}
	//出口
	exit->Update(camera);
	{
		Vector3 target = player->GetPosition() + Vector3{-1, 2, 0};
		Vector2 pos = exit->GetCoinSp()->ChangeTransformation(target, this->camera);
		exit->SetCoinSpPosition(pos);
	}

	if(slime->GetIsDeadAudio()){
		gameManager->AudioPlay(2, 0.5f);
		if(coin->PopPossible()){
			coin->Pop({slime->GetDeadParticlepos().x, -5, slime->GetDeadParticlepos().z});
		}
	}
	slime->Update(camera,player->GetPosition());

	if(coin->GetCoin()){
		gameManager->CoinIncrement();
		gameManager->AudioPlay(7,0.5f);
	}
	coin->Update(this->camera);
}

void SampleScane::Object2DUpdate()
{
	gameManager->SpriteUpdate();
}

void SampleScane::CommonUpdate()
{
	gameManager->PlayerCircleShadowSet(player->GetPosition());
	//地面
	areaManager->CSVAreaUpdate(this->camera, player->GetPosition());
	gameManager->LightUpdate(player->GetIsDead());

	//出口
	if(gameManager->GetCoinNum() >= exit->GetExitNeedCoinNum() && exit->GetIsPlayerContact()){
		exit->ExitOpen();
		player->SetIsExitOpen(true);
	}
	else if(!exit->GetIsPlayerContact()){
		exit->ExitClose();
		player->SetIsExitOpen(false);
	}

	//シーン遷移
	if(player->GetIsNextScene())	{
		IsNextSceneChange = true;
		if(!exit->GetIsOpenAudioOnce()){
			gameManager->AudioPlay(6, 0.5f);
			camera->ShakeStart();
			player->SetIsWait(true);
			exit->ModelChange();
		}
	}

	//すべての衝突をチェック
	collisionManager->CheckAllCollisions();
}

void SampleScane::RhythmMeasure()
{
	//計測開始
	if(IsPrevSceneChange) return;

	//リズム計測
	rhythmManager->StartMeasurement(clock());
	//計測開始時
	if(rhythmManager->GetMoveUpNumber() == 0 && IsBGMStart){
		gameManager->AudioPlay(8, 0.5f, true);
	}
	//リズム繰り上がり
	rhythmManager->BeatMoveUp();
}

void SampleScane::RhythmJudgeUpdate()
{
	if(IsPrevSceneChange) return;

	//リズム判別
	if(IsRhythmInput){
		IsRhythmInput = false;

		//High(入力が遅く、judgeTimeが更新された状態での更新)
		if(rhythmManager->HighJudgeRhythm()){
			gameManager->ComboIncrement();
			player->JudgeUpdate(true);
		}
		//Low(入力が早くて、JudgeTimeが更新されていない処理のみ通す　繰り上がり用確認整数との比較) judgeTimeが更新されるまで処理待ち
		else if(rhythmManager->GetMoveUpNumber() > rhythmManager->GetJudgeTimeBase()){
			if(rhythmManager->LowJudgeRhythm()){
				gameManager->ComboIncrement();
				player->JudgeUpdate(true);
			}
			//ミス
			else{
				gameManager->ComboReset();
				player->JudgeUpdate(false);
			}
		}
	}
}

void SampleScane::BeatEndUpdate()
{
	if(IsPrevSceneChange) return;

	//リズム終了時処理
	if(rhythmManager->GetIsRhythmEnd() && !IsGameEnd){
		
		//SE
		gameManager->AudioPlay(0,0.25f);

		//各オブジェクト処理
		if(!player->GetIsDead())player->IsBeatEndOn();

		areaManager->BeatEndUpdate(gameManager);

		exit->IsBeatEndOn();
		gameManager->IsBeatEndOn();

		slime->IsBeatEndOn();
		if(!slime->GetIsNotApp()){
			Vector3 lpos = areaManager->GetObjectPopPosition();
			slime->Pop({lpos.x, -3.5f,lpos.z});
		}

		if(coin->GetIsAlive()){
			coin->IsBeatEndOn();
		}
	}
}
#pragma endregion

#pragma region 描画
void SampleScane::Object3DDraw()
{
	player->Draw();

	areaManager->CSVAreaDraw();

	exit->Draw();

	slime->Draw();
	coin->Draw();
}

void SampleScane::ParticleDraw()
{
	areaManager->ParticleDraw();
	slime->ParticleDraw();
}

void SampleScane::UIDraw()
{
	Sprite::SetPipelineState();

	//出口
	exit->Draw2D();

	gameManager->SpriteDraw();

	//シーン遷移
	fade->Draw();
}

#pragma endregion

#pragma region シーン更新
void SampleScane::SceneGameEnd()
{
	sceneManager->SetNextScene(new TitleScene(dxCommon,window));
}

void SampleScane::SceneChange()
{
	//PrevSceneからの移動後処理
	if(IsPrevSceneChange){
		if(fadeColor.w <= 0){
			IsPrevSceneChange = false;
			fadeCurrentFrame = 0;
			//リズム
			rhythmManager->InitializeMeasurement(clock());
			return;
		}

		fadeColor.w = 
			Easing_Linear_Point2(1,0,Time_OneWay(fadeCurrentFrame, FadeSecond/2));
		fade->SetColor(fadeColor);
		fade->Update();
	}
	//NextSceneへの移動
	else if(IsNextSceneChange || IsGameEnd){

		if(fadeColor.w >= 1){
			if(IsNextSceneChange)NextSceneChange();
			else if(IsGameEnd)	SceneGameEnd();
		}

		fadeColor.w = 
			Easing_Linear_Point2(0,1,Time_OneWay(fadeCurrentFrame, FadeSecond));
		fade->SetColor(fadeColor);
		fade->Update();
	}
}
#pragma endregion

#pragma region 後処理
void SampleScane::ObjectFinaize()
{
#pragma region _3D解放
	player->Finalize();
	areaManager->CSVAreaFinalize();
	exit->Finalize();
	slime->Finalize();
	coin->Finalize();
#pragma endregion _3D解放

#pragma region _2D解放
	fade->Finalize();
#pragma endregion _2D解放
}

void SampleScane::CommonFinalize()
{
	gameManager->Finalize();
	delete gameManager;
	gameManager = nullptr;

	delete rhythmManager;
	rhythmManager = nullptr;
}
#pragma endregion



void SampleScane::InitializeCreateBlueSlime()
{
}

void SampleScane::InitializeCreateCoin()
{
}

void SampleScane::BlueSlimePop()
{
}
