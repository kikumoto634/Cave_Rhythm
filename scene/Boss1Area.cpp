#include "Boss1Area.h"
#include "HomeScene.h"
#include "SceneManager.h"

#include "../Engine/math/Easing/Easing.h"

using namespace std;

Boss1Area::~Boss1Area()
{
}

Boss1Area::Boss1Area(DirectXCommon *dxCommon, Window *window, int saveHP)
	:BaseBattleScene(
		dxCommon,
		window)
{
}

void Boss1Area::SceneGameEnd()
{
	sceneManager->SetNextScene(new HomeScene(dxCommon,window));
}

void Boss1Area::NextSceneChange()
{
	sceneManager->SetNextScene(new HomeScene(dxCommon,window));
}

void Boss1Area::SceneChange()
{
	//PrevScene����̈ړ��㏈��
	if(IsPrevSceneChange){

		//��ʂ��J��
		{
			if(fadeColor.w <= 0){
				IsPrevSceneChange = false;
				fadeCurrentFrame = 0;
				//���Y��
				rhythmManager->InitializeMeasurement(clock());
				return;
			}

			fadeColor.w = 
				Easing_Linear_Point2(1,0,Time_OneWay(fadeCurrentFrame, FadeSecond));
			fade->SetColor(fadeColor);
			fade->Update();
		}
	}
	//NextScene�ւ̈ړ�
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

void Boss1Area::AddCommonInitialize()
{
	//�_���W����
	areaManager->CSVAreaInitialize("Boss1");
}

void Boss1Area::AddObject3DInitialize()
{
	//�{�X
	boss = make_unique<Boss1>();
	boss->Initialize("Skeleton");
	Vector3 lpos = areaManager->GetCSVObjectPopPosition(0);
	boss->Pop({lpos.x, -3.f,lpos.z});


	IndestructibleWallModel = new ObjModelManager();
	IndestructibleWallModel->CreateModel("GroundBlock3");
	IndestructibleWallColliderModel = new ObjModelManager();
	IndestructibleWallColliderModel->CreateModel("GroundBlock2_Collider");
	
	//����
	for(int i = 0; i < 3; i++){
		enterWall[i] = make_unique<IndestructibleWall>();
		enterWall[i]->Initialize(IndestructibleWallModel, IndestructibleWallColliderModel);
		enterWall[i]->SetPosition({enterWallBasePos.x + Block_Size*i, enterWallBasePos.y, enterWallBasePos.z});
	}

	//�o��
	for(int i = 0; i < 3; i++){
		exitWall[i] = make_unique<IndestructibleWall>();
		exitWall[i]->Initialize(IndestructibleWallModel, IndestructibleWallColliderModel);
		exitWall[i]->SetPosition({exitWallBasePos.x + Block_Size*i, exitWallBasePos.y, exitWallBasePos.z});
	}
}

void Boss1Area::AddObject2DInitialize()
{
	cutinInitialize();
}

void Boss1Area::AddCommonUpdate()
{
	areaManager->CSVAreaUpdate(camera, player->GetPosition());

	if(IsBossStart && !IsCutInHide){
		if(input->Trigger(DIK_Z) || input->Trigger(DIK_DOWN) || input->Trigger(DIK_UP) || input->Trigger(DIK_LEFT) || input->Trigger(DIK_RIGHT)){
			IsCutInMoveEnd = true;
			IsBossStart = false;
		}
	}
}

void Boss1Area::AddObject3DUpdate()
{
	//�{�X
	if(IsEnterClose){
		if(boss->GetIsDeadAudio()){
			gameManager->AudioPlay(2, 1.5f);
		}
		else if(boss->GetIsDead()){
			IsExitOpen = true;
			gameManager->AudioPlay(11, 0.5f);
		}
		boss->Update(camera,player->GetPosition());
	}

	//����
	{
		if(IsEnterClose){
			for(int i = 0; i < 3; i++){
				enterWall[i]->Update(this->camera);
				enterWall[i]->SetPlayerPos(player->GetPosition());
			}
		}
		else if(!IsEnterClose){
			for(int i = 0; i < 3; i++){
				enterWall[i]->ColliderRemove();
			}
			//�ꏊ�ɉ����ĕ���
			if(player->GetPosition().z >= enterBorderLineZ){
				IsEnterClose = true;
				camera->ShakeStart();
				gameManager->AudioPlay(11);
			}
		}
	}

	//�o��
	{
		if(IsExitOpen){
			for(int i = 0; i < 3; i++){
				exitWall[i]->ColliderRemove();
			}
		}
		else if(!IsExitOpen){
			for(int i = 0; i < 3; i++){
				exitWall[i]->Update(this->camera);
				exitWall[i]->SetPlayerPos(player->GetPosition());
			}
		}
	}
	exit->SetExitOpenNeedCoin(0);
	exit->NeedCoinSpriteUpdate();
}

void Boss1Area::AddObject2DUpdate()
{
	cutinUpdate();
}

void Boss1Area::AddBeatEndUpdate()
{
	if(!IsEnterClose) return;
	boss->IsBeatEndOn();
}

void Boss1Area::AddObject3DDraw()
{
	areaManager->CSVAreaDraw();

	if(!IsExitOpen){
		for(int i = 0; i < 3; i++){
			exitWall[i]->Draw();
		}
	}

	if(!IsEnterClose) return;
	//�{�X
	boss->Draw();
	//����
	for(int i = 0; i < 3; i++){
		enterWall[i]->Draw();
	}
}

void Boss1Area::AddParticleDraw()
{
	if(!IsEnterClose) return;
	boss->ParticleDraw();
}

void Boss1Area::AddUIDraw()
{
	cutinDraw();
}

void Boss1Area::AddObjectFinalize()
{
	//�{�X
	boss->Finalize();

	delete IndestructibleWallModel;
	delete IndestructibleWallColliderModel;
	//����
	for(int i = 0; i < 3; i++){
		enterWall[i]->Finalize();
	}

	//�o��
	for(int i = 0; i < 3; i++){
		exitWall[i]->Finalize();
	}

	//�J�b�g�C��Sp
	cutinFinalize();
}

void Boss1Area::AddCommonFinalize()
{
	areaManager->CSVAreaFinalize();
	delete areaManager;
	areaManager = nullptr;
}

#pragma region �J�b�g�C��
void Boss1Area::cutinInitialize()
{
	{
		cutInSpMain = make_unique<BaseSprites>();
		cutInSpMain->Initialize(28);
		cutInSpMain->SetPosition(cutInPos);
		cutInSpMain->SetAnchorPoint({0.5,0.5});
		cutInSpMain->SetSize({1280, 360});
	}
	{
		cutInSpPart1 = make_unique<BaseSprites>();
		cutInSpPart1->Initialize(29);
		cutInSpPart1->SetPosition(cutInPartPos1);
		cutInSpPart1->SetAnchorPoint({0.5,0.5});
		cutInSpPart1->SetSize({800, 180});
	}
	{
		cutInSpPart2 = make_unique<BaseSprites>();
		cutInSpPart2->Initialize(30);
		cutInSpPart2->SetPosition(cutInPartPos2);
		cutInSpPart2->SetAnchorPoint({0.5,0.5});
		cutInSpPart2->SetSize({800, 180});
	}
	{
		bossName = make_unique<BaseSprites>();
		bossName->Initialize(20);
		bossName->SetPosition(bossNamePos);
		bossName->SetAnchorPoint({0.5f,0.5f});
		bossName->SetSize({650, 150});
	}

	gameManager->AudioPlay(12, 1.5f);
}

void Boss1Area::cutinUpdate()
{
	if(!IsCutInHide)
	{
		if(IsCutInMoveStart){
			cutInPos = Easing_Linear_Point2({1920,360},{640,360},Time_OneWay(cutInMoveFrameCur, cutinSecond));
			cutInPartPos1 = Easing_Linear_Point2({1680,600},{880,600},Time_OneWay(cutInMoveFrameCur, cutinSecond));
			cutInPartPos2 = Easing_Linear_Point2({-400,120},{380,120},Time_OneWay(cutInMoveFrameCur, cutinSecond));
			bossNamePos = Easing_Linear_Point2({1605,610},{950,610},Time_OneWay(cutInMoveFrameCur, cutinSecond));
	
			if(cutInMoveFrameCur >= 1.0f){
				cutInPos = {640,360};
				cutInPartPos1 = {880, 600};
				cutInPartPos2 = {380, 120};
				bossNamePos = {950,610};
				cutInMoveFrameCur = 0.f;
				IsCutInMoveStart = false;
				IsBossStart = true;
			}
			cutInSpMain->SetPosition(cutInPos);
			cutInSpPart1->SetPosition(cutInPartPos1);
			cutInSpPart2->SetPosition(cutInPartPos2);
			bossName->SetPosition(bossNamePos);
		}
		else if(IsCutInMoveEnd){
			cutInPos = Easing_Linear_Point2({640,360},{1920,360},Time_OneWay(cutInMoveFrameCur, cutinSecond));
			cutInPartPos1 = Easing_Linear_Point2({800,600},{1680,600},Time_OneWay(cutInMoveFrameCur, cutinSecond));
			cutInPartPos2 = Easing_Linear_Point2({380,120},{-400,120},Time_OneWay(cutInMoveFrameCur, cutinSecond));
			bossNamePos = Easing_Linear_Point2({950,610},{1605,610},Time_OneWay(cutInMoveFrameCur, cutinSecond));

			if(cutInMoveFrameCur >= 1.0f){
				cutInPos = {1920,360};
				cutInPartPos1 = {1680, 600};
				cutInPartPos2 = {-400, 120};
				bossNamePos = {1605,610};
				cutInMoveFrameCur = 0.f;
				IsCutInMoveEnd = false;
				IsCutInHide = true;
			}
			cutInSpMain->SetPosition(cutInPos);
			cutInSpPart1->SetPosition(cutInPartPos1);
			cutInSpPart2->SetPosition(cutInPartPos2);
			bossName->SetPosition(bossNamePos);
		}

		cutInSpMain->Update();
		cutInSpPart1->Update();
		cutInSpPart2->Update();
		bossName->Update();
	}
}

void Boss1Area::cutinDraw()
{
	if(IsCutInHide) return;
	cutInSpMain->Draw();
	cutInSpPart1->Draw();
	cutInSpPart2->Draw();
	bossName->Draw();
}

void Boss1Area::cutinFinalize()
{
	cutInSpMain->Finalize();
	cutInSpPart1->Finalize();
	cutInSpPart2->Finalize();
	bossName->Finalize();
}
#pragma endregion