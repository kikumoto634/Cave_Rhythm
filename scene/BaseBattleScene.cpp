#include "BaseBattleScene.h"
#include "../Engine/math//Easing/Easing.h"

#include "../Game/Collision/CollisionSystem/Collision.h"
#include "../Game/Collision/SphereCollider.h"
#include "../Game/Collision/MeshCollider.h"

#include "SceneManager.h"
#include "TitleScene.h"

using namespace std;

BaseBattleScene::BaseBattleScene(DirectXCommon *dxCommon, Window *window, int saveHP)
	:BaseScene(
		dxCommon,
		window)
{
	this->saveHP = saveHP;
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
	//�v���ABGM�J�n
	RhythmMeasure();

	BaseScene::Update();

#pragma region ���͏���

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

	//�V�[���X�V
	SceneChange();

	//����
	InputUpdate();

	//���Y��
	RhythmJudgeUpdate();
	BeatEndUpdate();

	//�X�V
	Object3DUpdate();
	AddObject3DUpdate();

	Object2DUpdate();
	AddObject2DUpdate();

	//�V�[���A�J�����A�ėp
	CommonUpdate();
	AddCommonUpdate();

#ifdef _DEBUG
	{
		//���W
		ImGui::SetNextWindowPos(ImVec2{0,100});
		//�T�C�Y
		ImGui::SetNextWindowSize(ImVec2{300,150});
		ImGui::Begin("Debug");
		//�J���� ��]:false , �ړ�:true
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
		//���W
		ImGui::SetNextWindowPos(ImVec2{1000,40});
		//�T�C�Y
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

	UIDraw();
	AddUIDraw();

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
	//�Փ˃}�l�[�W���[
	collisionManager = CollisionManager::GetInstance();

	//���Y���}�l�[�W���[
	rhythmManager = new RhythmManager();

	//�Q�[���}�l�[�W���[
	gameManager = new GameManager();
	gameManager->Initialize();

	areaManager = make_unique<AreaManager>();
	AreaManagerInitialize();

	//�J����
	camera->SetTarget(Vector3(0.f, 2.f, -3.f));
	camera->RotVector({XMConvertToRadians(-60.f), 0.f, 0.f});
	camera->Update();
}

void BaseBattleScene::Object3DInitialize()
{
	//blender�ł̕ۑ��X�P�[���� 2/10(0.2)�ł̃G�N�X�|�[�g
	player = make_unique<Player>();
	player->Initialize("human1");
	player->SetPosition(areaManager->GetPlayerPosition());
	player->SetCameeraInitPos(areaManager->GetPlayerPosition());
	player->SetWeaponPos({0,0,-2.f});
	player->SetRotation({0, DirectX::XMConvertToRadians(180),0.f});
	player->SetHp(saveHP);
	gameManager->InitializeSetHp(player->GetHP());

	//�o��
	exit = make_unique<Exit>();
	exit->SetExitOpenNeedCoin(1);
	exit->Initialize("Exit");
	exit->SetPosition(areaManager->GetExitPosition());
}

void BaseBattleScene::Object2DInitialize()
{
	//�V�[���J��(FadeOut)
	fadeInSize = {static_cast<float>(window->GetWindowWidth()), static_cast<float>(window->GetWindowHeight())};
	fade = make_unique<BaseSprites>();
	fade->Initialize(1);
	fade->SetColor(fadeColor);
	fade->SetSize({fadeInSize});
}

void BaseBattleScene::InputUpdate()
{
	if(IsPrevSceneChange)return;

	//ToDo: 
	// ���������ł̃R���{+2�̏C��(��x�����ɂȂ����玟�̓��͉\���Ԃ܂œ��͕s�B)
	// �~�X���͂̎��A�v���C���[�̈ړ���s�ɁB
	if(player->GetIsInputOnce()){
		rhythmManager->InputRhythm();
		IsRhythmInput = true;
	}
}

void BaseBattleScene::Object3DUpdate()
{
	//�v���C���[
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
	//�o��
	exit->Update(camera);
	{
		Vector3 target = player->GetPosition() + Vector3{-1, 2, 0};
		Vector2 pos = exit->ChangeTransformation(target);
		exit->SetCoinSpPosition(pos);
	}
}

void BaseBattleScene::Object2DUpdate()
{
	gameManager->SpriteUpdate();
}

void BaseBattleScene::CommonUpdate()
{
	gameManager->PlayerCircleShadowSet(player->GetPosition());
	//�n��
	areaManager->Update(this->camera, player->GetPosition());
	gameManager->LightUpdate(player->GetIsDead());

	//�o��
	if(gameManager->GetCoinNum() >= exit->GetExitNeedCoinNum() && exit->GetIsPlayerContact()){
		exit->ExitOpen();
		player->SetIsExitOpen(true);
	}
	else if(!exit->GetIsPlayerContact()){
		exit->ExitClose();
		player->SetIsExitOpen(false);
	}

	//�V�[���J��
	if(player->GetIsNextScene())	{
		IsNextSceneChange = true;
		if(!exit->GetIsOpenAudioOnce()){
			gameManager->AudioPlay(6, 0.5f);
			camera->ShakeStart();
			player->SetIsWait(true);
			exit->ModelChange();
		}
	}

	//���ׂĂ̏Փ˂��`�F�b�N
	collisionManager->CheckAllCollisions();
}

void BaseBattleScene::RhythmMeasure()
{
	//�v���J�n
	if(IsPrevSceneChange) return;

	//���Y���v��
	rhythmManager->StartMeasurement(clock());
	//�v���J�n��
	if(rhythmManager->GetMoveUpNumber() == 0 && IsBGMStart){
		gameManager->AudioPlay(8, 0.5f, true);
	}
	//���Y���J��オ��
	rhythmManager->BeatMoveUp();
}

void BaseBattleScene::RhythmJudgeUpdate()
{
	if(IsPrevSceneChange) return;

	//���Y������
	if(IsRhythmInput){
		IsRhythmInput = false;

		//High(���͂��x���AjudgeTime���X�V���ꂽ��Ԃł̍X�V)
		if(rhythmManager->HighJudgeRhythm()){
			gameManager->ComboIncrement();
			player->JudgeUpdate(true);
		}
		//Low(���͂������āAJudgeTime���X�V����Ă��Ȃ������̂ݒʂ��@�J��オ��p�m�F�����Ƃ̔�r) judgeTime���X�V�����܂ŏ����҂�
		else if(rhythmManager->GetMoveUpNumber() > rhythmManager->GetJudgeTimeBase()){
			if(rhythmManager->LowJudgeRhythm()){
				gameManager->ComboIncrement();
				player->JudgeUpdate(true);
			}
			//�~�X
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

	//���Y���I��������
	if(rhythmManager->GetIsRhythmEnd() && !IsGameEnd){
		
		//SE
		gameManager->AudioPlay(0,0.25f);

		//�e�I�u�W�F�N�g����
		if(!player->GetIsDead())player->IsBeatEndOn();

		areaManager->BeatEndUpdate(gameManager);

		exit->IsBeatEndOn();
		gameManager->IsBeatEndOn();

		AddBeatEndUpdate();
	}
}

void BaseBattleScene::Object3DDraw()
{
	player->Draw();

	areaManager->Draw();

	exit->Draw();
}

void BaseBattleScene::ParticleDraw()
{
	areaManager->ParticleDraw();
}

void BaseBattleScene::UIDraw()
{
	Sprite::SetPipelineState();

	//�o��
	exit->Draw2D();

	gameManager->SpriteDraw();

	//�V�[���J��
	fade->Draw();
}

void BaseBattleScene::SceneGameEnd()
{
	sceneManager->SetNextScene(new TitleScene(dxCommon,window));
}

void BaseBattleScene::SceneChange()
{
	//PrevScene����̈ړ��㏈��
	if(IsPrevSceneChange){
		if(fadeColor.w <= 0){
			IsPrevSceneChange = false;
			fadeCurrentFrame = 0;
			//���Y��
			rhythmManager->InitializeMeasurement(clock());
			return;
		}

		fadeColor.w = 
			Easing_Linear_Point2(1,0,Time_OneWay(fadeCurrentFrame, FadeSecond/2));
		fade->SetColor(fadeColor);
		fade->Update();
	}
	//NextScene�ւ̈ړ�
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

void BaseBattleScene::ObjectFinaize()
{
	#pragma region _3D���
	player->Finalize();
	areaManager->Finalize();
	exit->Finalize();
#pragma endregion _3D���

#pragma region _2D���
	fade->Finalize();
#pragma endregion _2D���
}

void BaseBattleScene::CommonFinalize()
{
	gameManager->Finalize();
	delete gameManager;
	gameManager = nullptr;

	delete rhythmManager;
	rhythmManager = nullptr;
}


void BaseBattleScene::AreaManagerInitialize()
{
	areaManager->RandamAreaInitialize();
}
