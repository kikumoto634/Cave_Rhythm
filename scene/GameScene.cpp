#include "GameScene.h"

#include "../Engine/math//Easing/Easing.h"

#include "../Game/Collision/CollisionSystem/Collision.h"
#include "../Game/Collision/SphereCollider.h"
#include "../Game/Collision/MeshCollider.h"

#include <sstream>
#include <iomanip>

#include "SceneManager.h"
#include "TitleScene.h"

using namespace std;
using namespace DirectX;

const float GameScene::Plane_Size = 2.5f;

GameScene::GameScene(DirectXCommon *dxCommon, Window *window)
		: BaseScene(
		dxCommon,
		window)
{
}

void GameScene::Application()
{
	BaseScene::Application();
}

void GameScene::Initialize()
{
	BaseScene::Initialize();

#pragma region �ėp������
	//�Փ˃}�l�[�W���[
	collisionManager = CollisionManager::GetInstance();

	//���Y���}�l�[�W���[
	rhythmManager = new RhythmManager();

	//�Q�[���}�l�[�W���[
	gameManager = new GameManager();
	gameManager->Initialize();

	//�I�[�f�B�I
	gameManager->AudioAdd(0,"rhythm.wav");
	gameManager->AudioAdd(1,"miss.wav");
	gameManager->AudioAdd(2,"damage.wav");

	gameManager->AudioAdd(3,"ex)_BPM90.wav");
	gameManager->AudioAdd(4,"ex)_BPM120.wav");
	gameManager->AudioAdd(5,"ex)_BPM180.wav");

	//�J����
	camera->SetTarget(Vector3(0.f, 0.f, -12.f));
	camera->SetEye(Vector3(0.f, 9.f, -30.f));

#pragma endregion �ėp������

#pragma region _3D������

	//blender�ł̕ۑ��X�P�[���� 2/10(0.2)�ł̃G�N�X�|�[�g
	player = make_unique<Player>();
	player->Initialize("human1");
	player->SetPosition({0, -3.f, -12.5f});

	for(int i = 0; i < IniCreateEnemyNum; i++){
		EnemyInitPop();
	}

	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			plane[i][j] = make_unique<Planes>();
			plane[i][j]->Initialize("GroundBlock");
			plane[i][j]->SetPosition({ float(-((DIV_NUM/2)*Plane_Size) + (i*Plane_Size)) ,-5 ,float(-((DIV_NUM/2)*Plane_Size) + (j*Plane_Size))});
		}
	}

	skydome = make_unique<SampleObjObject>();
	skydome->Initialize("skydome", true);

	rock = make_unique<AreaOutRock>();
	rock->Initialize("AreaRock", true);

#pragma endregion _3D������

#pragma region _2D������

#pragma endregion _2D������

	//���Y��
	rhythmManager->InitializeMeasurement(clock());

#ifdef _DEBUG
	dummy = make_unique<TrainingDummy>();
	dummy->Initialize("slime");
#endif // _DEBUG
}

void GameScene::Update()
{
	//���Y���v��
	rhythmManager->StartMeasurement(clock());
	//�v���J�n��
	if(rhythmManager->GetMoveUpNumber() == 0){
		gameManager->AudioPlay(3, 0.5f, true);
	}
	//���Y���J��オ��
	rhythmManager->BeatMoveUp();

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

	if(input->Push(DIK_W)){
		if(!IsCameraMovementChange)		camera->RotVector({XMConvertToRadians(-3.f), 0.f, 0.f});
		else if(IsCameraMovementChange)	camera->MoveVector({0.f, 0.f, 1.f});
	}
	else if(input->Push(DIK_S)){
		if(!IsCameraMovementChange)		camera->RotVector({XMConvertToRadians(3.f), 0.f, 0.f});
		else if(IsCameraMovementChange)	camera->MoveVector({0.f, 0.f, -1.f});
	}

	if(input->Trigger(DIK_RETURN)){
		sceneManager->SetNextScene(new TitleScene(dxCommon,window));
	}

#endif // _DEBUG

	//ToDo: 
	// ���������ł̃R���{+2�̏C��(��x�����ɂȂ����玟�̓��͉\���Ԃ܂œ��͕s�B)
	// �~�X���͂̎��A�v���C���[�̈ړ���s�ɁB
	if(player->GetIsInputOnce()){
		rhythmManager->InputRhythm();
		IsRhythmInput = true;
	}

#pragma endregion ���͏���

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

	//���Y���I��������
	if(rhythmManager->GetIsRhythmEnd()){
		
		//SE
		gameManager->AudioPlay(0,0.25f);

		//�e�I�u�W�F�N�g����
		player->IsBeatEndOn();
		for(auto it = enemy.begin(); it != enemy.end(); it++){
			(*it)->IsBeatEndOn();
		}
		for(int i = 0; i < DIV_NUM; i++){
			for(int j = 0; j < DIV_NUM; j++){
				plane[i][j]->IsBeatEndOn();
			}
		}

		//�G����
		for(int i = 0; i < gameManager->EnemyPopTurnCount(); i++){
			//���W;
			Vector2 lpos = gameManager->EnemyRandomPos(DIV_NUM, Plane_Size);
			//����
			Vector2 ldir = gameManager->EnemyRandomDir(lpos);
			//POP
			EnemyPop(lpos, ldir);
		}

		#ifdef _DEBUG
		dummy->IsBeatEndOn();
		#endif // _DEBUG

	}

#pragma region _3D�X�V
	//�G�l�~�[
	for(auto it = enemy.begin(); it != enemy.end(); it++){
		if(!(*it)->GetIsNotApp()){
			if((*it)->GetIsDeadAudioOnce())	gameManager->AudioPlay(2,0.2f);
			(*it)->Update(camera);
		}
	}
	//�v���C���[
	if(player->DamageSound())	gameManager->AudioPlay(2,0.2f);
	player->Update(camera);
	gameManager->PlayerCircleShadowSet(player->GetPosition());
	//�n��
	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			plane[i][j]->Update(camera);
		}
	}
	//�V��
	skydome->Update(camera);
	//��
	rock->Update(camera);

#ifdef _DEBUG
	if(dummy->GetIsDeadAudioOnce())	gameManager->AudioPlay(2,0.2f);
	dummy->Update(camera);
#endif // _DEBUG

#pragma endregion _3D�X�V

#pragma region _2D�X�V

#pragma endregion _2D�X�V

#pragma region �ėp�X�V	
	gameManager->LightUpdate();

	//���ׂĂ̏Փ˂��`�F�b�N
	collisionManager->CheckAllCollisions();
#pragma endregion �ėp�X�V

#ifdef _DEBUG
	{
		//���W
		ImGui::SetNextWindowPos(ImVec2{0,100});
		//�T�C�Y
		ImGui::SetNextWindowSize(ImVec2{300,300});
		ImGui::Begin("Debug");
		//�J���� ��]:false , �ړ�:true
		ImGui::Text("Camera");
		ImGui::Text("true = transform / false = rotation");
		ImGui::Checkbox("Change", &IsCameraMovementChange);
		if(ImGui::Button("Shake")){
			camera->ShakeStart();
		}

		//�G�̏o��
		ImGui::Text("EnemyPOP");
		ImGui::InputInt2("EnemyPos X,Z : 0~11", popPosition);
		ImGui::InputInt2("EnemyDir X,Z : -1~1", popDirection);
		//����
		if (ImGui::Button("POP")) {
			//���W;
			Vector2 pos = {(-12.5f + (float)popPosition[0]*2.5f), (-12.5f + (float)popPosition[1]*2.5f)};
			//����
			if(popDirection[0] >= 1)popDirection[0] = 1;
			else if(popDirection[0] <= -1)popDirection[0] = -1;
			if(popDirection[1] >= 1)popDirection[1] = 1;
			else if(popDirection[1] <= -1)popDirection[1] = -1;
			Vector2 dir = {(float)popDirection[0], (float)popDirection[1]};
			//POP
			EnemyPop(pos, dir);
		}
		ImGui::End();
	}
#endif // _DEBUG


	BaseScene::EndUpdate();
}

void GameScene::Draw()
{
	BaseScene::Draw();

#pragma region _2D_�w�i�`��
	
#pragma endregion _2D_�w�i�`��

#pragma region _3D�`��
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

	rock->Draw();

#pragma region �p�[�e�B�N��
	for(auto it = enemy.begin(); it != enemy.end(); it++){
		(*it)->ParticleDraw();
	}

	rock->ParticleDraw();
#pragma endregion �p�[�e�B�N��

#pragma endregion _3D�`��

#ifdef _DEBUG
	dummy->Draw();
#endif // _DEBUG

#pragma region _2D_UI�`��
	Sprite::SetPipelineState();

#ifdef _DEBUG
	debugText->Printf(0,0,1.f,"Camera Target  X:%f, Y:%f, Z:%f", camera->GetTarget().x, camera->GetTarget().y, camera->GetTarget().z);
	debugText->Printf(0,16,1.f,"Camera Eye  X:%f, Y:%f, Z:%f", camera->GetEye().x, camera->GetEye().y, camera->GetEye().z);

	debugText->Printf(0,48,1.f,"Player Pos X:%f, Y:%f, Z:%f", player->GetPosition().x, player->GetPosition().y,player->GetPosition().z);

	debugText->Printf(0,580, 1.f,  "Time				: %lf[ms]", rhythmManager->GetTimer());
	debugText->Printf(0, 600, 1.f, "JudgeTimeBase		: %lf[ms]", rhythmManager->GetJudgeTimeBase());
	debugText->Printf(0, 620, 1.f, "InputJudgeTimeBase	: %lf[ms]", rhythmManager->GetInputJudgeTime());
	
	debugText->Printf(200, 660, 1.f, "COMBO	: %d", gameManager->GetComboNum());


	debugText->Printf(0, 640, 1.f, "IsBeat : %d", rhythmManager->GetIsRhythmEnd());
	//debugText->Printf(0, 640, 1.f, "Combo : %d", combo);
	debugText->Printf(0, 660, 1.f, "HP : %d", player->GetHP());

	debugText->Printf(1000,0,1.0f, "EnemyNum : %d", enemy.size());

#endif // _DEBUG
	BaseScene::EndDraw();
#pragma endregion _2D_UI�`��
}

void GameScene::Finalize()
{
#ifdef _DEBUG
	dummy->Finalize();
#endif // _DEBUG

#pragma region _3D���
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

	rock->Finalize();
#pragma endregion _3D���

#pragma region _2D���

#pragma endregion _2D���

#pragma region �ėp���

	gameManager->Finalize();
	delete gameManager;
	gameManager = nullptr;

	delete rhythmManager;
	rhythmManager = nullptr;
#pragma endregion �ėp���

	BaseScene::Finalize();
}

void GameScene::EnemyInitPop()
{
	unique_ptr<Enemy> newObj = make_unique<Enemy>();
	newObj->Initialize("slime");
	enemy.push_back(move(newObj));
}

void GameScene::EnemyPop(Vector2 pos, Vector2 dir)
{
	Vector3 lpos = {pos.x, -3.5f, pos.y};
	Vector3 ldir = {dir.x, 0, dir.y};

	for(auto it = enemy.begin(); it != enemy.end(); it++){
		if((*it)->GetIsNotApp()){
			(*it)->SetPopPoasition(lpos);
			(*it)->SetDirection(ldir);
			(*it)->BeginAppearance();
			break;
		}
	}
}
