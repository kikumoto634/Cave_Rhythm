#include "BaseBattleScene.h"
#include "Easing.h"

#include "Collision.h"
#include "SphereCollider.h"
#include "MeshCollider.h"

#include "SceneManager.h"
#include "TitleScene.h"

#include <thread>

using namespace std;

BaseBattleScene::BaseBattleScene(DirectXCommon *dxCommon, Window *window, int saveHP, int floorVaule)
	:BaseScene(
		dxCommon,
		window)
{
	this->saveHP_ = saveHP;
	this->floorValue_ = floorValue_;
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

	//リズム
	rhythmManager_->PreUpdate();
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

	rhythmManager_->PostUpdate();

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
		if(!isPrevSceneChange_ && ImGui::Button("NextScene")){
			isNextSceneChange_ = true;
		}
		ImGui::Text("Now:Home   Next:Title");
		if(!isPrevSceneChange_ && ImGui::Button("GameEnd")){
			isGameEnd_ = true;
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


	debugText->Printf(0,560,1.f,"Player Pos X:%f, Y:%f, Z:%f", player_->GetPosition().x, player_->GetPosition().y,player_->GetPosition().z);

	debugText->Printf(0,580, 1.f,  "Time				: %lf[ms]", rhythmManager_->GetCalTime());
	debugText->Printf(0, 600, 1.f, "JudgeTimeBase		: %lf[ms]", rhythmManager_->GetInputTimeTarget());
	debugText->Printf(0, 620, 1.f, "InputJudgeTimeBase	: %lf[ms]", rhythmManager_->GetInputTimet());
	
	debugText->Printf(200, 640, 1.f, "COMBO	: %d", gameManager_->GetComboNum());
	debugText->Printf(200, 660, 1.f, "COIN	: %d", gameManager_->GetCoinNum());


	//debugText->Printf(0, 640, 1.f, "IsBeat : %d", rhythmManager->GetIsRhythmEnd());


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
	collisionManager_ = CollisionManager::GetInstance();

	//リズムマネージャー
	rhythmManager_ = make_unique<RhythmManager>();
	rhythmManager_->Initialize();

	//ゲームマネージャー
	gameManager_ = make_unique<GameManager>();
	gameManager_->Initialize();

	areaManager_ = make_unique<AreaManager>();

	//カメラ
	camera->RotVector({XMConvertToRadians(-60.f), 0.f, 0.f});
	camera->Update();
}

void BaseBattleScene::Object3DInitialize()
{
	//blenderでの保存スケールは 2/10(0.2)でのエクスポート
	player_ = make_unique<Player>();
	player_->Initialize("human1");
	player_->Pop(areaManager_->GetPlayerPosition());
	player_->SetWeaponModelPos({0,0,-2.f});
	player_->SetRotation({0, DirectX::XMConvertToRadians(180),0.f});
	player_->SetHp(saveHP_);
	gameManager_->InitializeSetHp(player_->GetHp());

	//出口
	exit_ = make_unique<Exit>();
	exit_->Initialize("Exit");
	exit_->SetPosition(areaManager_->GetExitPosition());
	exit_->SetExitOpenNeedCoin(0);
	exit_->NeedCoinSpriteUpdate();
}

void BaseBattleScene::Object2DInitialize()
{
	//シーン遷移(FadeOut)
	fadeInSize_ = {static_cast<float>(window->GetWindowWidth()), static_cast<float>(window->GetWindowHeight())};
	fade_ = make_unique<BaseSprites>();
	fade_->Initialize(white1x1_tex.number);
	fade_->SetColor(fadeColor_);
	fade_->SetSize({fadeInSize_});

	judgeLoca_ = make_unique<JudgeLocation>();
	judgeLoca_->Initialize();

}

void BaseBattleScene::Object3DUpdate()
{
	//プレイヤー
	//小節終了時に入力可能状態に変更
	if(rhythmManager_->GetIsMeasureUp()){
		player_->InputPossible();
	}
	if(player_->GetIsInput()){
		rhythmManager_->InputTime();
		isRhythmInput_ = true;
		isNoteInput_ = true;
	}
	if(player_->GetIsDamage())	{
		gameManager_->AudioPlay(damage_audio.number,damage_audio.volume);
		gameManager_->HpDecrement();
	}
	if(player_->GetIsDead())	{
		gameManager_->AudioPlay(damage_audio.number,damage_audio.volume);
		isGameEnd_ = true;
	}
	player_->Update(camera);
	//player->SetMoveEasingMaxTime(static_cast<float>(rhythmManager->GetBPMTimeSub()));
	//出口
	exit_->Update(camera);
	{
		Vector3 target = player_->GetPosition() + Vector3{-1, 2, 0};
		Vector2 pos = exit_->GetCoinSp()->ChangeTransformation(target, this->camera);
		exit_->SetCoinSpPosition(pos);
	}
}

void BaseBattleScene::Object2DUpdate()
{
	gameManager_->SpriteUpdate();

	judgeLoca_->Update(isNoteInput_);
}

void BaseBattleScene::CommonUpdate()
{
	gameManager_->PlayerCircleShadowSet(player_->GetPosition());
	//地面
	gameManager_->LightUpdate(player_->GetIsDead());

	//出口
	if(gameManager_->GetCoinNum() >= exit_->GetExitNeedCoinNum() && exit_->GetIsPlayerContact()){
		exit_->ExitOpen();
		player_->SetIsExitOpen(true);
	}
	else if(!exit_->GetIsPlayerContact()){
		exit_->ExitClose();
		player_->SetIsExitOpen(false);
	}

	//カメラ追従
	camera->Tracking(player_->GetPosition());

	//シーン遷移
	if(player_->GetIsNextScene())	{
		isNextSceneChange_ = true;
		if(!exit_->GetIsOpenAudioOnce()){
			gameManager_->AudioPlay(openExit_audio.number, openExit_audio.volume);
			camera->ShakeStart();
			exit_->ModelChange();
		}
	}

	//すべての衝突をチェック
	collisionManager_->CheckAllCollisions();
}

void BaseBattleScene::RhythmJudgeUpdate()
{
	if(isPrevSceneChange_) return;

	//リズム判別
	if(isRhythmInput_){
		isRhythmInput_ = false;

		if(rhythmManager_->IsInputRhythmJudge()){
			gameManager_->ComboIncrement();
			player_->SetInputJudge(true);
		}
		else if(!rhythmManager_->IsInputRhythmJudge()){
			gameManager_->ComboReset();
			player_->SetInputJudge(false);
		}
	}
}

void BaseBattleScene::BeatEndUpdate()
{
	if(isPrevSceneChange_) return;

	//リズム終了時処理
	if(rhythmManager_->GetIsJustRhythm() && !isGameEnd_){
		
		//SE
		gameManager_->AudioPlay(rhythm_audio.number, rhythm_audio.volume);

		//各オブジェクト処理
		if(!player_->GetIsDead())player_->IsBeatEndOn();

		areaManager_->BeatEndUpdate(gameManager_.get());

		exit_->IsBeatEndOn();
		gameManager_->IsBeatEndOn();
		

		AddBeatEndUpdate();
	}
}

void BaseBattleScene::Object3DDraw()
{
	player_->Draw();
	exit_->Draw();
}

void BaseBattleScene::ParticleDraw()
{
	areaManager_->ParticleDraw();
}

void BaseBattleScene::UIDraw()
{
	//出口
	exit_->Draw2D();

	judgeLoca_->Draw();

	gameManager_->SpriteDraw();

	//シーン遷移
	fade_->Draw();
}

void BaseBattleScene::SceneGameEnd()
{
	sceneManager->SetNextScene(new TitleScene(dxCommon,window));
}

void BaseBattleScene::SceneChange()
{
	//PrevSceneからの移動後処理
	if(isPrevSceneChange_){

		//画面が開く
		{
			if(fadeColor_.w <= 0){
				isPrevSceneChange_ = false;
				fadeCurrentFrame_ = 0;
				//リズム
				rhythmManager_->TimeStart();
				gameManager_->AudioPlay(bpm120Game_audio.number, bpm120Game_audio.volume, true);
				return;
			}

			fadeColor_.w = 
				Easing_Linear_Point2(1,0,Time_OneWay(fadeCurrentFrame_, FadeSecond/2));
			fade_->SetColor(fadeColor_);
			fade_->Update();
		}
	}
	//NextSceneへの移動
	else if(isNextSceneChange_ || isGameEnd_){

		if(fadeColor_.w >= 1){
			camera->Reset();
			if(isNextSceneChange_)NextSceneChange();
			else if(isGameEnd_)	SceneGameEnd();
		}

		fadeColor_.w = 
			Easing_Linear_Point2(0,1,Time_OneWay(fadeCurrentFrame_, FadeSecond));
		fade_->SetColor(fadeColor_);
		fade_->Update();
	}
}

void BaseBattleScene::ObjectFinaize()
{
#pragma region _3D解放
	player_->Finalize();
	exit_->Finalize();
#pragma endregion _3D解放

#pragma region _2D解放
	judgeLoca_->Finalize();
	fade_->Finalize();
#pragma endregion _2D解放
}

void BaseBattleScene::CommonFinalize()
{
	gameManager_->Finalize();

	rhythmManager_ = nullptr;
}

