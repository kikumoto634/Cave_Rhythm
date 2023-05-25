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
		if(!postEffect->FadeIn()) return;
		
		isPrevSceneChange_ = false;
		return;
	}
	//NextSceneへの移動
	else if(isNextSceneChange_ || isGameEnd_){
		if(!postEffect->FadeOut()) return;

		isDrawStop = true;
		
		camera->Reset();
		if(isNextSceneChange_)NextSceneChange();
		else if(isGameEnd_)SceneGameEnd();
	}
}

void Boss1Area::AddCommonInitialize()
{
	//ダンジョン
	areaManager_->CSVAreaInitialize("Boss1");
}

void Boss1Area::AddObject3DInitialize()
{
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
			//リズム
			rhythmManager_->TimeStart();
			IsBossStart = false;
		}
	}
}

void Boss1Area::AddObject3DUpdate()
{
	exit_->SetExitOpenNeedCoin(0);
	exit_->NeedCoinSpriteUpdate();
}

void Boss1Area::AddObject2DUpdate()
{
	cutinUpdate();
}

void Boss1Area::AddBeatEndUpdate()
{
}

void Boss1Area::AddObject3DDraw()
{
	areaManager_->CSVAreaDraw();
}

void Boss1Area::AddParticleDraw()
{
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
			gameManager_->AudioPlay(cutIn_audio.number, cutIn_audio.volume);
			IsCutInAudio = true;
		}

		if(IsCutInMoveStart){
			cutInPos = Easing_Point2_Linear<Vector2>({1920,360},{640,360},Time_OneWay(cutInMoveFrameCur, cutinSecond));
			cutInPartPos1 = Easing_Point2_Linear<Vector2>({1680,600},{880,600},Time_OneWay(cutInMoveFrameCur, cutinSecond));
			cutInPartPos2 = Easing_Point2_Linear<Vector2>({-400,120},{380,120},Time_OneWay(cutInMoveFrameCur, cutinSecond));
			bossNamePos = Easing_Point2_Linear<Vector2>({1605,610},{950,610},Time_OneWay(cutInMoveFrameCur, cutinSecond));
	
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
			cutInPos = Easing_Point2_Linear<Vector2>({640,360},{1920,360},Time_OneWay(cutInMoveFrameCur, cutinSecond));
			cutInPartPos1 = Easing_Point2_Linear<Vector2>({800,600},{1680,600},Time_OneWay(cutInMoveFrameCur, cutinSecond));
			cutInPartPos2 = Easing_Point2_Linear<Vector2>({380,120},{-400,120},Time_OneWay(cutInMoveFrameCur, cutinSecond));
			bossNamePos = Easing_Point2_Linear<Vector2>({950,610},{1605,610},Time_OneWay(cutInMoveFrameCur, cutinSecond));

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
