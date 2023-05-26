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
	indestructibleWallModel_ = new ObjModelManager();
	indestructibleWallModel_->CreateModel("GroundBlock3");
	indestructibleWallColliderModel_ = new ObjModelManager();
	indestructibleWallColliderModel_->CreateModel("GroundBlock2_Collider");
	//EnterInitialize();
	ExitInitialize();
}

void Boss1Area::AddObject2DInitialize()
{
	CutInInitialize();
}

void Boss1Area::AddCommonUpdate()
{
	areaManager_->CSVAreaUpdate(camera, player_->GetPosition());

	cutInInput();
}

void Boss1Area::AddObject3DUpdate()
{
	exit_->SetExitOpenNeedCoin(0);
	exit_->NeedCoinSpriteUpdate();

	//EnterUpdate();
	ExitUpdate();
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

	//EnterDraw();
	ExitDraw();
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
	CutInFinalize();
	//EnterFinalize();
	ExitFinalize();
	delete indestructibleWallModel_;
	delete indestructibleWallColliderModel_;
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
	center->SetAnchorPoint(CutInSpAnc);
	center->SetSize(CutInSpCenterSize);

	//上
	unique_ptr<BaseSprites> up = make_unique<BaseSprites>();
	up->Initialize(cutInParts2_tex.number);
	up->SetAnchorPoint(CutInSpAnc);
	up->SetSize(CutInSpUpDownSize);

	//下
	unique_ptr<BaseSprites> down = make_unique<BaseSprites>();
	down->Initialize(cutInParts1_tex.number);
	down->SetAnchorPoint(CutInSpAnc);
	down->SetSize(CutInSpUpDownSize);

	//名前
	unique_ptr<BaseSprites> name = make_unique<BaseSprites>();
	name->Initialize(bossName_tex.number);
	name->SetAnchorPoint(CutInSpAnc);
	name->SetSize(CutInSpNameSize);

	//追加
	cutInSp_.push_back(move(center));
	cutInSp_.push_back(move(up));
	cutInSp_.push_back(move(down));
	cutInSp_.push_back(move(name));

	cutInSpPos_.resize(CutInSpNum);
}

void Boss1Area::cutInInput()
{
	if(!isCutInAlive_) return;

	if(!input->Trigger(DIK_UP) && !input->Trigger(DIK_LEFT) && !input->Trigger(DIK_DOWN) && !input->Trigger(DIK_RIGHT) &&
		!input->Trigger(DIK_W) && !input->Trigger(DIK_A) && !input->Trigger(DIK_S) && !input->Trigger(DIK_D) &&
		!input->Trigger(DIK_Z) && !input->Trigger(DIK_SPACE) && !input->Trigger(DIK_RETURN)) return;
		

	isBossAppUIFlag_ = false;
	//リズム
	rhythmManager_->TimeStart();
}

void Boss1Area::CutInUpdate()
{
	if(!isCutInAlive_) return;
	//開始時
	if(isBossAppUIFlag_ && !isCutInUIMove_){
		//時間
		cutInMoveframe_ = Time_OneWay(cutInMoveframe_, CutInMoveSecondMax);

		//移動
		cutInSpPos_[CutInSpCenterNumber] = Easing_Point2_Linear<Vector2>(CutInSpCenterBeginPos, CutInSpCenterEndPos, cutInMoveframe_);
		cutInSpPos_[CutInSpUpNumber]     = Easing_Point2_Linear<Vector2>(CutInSpUpBeginPos, CutInSpUpEndPos, cutInMoveframe_);
		cutInSpPos_[CutInSpDownNumber]   = Easing_Point2_Linear<Vector2>(CutInSpDownBeginPos, CutInSpDownEndPos, cutInMoveframe_);
		cutInSpPos_[CutInSpNameNumber]   = Easing_Point2_Linear<Vector2>(CutInSpNameBeginPos, CutInSpNameEndPos, cutInMoveframe_);

		//目標値
		if(cutInMoveframe_ >= 1.0){
			cutInSpPos_[CutInSpCenterNumber] = CutInSpCenterEndPos;
			cutInSpPos_[CutInSpUpNumber]     = CutInSpUpEndPos;
			cutInSpPos_[CutInSpDownNumber]   = CutInSpDownEndPos;
			cutInSpPos_[CutInSpNameNumber]   = CutInSpNameEndPos;
			cutInMoveframe_ = 0.f;

			//移動完了
			isCutInUIMove_ = true;
		}
	}
	//UI引き
	else if(!isBossAppUIFlag_ && isCutInUIMove_){
		//時間
		cutInMoveframe_ = Time_OneWay(cutInMoveframe_, CutInMoveSecondMax);

		//移動
		cutInSpPos_[CutInSpCenterNumber] = Easing_Point2_Linear<Vector2>(CutInSpCenterEndPos, CutInSpCenterBeginPos, cutInMoveframe_);
		cutInSpPos_[CutInSpUpNumber]     = Easing_Point2_Linear<Vector2>(CutInSpUpEndPos, CutInSpUpBeginPos, cutInMoveframe_);
		cutInSpPos_[CutInSpDownNumber]   = Easing_Point2_Linear<Vector2>(CutInSpDownEndPos, CutInSpDownBeginPos, cutInMoveframe_);
		cutInSpPos_[CutInSpNameNumber]   = Easing_Point2_Linear<Vector2>(CutInSpNameEndPos, CutInSpNameBeginPos, cutInMoveframe_);

		//目標値
		if(cutInMoveframe_ >= 1.0){
			cutInSpPos_[CutInSpCenterNumber] = CutInSpCenterBeginPos;
			cutInSpPos_[CutInSpUpNumber]     = CutInSpUpBeginPos;
			cutInSpPos_[CutInSpDownNumber]   = CutInSpDownBeginPos;
			cutInSpPos_[CutInSpNameNumber]   = CutInSpNameBeginPos;
			isCutInAlive_ = false;
		}
	}

	//更新
	int i = 0;
	for(const auto& it : cutInSp_){
		it->SetPosition(cutInSpPos_[i]);
		it->Update();
		i++;
	}
}

void Boss1Area::CutInDraw()
{
	if(!isCutInAlive_) return;
	for(const auto& it : cutInSp_){
		it->Draw();
	}
}

void Boss1Area::CutInFinalize()
{
	for(const auto& it : cutInSp_){
		it->Finalize();
	}
}
#pragma endregion


#pragma region 入口
void Boss1Area::EnterInitialize()
{
	for(int i = 0; i < BlocksNum; i++){
		unique_ptr<IndestructibleWall> obj = make_unique<IndestructibleWall>();
		obj->Initialize(indestructibleWallModel_,indestructibleWallColliderModel_);
		obj->SetPosition({EnterWallBasePos.x + BlockSize*i, EnterWallBasePos.y, EnterWallBasePos.z});
		enterWall_.push_back(move(obj));
	}
}

void Boss1Area::EnterUpdate()
{
	if(!isEnterBlocksAlive_){
		if(player_->GetPosition().z < EnterCloseBorderPosZ) return;
		camera->ShakeStart();
		gameManager_->AudioPlay(gateEnter_audio.number);
		isEnterBlocksAlive_ = true;
		return;
	}

	for(const auto& it : enterWall_){
		it->Update(camera);
	}
}

void Boss1Area::EnterDraw()
{
	if(!isEnterBlocksAlive_) return;
	for(const auto& it : enterWall_){
		it->Draw();
	}
}

void Boss1Area::EnterFinalize()
{
	for(const auto& it : enterWall_){
		it->Finalize();
	}
}
#pragma endregion


#pragma region 出口
void Boss1Area::ExitInitialize()
{
	for(int i = 0; i < BlocksNum; i++){
		unique_ptr<IndestructibleWall> obj = make_unique<IndestructibleWall>();
		obj->Initialize(indestructibleWallModel_,indestructibleWallColliderModel_);
		obj->SetPosition({ExitWallBasePos.x + BlockSize*i, ExitWallBasePos.y, ExitWallBasePos.z});
		exitWall_.push_back(move(obj));
	}
}

void Boss1Area::ExitUpdate()
{
	if(!isExitBlocksAlive_) return;
	for(const auto& it : exitWall_){
		it->Update(camera);
	}
}

void Boss1Area::ExitDraw()
{
	if(!isExitBlocksAlive_) return;
	for(const auto& it : exitWall_){
		it->Draw();
	}
}

void Boss1Area::ExitFinalize()
{
	for(const auto& it : exitWall_){
		it->Finalize();
	}
}
#pragma endregion