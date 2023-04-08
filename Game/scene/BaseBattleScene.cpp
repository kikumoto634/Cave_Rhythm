﻿#include "BaseBattleScene.h"
#include "../../Engine/math//Easing/Easing.h"

#include "../../Game/Collision/CollisionSystem/Collision.h"
#include "../../Game/Collision/SphereCollider.h"
#include "../../Game/Collision/MeshCollider.h"

#include "SceneManager.h"
#include "TitleScene.h"

#include <thread>

using namespace std;

BaseBattleScene::BaseBattleScene(DirectXCommon *dxCommon, Window *window, int saveHP, int floorVaule)
	:BaseScene(
		dxCommon,
		window)
{
	this->saveHP = saveHP;
	this->floorValue = floorValue;
}

BaseBattleScene::~BaseBattleScene()
{
	Finalize();
}

void BaseBattleScene::Application()
{
	BaseScene::Application();
}

void BaseBattleScene::Initialize()
{
	BaseScene::Initialize();

	CommonInitialize();
	AddCommonInitialize();

	Object3DInitialize();
	AddObject3DInitialize();

	Object2DInitialize();
	AddObject2DInitialize();
}

void BaseBattleScene::Update()
{
	//計測、BGM開始
	RhythmMeasure();

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
	AddObject3DUpdate();

	Object2DUpdate();
	AddObject2DUpdate();

	//シーン、カメラ、汎用
	CommonUpdate();
	AddCommonUpdate();

#ifdef _DEBUG
	{
		//座標
		ImGui::SetNextWindowPos(ImVec2{0,100});
		//サイズ
		ImGui::SetNextWindowSize(ImVec2{300,125});
		ImGui::Begin("Camera");
		//カメラ 回転:false , 移動:true
		ImGui::Text("Camera");
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

void BaseBattleScene::Draw()
{
	BaseScene::Draw();

	Object3DDraw();
	AddObject3DDraw();
	
	ParticleDraw();
	AddParticleDraw();

	Sprite::SetPipelineState();
	AddFrontUIDraw();
	UIDraw();
	AddBackUIDraw();

#ifdef _DEBUG
	debugText->Printf(0,400,1.f,"Camera Target  X:%f, Y:%f, Z:%f", camera->GetTarget().x, camera->GetTarget().y, camera->GetTarget().z);
	debugText->Printf(0,420,1.f,"Camera Eye  X:%f, Y:%f, Z:%f", camera->GetEye().x, camera->GetEye().y, camera->GetEye().z);


	debugText->Printf(0,560,1.f,"Player Pos X:%f, Y:%f, Z:%f", player->GetPosition().x, player->GetPosition().y,player->GetPosition().z);

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

void BaseBattleScene::Finalize()
{
	ObjectFinaize();
	AddObjectFinalize();

	CommonFinalize();
	AddCommonFinalize();

	BaseScene::Finalize();
}


void BaseBattleScene::CommonInitialize()
{
	//衝突マネージャー
	collisionManager = CollisionManager::GetInstance();

	//リズムマネージャー
	rhythmManager = make_unique<RhythmManager>();

	//ゲームマネージャー
	gameManager = make_unique<GameManager>();
	gameManager->Initialize();

	areaManager = make_unique<AreaManager>();

	//カメラ
	camera->RotVector({XMConvertToRadians(-60.f), 0.f, 0.f});
	camera->Update();
}

void BaseBattleScene::Object3DInitialize()
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
	exit->Initialize("Exit");
	exit->SetPosition(areaManager->GetExitPosition());
	exit->SetExitOpenNeedCoin(1);
	exit->NeedCoinSpriteUpdate();
}

void BaseBattleScene::Object2DInitialize()
{
	//シーン遷移(FadeOut)
	fadeInSize = {static_cast<float>(window->GetWindowWidth()), static_cast<float>(window->GetWindowHeight())};
	fade = make_unique<BaseSprites>();
	fade->Initialize(1);
	fade->SetColor(fadeColor);
	fade->SetSize({fadeInSize});

	judgeLoca = make_unique<JudgeLocation>();
	judgeLoca->Initialize();

	VectorObjIni();
}

void BaseBattleScene::InputUpdate()
{
	if(IsPrevSceneChange)return;

	//ToDo: 
	// 同時押しでのコンボ+2の修正(一度正解になったら次の入力可能時間まで入力不可。)
	// ミス入力の時、プレイヤーの移動を不可に。
	if(player->GetIsInputOnce()){
		rhythmManager->InputRhythm();
		IsRhythmInput = true;
		IsNoteInput = true;
	}

	//入力を確認
	if(IsNoteInput){
		//左
		for(auto it = lNotes.begin(); it != lNotes.end(); it++){

			//前提条件
			if(!(*it)->GetIsNoteAlive()) continue;
			
			//ハートとノーツの当たり判定
			if(judgeLoca->GetPosition().x-judgeLoca->GetSize().x/2 <= (*it)->GetPosition().x+(*it)->GetSize().x/2 && 
				(*it)->GetPosition().x-(*it)->GetSize().x/2 <= judgeLoca->GetPosition().x+judgeLoca->GetSize().x/2){
				(*it)->InputUpdate();
			}
		}
		//右
		for(auto it = rNotes.begin(); it != rNotes.end(); it++){

			//前提条件
			if(!(*it)->GetIsNoteAlive()) continue;
			
			//ハートとノーツの当たり判定
			if(judgeLoca->GetPosition().x-judgeLoca->GetSize().x/2 <= (*it)->GetPosition().x+(*it)->GetSize().x/2 && 
				(*it)->GetPosition().x-(*it)->GetSize().x/2 <= judgeLoca->GetPosition().x+judgeLoca->GetSize().x/2){
				(*it)->InputUpdate();
			}
		}
	}
}

void BaseBattleScene::Object3DUpdate()
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
}

void BaseBattleScene::Object2DUpdate()
{
	gameManager->SpriteUpdate();

	judgeLoca->Update(IsNoteInput);

	for(auto it = lNotes.begin(); it != lNotes.end();it++){
		if((*it)->GetIsNoteAlive()){
			(*it)->Update((float)rhythmManager->GetBPMTime());
		}
	}
	for(auto it = rNotes.begin(); it != rNotes.end();it++){
		if((*it)->GetIsNoteAlive()){
			(*it)->Update((float)rhythmManager->GetBPMTime());
		}
	}
}

void BaseBattleScene::CommonUpdate()
{
	gameManager->PlayerCircleShadowSet(player->GetPosition());
	//地面
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

	//カメラ追従
	camera->Tracking(player->GetPosition());

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

void BaseBattleScene::RhythmMeasure()
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

void BaseBattleScene::RhythmJudgeUpdate()
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

void BaseBattleScene::BeatEndUpdate()
{
	if(IsPrevSceneChange) return;

	//リズム終了時処理
	if(rhythmManager->GetIsRhythmEnd() && !IsGameEnd){
		
		//SE
		gameManager->AudioPlay(0,0.25f);

		//各オブジェクト処理
		if(!player->GetIsDead())player->IsBeatEndOn();

		areaManager->BeatEndUpdate(gameManager.get());

		exit->IsBeatEndOn();
		gameManager->IsBeatEndOn();
		
		//ビート目視用
		for(auto it = lNotes.begin(); it != lNotes.end(); it++){
			if(!(*it)->GetIsNoteAlive()){
				(*it)->BeatUpdate();
				break;
			}
		}
		for(auto it = rNotes.begin(); it != rNotes.end(); it++){
			if(!(*it)->GetIsNoteAlive()){
				(*it)->BeatUpdate();
				break;
			}
		}

		AddBeatEndUpdate();
	}
}

void BaseBattleScene::Object3DDraw()
{
	player->Draw();
	exit->Draw();
}

void BaseBattleScene::ParticleDraw()
{
	areaManager->ParticleDraw();
}

void BaseBattleScene::UIDraw()
{
	//出口
	exit->Draw2D();

	for(auto it = lNotes.begin(); it != lNotes.end(); it++){
		(*it)->Draw();
	}
	for(auto it = rNotes.begin(); it != rNotes.end(); it++){
		(*it)->Draw();
	}
	judgeLoca->Draw();

	gameManager->SpriteDraw();

	//シーン遷移
	fade->Draw();
}

void BaseBattleScene::SceneGameEnd()
{
	sceneManager->SetNextScene(new TitleScene(dxCommon,window));
}

void BaseBattleScene::SceneChange()
{
	//PrevSceneからの移動後処理
	if(IsPrevSceneChange){

		//画面が開く
		{
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
	}
	//NextSceneへの移動
	else if(IsNextSceneChange || IsGameEnd){

		if(fadeColor.w >= 1){
			camera->Reset();
			if(IsNextSceneChange)NextSceneChange();
			else if(IsGameEnd)	SceneGameEnd();
		}

		fadeColor.w = 
			Easing_Linear_Point2(0,1,Time_OneWay(fadeCurrentFrame, FadeSecond));
		fade->SetColor(fadeColor);
		fade->Update();
	}
}

void BaseBattleScene::ObjectFinaize()
{
#pragma region _3D解放
	player->Finalize();
	exit->Finalize();
#pragma endregion _3D解放

#pragma region _2D解放
	for(auto it = lNotes.begin(); it != lNotes.end(); it++){
		(*it)->Finalize();
	}
	for(auto it = rNotes.begin(); it != rNotes.end(); it++){
		(*it)->Finalize();
	}
	judgeLoca->Finalize();
	fade->Finalize();
#pragma endregion _2D解放
}

void BaseBattleScene::CommonFinalize()
{
	gameManager->Finalize();

	rhythmManager = nullptr;
}

void BaseBattleScene::VectorObjIni()
{
	for(int i = 0; i < notesNum; i++){
		unique_ptr<LNotes> newsp_L = make_unique<LNotes>();
		newsp_L->Initialize(21);
		lNotes.push_back(move(newsp_L));

		unique_ptr<RNotes> newsp_R = make_unique<RNotes>();
		newsp_R->Initialize(21);
		rNotes.push_back(move(newsp_R));
	}
}
