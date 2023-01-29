#include "DebugScene.h"

#include "../Engine/math//Easing/Easing.h"

#include "../Game/Collision/CollisionSystem/Collision.h"
#include "../Game/Collision/SphereCollider.h"
#include "../Game/Collision/MeshCollider.h"

#include <sstream>
#include <iomanip>

#include "SceneManager.h"
#include "GameScene.h"
#include "TitleScene.h"

#include "../Engine/math/Easing/Easing.h"

using namespace std;
using namespace DirectX;

const float DebugScene::Plane_Size = 2.5f;

DebugScene::DebugScene(DirectXCommon *dxCommon, Window *window)
		: BaseScene(
		dxCommon,
		window)
{
}

void DebugScene::Application()
{
	BaseScene::Application();
}

void DebugScene::Initialize()
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

	//�J����
	camera->SetTarget(Vector3(0.f, 2.f, -14.f));
	camera->SetEye(Vector3(0.f, 12.f, -31.f));

#pragma endregion �ėp������

#pragma region _3D������

	//blender�ł̕ۑ��X�P�[���� 2/10(0.2)�ł̃G�N�X�|�[�g
	player = make_unique<Player>();
	player->Initialize("human1");
	player->SetPosition({0, -3.f, -2.5f});
	player->SetWeaponPos({0,0,-2.5f});
	player->SetRotation({0, DirectX::XMConvertToRadians(180),0.f});
	gameManager->InitializeSetHp(player->GetHP());


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

	coin = make_unique<Coins>();
	coin->Initialize("Coins");

	//�o��
	exit = make_unique<Exit>();
	exit->SetExitOpenNeedCoin(1);
	exit->Initialize("Exit");
	exit->SetPosition({0,-5,-12.5f});

	dummy = make_unique<TrainingDummy>();
	dummy->Initialize("Dummy");

	enemy = make_unique<Enemy>();
	enemy->Initialize("Slime");
	enemy2 = make_unique<Enemy2>();
	enemy2->Initialize("Skeleton");

#pragma endregion _3D������

	//�V�[���J��(FadeOut)
	fadeInSize = {static_cast<float>(window->GetWindowWidth()), static_cast<float>(window->GetWindowHeight())};
	fade = make_unique<BaseSprites>();
	fade->Initialize(1);
	fade->SetColor(fadeColor);
	fade->SetSize({fadeInSize});

#pragma region _2D������
	
#pragma endregion
}

void DebugScene::Update()
{
	//�v���J�n
	if(!IsPrevSceneChange){
		//���Y���v��
		rhythmManager->StartMeasurement(clock());
		//�v���J�n��
		if(rhythmManager->GetMoveUpNumber() == 0){
			gameManager->AudioPlay(8, 0.5f, true);
		}
		//���Y���J��オ��
		rhythmManager->BeatMoveUp();
	}

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

	if(input->Push(DIK_Q)){
		camera->MoveVector({0.f, 1.f, 0.f});
	}
	else if(input->Push(DIK_E)){
		camera->MoveVector({0.f, -1.f, 0.f});
	}

#endif // _DEBUG

	//�V�[���X�V
	SceneChange();

	if(!IsPrevSceneChange){

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
		if(rhythmManager->GetIsRhythmEnd() && !IsGameEnd){
		
			//SE
			gameManager->AudioPlay(0,0.25f);

			//�e�I�u�W�F�N�g����
			if(!player->GetIsDead())player->IsBeatEndOn();

			IsComboColorChange = !IsComboColorChange;
			for(int i = 0; i < DIV_NUM; i++){
				for(int j = 0; j < DIV_NUM; j++){
					//�R���{���ɉ����ĐF�ω�
					if(gameManager->GetComboNum() >= gameManager->GetPlaneColorChangeCombo()){
						
						int changePos = i%2+j%2;
						if(changePos == 0 || changePos == 2)		{plane[i][j]->PlaneColorChange(true, IsComboColorChange);}
						if(changePos == 1)							{plane[i][j]->PlaneColorChange(false, IsComboColorChange);}
					}
					else if(gameManager->GetComboNum() < 10){
						plane[i][j]->PlaneColorReset();
					}

					plane[i][j]->IsBeatEndOn();
				}
			}

			exit->IsBeatEndOn();

			if(coin->GetIsAlive())	coin->IsBeatEndOn();

			gameManager->IsBeatEndOn();

			dummy->IsBeatEndOn();
			enemy->IsBeatEndOn();
			enemy2->IsBeatEndOn();
		}
	}

#pragma region _3D�X�V

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

	//�o��
	exit->Update(camera);
	{
		Vector3 target = player->GetPosition() + Vector3{-1, 2, 0};
		Vector2 pos = exit->ChangeTransformation(target);
		exit->SetCoinSpPosition(pos);
	}

	//�_�~�[
	if(dummy->GetIsDeadAudioOnce())	{
		gameManager->AudioPlay(2,0.2f);
		if(coin->PopPossible()){
			coin->Pop({dummy->GetDeadParticlePos().x, -5 , dummy->GetDeadParticlePos().z});
		}
	}
	dummy->Update(camera);

	//�G�l�~�[
	if(!enemy->GetIsNotApp()){
		if(enemy->GetIsDeadAudioOnce())	{
			gameManager->AudioPlay(2,0.2f);
			if(coin->PopPossible()){
				coin->Pop({enemy->GetDeadParticlePos().x, -5 , enemy->GetDeadParticlePos().z});
			}
		}
		enemy->Update(camera);
	}

	//�G�l�~�[2
	if(!enemy2->GetIsNotApp()){
		if(enemy2->GetIsDeadAudioOnce())	{
			gameManager->AudioPlay(2,0.2f);
			if(coin->PopPossible()){
				coin->Pop({enemy2->GetDeadParticlePos().x, -5 , enemy2->GetDeadParticlePos().z});
			}
		}
		enemy2->Update(camera, player->GetPosition());
	}


	if(coin->GetCoin()){
		gameManager->CoinIncrement();
		gameManager->AudioPlay(7,0.5f);
	}
	coin->Update(camera);

#pragma endregion _3D�X�V

#pragma region _2D�X�V

	gameManager->SpriteUpdate();

#pragma endregion

#pragma region �ėp�X�V	
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

		ImGui::Text("POP:pos dir");
		ImGui::InputInt2("EnemyPos X,Z : 0~11", popPosition);
		ImGui::InputInt2("EnemyDir X,Z : -1~1", popDirection);

		//�G�̏o��
		ImGui::Text("EnemyPOP");
		//����
		if (ImGui::Button("POP1")) {
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

		//�G2�̏o��
		ImGui::Text("Enemy2POP");
		//����
		if (ImGui::Button("POP2")) {
			//���W;
			Vector2 pos = {(-12.5f + (float)popPosition[0]*2.5f), (-12.5f + (float)popPosition[1]*2.5f)};
			//POP
			Enemy2Pop(pos);
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

void DebugScene::Draw()
{
	BaseScene::Draw();

#pragma region _3D�`��
	player->Draw();

	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			plane[i][j]->Draw();
		}
	}
	skydome->Draw();

	rock->Draw();

	exit->Draw();

	dummy->Draw();
	enemy->Draw();
	enemy2->Draw();

	coin->Draw();

#pragma region �p�[�e�B�N��
	dummy->ParticleDraw();
	enemy->ParticleDraw();
	enemy2->ParticleDraw();
	rock->ParticleDraw();
#pragma endregion �p�[�e�B�N��

#pragma endregion _3D�`��

#pragma region _2D_UI�`��
	Sprite::SetPipelineState();

	//�o��
	exit->Draw2D();

	dummy->Draw2D();

	gameManager->SpriteDraw();

	//�V�[���J��
	fade->Draw();

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
#pragma endregion _2D_UI�`��
}

void DebugScene::Finalize()
{
#pragma region _3D���
	player->Finalize();

	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			plane[i][j]->Finalize();
		}
	}

	skydome->Finalize();

	rock->Finalize();

	exit->Finalize();

	dummy->Finalize();
	enemy->Finalize();
	enemy2->Finalize();

	coin->Finalize();
#pragma endregion _3D���

#pragma region _2D���
	fade->Finalize();
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

void DebugScene::EnemyPop(Vector2 pos, Vector2 dir)
{
	Vector3 lpos = {pos.x, -3.5f, pos.y};
	Vector3 ldir = {dir.x, 0, dir.y};

	if(enemy->GetIsNotApp()){
		enemy->SetPopPoasition(lpos);
		enemy->SetDirection(ldir);
		enemy->BeginAppearance();
	}
}

void DebugScene::Enemy2Pop(Vector2 pos)
{
	Vector3 lpos = {pos.x, -3.5f, pos.y};

	if(enemy2->GetIsNotApp()){
		enemy2->SetPopPoasition(lpos);
		enemy2->BeginAppearance();
	}
}

void DebugScene::NextSceneChange()
{
	sceneManager->SetNextScene(new GameScene(dxCommon,window));
}

void DebugScene::SceneGameEnd()
{
	sceneManager->SetNextScene(new TitleScene(dxCommon,window));
}

void DebugScene::SceneChange()
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
