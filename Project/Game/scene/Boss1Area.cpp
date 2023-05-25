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
	CutInInitialize();
}

void Boss1Area::AddCommonUpdate()
{
	areaManager_->CSVAreaUpdate(camera, player_->GetPosition());

	//if(IsBossStart && !IsCutInHide){
	//	if(input->Trigger(DIK_Z) || input->Trigger(DIK_DOWN) || input->Trigger(DIK_UP) || input->Trigger(DIK_LEFT) || input->Trigger(DIK_RIGHT)){
	//		IsCutInMoveEnd = true;
	//		//リズム
	//		rhythmManager_->TimeStart();
	//		IsBossStart = false;
	//	}
	//}
	cutInInput();
}

void Boss1Area::AddObject3DUpdate()
{
	exit_->SetExitOpenNeedCoin(0);
	exit_->NeedCoinSpriteUpdate();
}

void Boss1Area::AddObject2DUpdate()
{
	CutInUpdate();
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
	CutInDraw();
}

void Boss1Area::AddObjectFinalize()
{
	//カットインSp
	CutInFinalize();
}

void Boss1Area::AddCommonFinalize()
{
	areaManager_->CSVAreaFinalize();
}

#pragma region カットイン
void Boss1Area::CutInInitialize()
{
	//音声
	gameManager_->AudioPlay(cutIn_audio.number, cutIn_audio.volume);

	//センター
	unique_ptr<BaseSprites> center = make_unique<BaseSprites>();
	center->Initialize(cutInBoss_tex.number);
	center->SetPosition(cutInSpCenterPos);
	center->SetAnchorPoint(cutInSpAnc);
	center->SetSize(cutInSpCenterSize);


	//追加
	cutInSp_.push_back(move(center));

	/*{
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
	}*/
}

void Boss1Area::cutInInput()
{
	if(!isBossAppUIFlag_ && isCutInUIMove) return;

	if(!input->Trigger(DIK_Z)) return;
	if(!input->Trigger(DIK_RETURN) || !input->Trigger(DIK_SPACE)) return;
	if(!input->Trigger(DIK_W) || !input->Trigger(DIK_A) || !input->Trigger(DIK_S) || !input->Trigger(DIK_D)) return;
	if(!input->Trigger(DIK_UP) || !input->Trigger(DIK_LEFT) || !input->Trigger(DIK_DOWN) || !input->Trigger(DIK_RIGHT)) return;
		

	isBossAppUIFlag_ = false;
	//リズム
	rhythmManager_->TimeStart();
}

void Boss1Area::CutInUpdate()
{
	//開始時
	if(isBossAppUIFlag_ && !isCutInUIMove){
		//移動
		cutInSpCenterPos = Easing_Point2_Linear<Vector2>(cutInSpCenterBeginPos, cutInSpCenterEndPos, Time_OneWay(cutInMoveframe,cutInMoveSecondMax));

		//目標値
		if(cutInMoveframe >= 1.0){
			cutInSpCenterPos = cutInSpCenterEndPos;

			//移動完了
			isCutInUIMove = true;
		}

		//更新
		cutInSp_[cutInSpCenterNumber]->SetPosition(cutInSpCenterPos);
	}

	//if(!IsCutInHide)
	//{
	//	if(IsCutInMoveStart){
	//		cutInPos = Easing_Point2_Linear<Vector2>({1920,360},{640,360},Time_OneWay(cutInMoveFrameCur, cutinSecond));
	//		cutInPartPos1 = Easing_Point2_Linear<Vector2>({1680,600},{880,600},Time_OneWay(cutInMoveFrameCur, cutinSecond));
	//		cutInPartPos2 = Easing_Point2_Linear<Vector2>({-400,120},{380,120},Time_OneWay(cutInMoveFrameCur, cutinSecond));
	//		bossNamePos = Easing_Point2_Linear<Vector2>({1605,610},{950,610},Time_OneWay(cutInMoveFrameCur, cutinSecond));
	//
	//		if(cutInMoveFrameCur >= 1.0f){
	//			cutInPos = {640,360};
	//			cutInPartPos1 = {880, 600};
	//			cutInPartPos2 = {380, 120};
	//			bossNamePos = {950,610};
	//			cutInMoveFrameCur = 0.f;
	//			IsCutInMoveStart = false;
	//			IsBossStart = true;
	//		}
	//		cutInSpMain->SetPosition(cutInPos);
	//		cutInSpPart1->SetPosition(cutInPartPos1);
	//		cutInSpPart2->SetPosition(cutInPartPos2);
	//		bossName->SetPosition(bossNamePos);
	//	}
	//	else if(IsCutInMoveEnd){
	//		cutInPos = Easing_Point2_Linear<Vector2>({640,360},{1920,360},Time_OneWay(cutInMoveFrameCur, cutinSecond));
	//		cutInPartPos1 = Easing_Point2_Linear<Vector2>({800,600},{1680,600},Time_OneWay(cutInMoveFrameCur, cutinSecond));
	//		cutInPartPos2 = Easing_Point2_Linear<Vector2>({380,120},{-400,120},Time_OneWay(cutInMoveFrameCur, cutinSecond));
	//		bossNamePos = Easing_Point2_Linear<Vector2>({950,610},{1605,610},Time_OneWay(cutInMoveFrameCur, cutinSecond));

	//		if(cutInMoveFrameCur >= 1.0f){
	//			cutInPos = {1920,360};
	//			cutInPartPos1 = {1680, 600};
	//			cutInPartPos2 = {-400, 120};
	//			bossNamePos = {1605,610};
	//			cutInMoveFrameCur = 0.f;
	//			IsCutInMoveEnd = false;
	//			IsCutInHide = true;
	//		}
	//		cutInSpMain->SetPosition(cutInPos);
	//		cutInSpPart1->SetPosition(cutInPartPos1);
	//		cutInSpPart2->SetPosition(cutInPartPos2);
	//		bossName->SetPosition(bossNamePos);
	//	}

	//	cutInSpMain->Update();
	//	cutInSpPart1->Update();
	//	cutInSpPart2->Update();
	//	bossName->Update();
	//}

	for(auto it = cutInSp_.begin(); it != cutInSp_.end(); it++){
		(*it)->Update();
	}
}

void Boss1Area::CutInDraw()
{
	/*if(IsCutInHide) return;
	cutInSpMain->Draw();
	cutInSpPart1->Draw();
	cutInSpPart2->Draw();
	bossName->Draw();*/
	for(auto it = cutInSp_.begin(); it != cutInSp_.end(); it++){
		(*it)->Draw();
	}
}

void Boss1Area::CutInFinalize()
{
	/*cutInSpMain->Finalize();
	cutInSpPart1->Finalize();
	cutInSpPart2->Finalize();
	bossName->Finalize();*/

	for(auto it = cutInSp_.begin(); it != cutInSp_.end(); it++){
		(*it)->Finalize();
	}
}
#pragma endregion
