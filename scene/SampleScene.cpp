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

#pragma region �ėp������
	//���C�g
	lightGroup = LightGroup::Create();
	//�F�ݒ�
	lightGroup->SetAmbientColor({1,1,1});
	//3D�I�u�W�F�N�g(.obj)�ɃZ�b�g
	ObjModelObject::SetLight(lightGroup);

	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, true);

	//�ۉe
	lightGroup->SetCircleShadowActive(0, true);

	//�Փ˃}�l�[�W���[
	collisionManager = CollisionManager::GetInstance();

	//�p�[�e�B�N��
	particle = ParticleManager::GetInstance();
	particleWorld.Initialize();

	//�I�[�f�B�I
	audio = new Audio();
	audio->Initialize();
	audio->LoadWave(0, "Resources/sound/rhythm.wav");
	audio->LoadWave(1, "Resources/sound/miss.wav");
	audio->LoadWave(2, "Resources/sound/kari01.wav");

	//���Y���}�l�[�W���[
	rhythmManager = new RhythmManager();

	camera->SetEye(Vector3(0.f, 9.f, -18.f));

#pragma endregion �ėp������

#pragma region _3D������

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

#pragma endregion _3D������

#pragma region _2D������

#pragma endregion _2D������

	//���Y��
	rhythmManager->InitializeMeasurement(clock());
}

void SampleScene::Update()
{
	//���Y���v��
	rhythmManager->StartMeasurement(clock());
	//�v���J�n��
	if(rhythmManager->GetTimer() == 0){
		audio->PlayWave(2, 0.15f, true);
	}
	//���Y���J��オ��
	rhythmManager->BeatMoveUp();

	BaseScene::Update();

#pragma region ���͏���

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


	//�G�o��
	if(input->Trigger(DIK_SPACE)){
		EnemyPop({0,-4,12.5}, {-1,0,0});
	}
#endif // _DEBUG

	if(player->GetIsMovement()){
		rhythmManager->InputRhythm();
		IsRhythmInput = true;
	}

#pragma endregion ���͏���

	//���Y������
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

	//���Y���I��������
	if(rhythmManager->GetIsRhythmEnd()){
		
		//SE
		audio->PlayWave(0,0.5f);

		//�e�I�u�W�F�N�g����
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

#pragma region _3D�X�V
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
#pragma endregion _3D�X�V

#pragma region _2D�X�V

#pragma endregion _2D�X�V

#pragma region �ėp�X�V
	{
		//�ۉe
		lightGroup->SetCircleShadowDir(0, DirectX::XMVECTOR({circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0}));
		lightGroup->SetCircleShadowAtten(0, Vector3(circleShadowAtten[0], circleShadowAtten[1], circleShadowAtten[2]));
		lightGroup->SetCircleShadowFactorAngle(0, Vector2(circleShadowFactorAngle[0], circleShadowFactorAngle[1]));

		//�v���C���[�A�ۉe���W
		lightGroup->SetCircleShadowCasterPos(0, player->GetPosition());
	}
	lightGroup->Update();


	//���ׂĂ̏Փ˂��`�F�b�N
	collisionManager->CheckAllCollisions();

	//�p�[�e�B�N��
	particle->Update(particleWorld, camera);
	particleWorld.UpdateMatrix();

#pragma endregion �ėp�X�V

	BaseScene::EndUpdate();
}

void SampleScene::Draw()
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

#pragma region �p�[�e�B�N��
	particle->Draw();
#pragma endregion �p�[�e�B�N��

#pragma endregion _3D�`��

#pragma region _2D_UI�`��
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
#pragma endregion _2D_UI�`��

}

void SampleScene::Finalize()
{

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
#pragma endregion _3D���

#pragma region _2D���

#pragma endregion _2D���

#pragma region �ėp���

	delete rhythmManager;
	rhythmManager = nullptr;

	delete audio;
	audio=nullptr;

	delete lightGroup;
	lightGroup = nullptr;

	particleWorld = {};

#pragma endregion �ėp���

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
