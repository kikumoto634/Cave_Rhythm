#include "Boss1Area.h"
#include "HomeScene.h"
#include "SceneManager.h"

#include "Easing.h"

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
	//PrevSceneからの移動後処理
	if(isPrevSceneChange_){

		//画面が開く
		{
			if(fadeColor_.w <= 0){
				isPrevSceneChange_ = false;
				fadeCurrentFrame_ = 0;
				//リズム
				rhythmManager_->TimeStart();
				return;
			}

			fadeColor_.w = 
				Easing_Linear_Point2(1,0,Time_OneWay(fadeCurrentFrame_, GameStartFadeSecond));
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
			Easing_Linear_Point2(0,1,Time_OneWay(fadeCurrentFrame_, GameStartFadeSecond));
		fade_->SetColor(fadeColor_);
		fade_->Update();
	}
}

void Boss1Area::AddCommonInitialize()
{
	//ダンジョン
	areaManager_->CSVAreaInitialize("Boss1");
}

void Boss1Area::AddObject3DInitialize()
{
	//ボス
	boss = make_unique<Boss1>();
	boss->Initialize("Skeleton");
	Vector3 lpos = areaManager_->GetCSVObjectPopPosition(0);
	boss->Pop({lpos.x, -3.f,lpos.z});

	ActorCreateInitialize();

	IndestructibleWallModel = new ObjModelManager();
	IndestructibleWallModel->CreateModel("GroundBlock3");
	IndestructibleWallColliderModel = new ObjModelManager();
	IndestructibleWallColliderModel->CreateModel("GroundBlock2_Collider");
	
	//入口
	for(int i = 0; i < 3; i++){
		enterWall[i] = make_unique<IndestructibleWall>();
		enterWall[i]->Initialize(IndestructibleWallModel, IndestructibleWallColliderModel);
		enterWall[i]->SetPosition({enterWallBasePos.x + Block_Size*i, enterWallBasePos.y, enterWallBasePos.z});
	}

	//出口
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
	areaManager_->CSVAreaUpdate(camera, player_->GetPosition());

	if(IsBossStart && !IsCutInHide){
		if(input->Trigger(DIK_Z) || input->Trigger(DIK_DOWN) || input->Trigger(DIK_UP) || input->Trigger(DIK_LEFT) || input->Trigger(DIK_RIGHT)){
			IsCutInMoveEnd = true;
			IsBossStart = false;
		}
	}
}

void Boss1Area::AddObject3DUpdate()
{
	//ボス
	if(IsEnterClose){
		if(boss->GetIsDeadAudio()){
			gameManager_->AudioPlay(damage_audio.number, damage_audio.volume);
		}
		else if(boss->GetIsDead() && !IsExitOpen){
			IsExitOpen = true;
			gameManager_->AudioPlay(gateEnter_audio.number, gateEnter_audio.volume);
		}
		boss->Update(camera,player_->GetPosition());
		ActorSummon();
		boss->ParticleUpdate();
	}

	//for(auto it = skelton.begin(); it != skelton.end(); it++){
	//	/*if((*it)->GetIsDeadAudio()){
	//		gameManager->AudioPlay(2, 0.5f);
	//	}
	//	(*it)->Update(camera,player->GetPosition());*/
	//}

	//入口
	{
		if(IsEnterClose){
			for(int i = 0; i < 3; i++){
				enterWall[i]->Update(this->camera);
				enterWall[i]->SetPlayerPos(player_->GetPosition());
			}
		}
		else if(!IsEnterClose){
			for(int i = 0; i < 3; i++){
				enterWall[i]->ColliderRemove();
			}
			//場所に応じて閉じる
			if(player_->GetPosition().z >= enterBorderLineZ){
				IsEnterClose = true;
				camera->ShakeStart();
				gameManager_->AudioPlay(gateEnter_audio.number);
			}
		}
	}

	//出口
	{
		if(IsExitOpen){
			for(int i = 0; i < 3; i++){
				exitWall[i]->ColliderRemove();
			}
		}
		else if(!IsExitOpen){
			for(int i = 0; i < 3; i++){
				exitWall[i]->Update(this->camera);
				exitWall[i]->SetPlayerPos(player_->GetPosition());
			}
		}
	}
	exit_->SetExitOpenNeedCoin(0);
	exit_->NeedCoinSpriteUpdate();
}

void Boss1Area::AddObject2DUpdate()
{
	cutinUpdate();
}

void Boss1Area::AddBeatEndUpdate()
{
	if(!IsEnterClose) return;
	boss->IsBeatEndOn();

	/*for(auto it = skelton.begin(); it != skelton.end(); it++){
		(*it)->IsBeatEndOn();
	}*/
}

void Boss1Area::AddObject3DDraw()
{
	areaManager_->CSVAreaDraw();

	if(!IsExitOpen){
		for(int i = 0; i < 3; i++){
			exitWall[i]->Draw();
		}
	}

	if(!IsEnterClose) return;
	//ボス
	boss->Draw();
	/*for(auto it = skelton.begin(); it != skelton.end(); it++){
		(*it)->Draw();
	}*/
	//入口
	for(int i = 0; i < 3; i++){
		enterWall[i]->Draw();
	}
}

void Boss1Area::AddParticleDraw()
{
	boss->ParticleDraw();
	/*for(auto it = skelton.begin(); it != skelton.end(); it++){
		(*it)->ParticleDraw();
	}*/
}

void Boss1Area::AddFrontUIDraw()
{
}

void Boss1Area::AddBackUIDraw()
{
	cutinDraw();
}

void Boss1Area::AddObjectFinalize()
{
	//ボス
	boss->Finalize();

	/*for(auto it = skelton.begin(); it != skelton.end(); it++){
		(*it)->Finalize();
	}*/

	delete IndestructibleWallModel;
	delete IndestructibleWallColliderModel;
	//入口
	for(int i = 0; i < 3; i++){
		enterWall[i]->Finalize();
	}

	//出口
	for(int i = 0; i < 3; i++){
		exitWall[i]->Finalize();
	}

	//カットインSp
	cutinFinalize();
}

void Boss1Area::AddCommonFinalize()
{
	areaManager_->CSVAreaFinalize();
}

#pragma region カットイン
void Boss1Area::cutinInitialize()
{
	{
		cutInSpMain = make_unique<BaseSprites>();
		cutInSpMain->Initialize(cutInBoss_tex.number);
		cutInSpMain->SetPosition(cutInPos);
		cutInSpMain->SetAnchorPoint({0.5,0.5});
		cutInSpMain->SetSize({1280, 360});
	}
	{
		cutInSpPart1 = make_unique<BaseSprites>();
		cutInSpPart1->Initialize(cutInParts1_tex.number);
		cutInSpPart1->SetPosition(cutInPartPos1);
		cutInSpPart1->SetAnchorPoint({0.5,0.5});
		cutInSpPart1->SetSize({800, 180});
	}
	{
		cutInSpPart2 = make_unique<BaseSprites>();
		cutInSpPart2->Initialize(cutInParts2_tex.number);
		cutInSpPart2->SetPosition(cutInPartPos2);
		cutInSpPart2->SetAnchorPoint({0.5,0.5});
		cutInSpPart2->SetSize({800, 180});
	}
	{
		bossName = make_unique<BaseSprites>();
		bossName->Initialize(bossName_tex.number);
		bossName->SetPosition(bossNamePos);
		bossName->SetAnchorPoint({0.5f,0.5f});
		bossName->SetSize({650, 150});
	}
}

void Boss1Area::cutinUpdate()
{
	if(!IsCutInHide)
	{
		//音声
		if(!IsCutInAudio){
			gameManager_->AudioPlay(reflected_audio.number, reflected_audio.volume);
			IsCutInAudio = true;
		}

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

void Boss1Area::ActorCreateInitialize()
{
	/*for(int i = 0; i < skeltonPopNumMax; i++){
		unique_ptr<Skelton> newObj = make_unique<Skelton>();
		newObj->Initialize("Skeleton");
		skelton.push_back(move(newObj));
	}*/
}

void Boss1Area::ActorSummon()
{
	if(!boss->GetIsSummonEnemyPop()) return;
	/*for(auto it = skelton.begin(); it != skelton.end(); it++){
		if(!(*it)->GetIsNotApp()){
			int lx = rand()%7 + -3;
			int lz = rand()%4;
			Vector3 pos = {lx*Block_Size, 0.f,(-lz)*Block_Size};
			pos += boss->GetPosition();
			(*it)->Pop(pos);
			boss->IsSummonEnemyPopNot();
			break;
		}
	}*/
}
