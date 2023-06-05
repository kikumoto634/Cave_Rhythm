#include "GameScene.h"

#include "HomeScene.h"
#include "Boss1Area.h"
#include "SceneManager.h"

using namespace std;

GameScene::~GameScene()
{
	Finalize();
}

GameScene::GameScene(DirectXCommon *dxCommon, Window *window, int saveHP,int floorValue)
	:BaseBattleScene(
		dxCommon,
		window,
		saveHP,
		floorValue)
{
	this->saveHP_ = saveHP;
	this->floorValue_ = floorValue;
}

void GameScene::NextSceneChange()
{
	if(floorValue_ >= 1){
		sceneManager->SetNextScene(new Boss1Area(dxCommon,window,player_->GetHp()));
		//sceneManager->SetNextScene(new HomeScene(dxCommon,window,player_->GetHp()));
	}
	else{
		sceneManager->SetNextScene(new GameScene(dxCommon,window,player_->GetHp(), floorValue_));
	}
}

void GameScene::SceneGameEnd()
{
	sceneManager->SetNextScene(new HomeScene(dxCommon,window));
}


void GameScene::AddCommonInitialize()
{
	srand( (unsigned int)time(NULL) );

	//ダンジョン
	areaManager_->RandamAreaInitialize();

	//階層に応じた生成数
	switch (floorValue_)
	{
	case 1:
		needCoin_= one.needCoin;
		slimePopNumMax_ = one.slimne;
		skeltonPopNumMax_ = one.skelton;
		break;
	case 2:
		needCoin_= two.needCoin;
		slimePopNumMax_ = two.slimne;
		skeltonPopNumMax_ = two.skelton;
		break;
	case 3:
		needCoin_= three.needCoin;
		slimePopNumMax_ = three.slimne;
		skeltonPopNumMax_ = three.skelton;
		break;
	case 4:
		needCoin_= four.needCoin;
		slimePopNumMax_ = four.slimne;
		skeltonPopNumMax_ = four.skelton;
		break;
	default:
		needCoin_= four.needCoin;
		slimePopNumMax_ = four.slimne;
		skeltonPopNumMax_ = four.skelton;
		break;
	}
}

void GameScene::AddObject3DInitialize()
{
	ActorCreateInitialize();

	player_->SetHp(saveHP_);

	exit_->SetExitOpenNeedCoin(needCoin_);
	exit_->NeedCoinSpriteUpdate();
}

void GameScene::AddObject2DInitialize()
{
	//階層テキスト
	{
		unique_ptr<BaseSprites> newSp = make_unique<BaseSprites>();
		newSp->Initialize(depthValue_tex.number);
		newSp->SetPosition(TextPos);
		newSp->SetSize(TextSize);
		newSp->SetAnchorPoint(TextAnc);

		sp_.push_back(move(newSp));
	}
	//階層値
	{
		unique_ptr<BaseSprites> newSp = make_unique<BaseSprites>();
		int value = floorValue_ + numberTextBase_;
		newSp->Initialize(value);
		newSp->SetPosition(ValuePos);
		newSp->SetSize(ValueSize);
		newSp->SetAnchorPoint(ValueAnc);
		floorValue_ += DepthIncreValue;

		sp_.push_back(move(newSp));
	}
}

void GameScene::AddCommonUpdate()
{
	if(player_ == nullptr) return;
	areaManager_->RandamAreaUpdate(camera, player_->GetPosition());
	areaManager_->BreakBlock(player_->GetBlockBreakPos());
}

void GameScene::AddObject3DUpdate()
{
	for(auto it = enemys_.begin(); it != enemys_.end(); ++it){
		if((*it)->GetIsContactTrigger()){
			gameManager_->AudioPlay(damage_audio.number, damage_audio.volume);
			coinDropPos_.push((*it)->GetPopPosition());
		}
		(*it)->SetMapInfo(areaManager_->GetMapInfo());
		(*it)->Update(camera,player_->GetPosition());
	}
	
	for(auto it = obj_.begin(); it != obj_.end(); it++){
		if((*it)->GetIsContactTrigger()){
			gameManager_->CoinIncrement();
			gameManager_->AudioPlay(coinGet_audio.number,coinGet_audio.volume);
		}

		if(!coinDropPos_.empty() && (*it)->GetIsPopsPmposibble_()){
			(*it)->Pop(coinDropPos_.front());
			coinDropPos_.pop();
		}

		(*it)->Update(this->camera);
	}
}

void GameScene::AddObject2DUpdate()
{
	for(auto it = sp_.begin(); it != sp_.end(); ++it){
		(*it)->Update();
	}
}

void GameScene::AddBeatEndUpdate()
{
	for(auto it = enemys_.begin(); it != enemys_.end(); ++it){
		if((*it)->GetIsPopsPmposibble_()){
			Vector3 lpos = areaManager_->GetObjectPopPosition();
			(*it)->Pop(lpos);
			(*it)->CaveLightOn();
			break;
		}
	}


	for(auto it = enemys_.begin(); it != enemys_.end(); ++it){
		if(!(*it)->GetIsAlive()) continue;
		(*it)->IsBeatEndOn();
	}

	for(auto it = obj_.begin(); it != obj_.end(); ++it){
		if(!(*it)->GetIsAlive()) continue;
		(*it)->IsBeatEndOn();
	}
}

void GameScene::AddObject3DDraw()
{
	areaManager_->RandamAreaDraw();

	for(auto it = enemys_.begin(); it != enemys_.end(); ++it){
		(*it)->Draw();
	}

	for(auto it = obj_.begin(); it != obj_.end(); ++it){
		(*it)->Draw();
	}
}

void GameScene::AddParticleDraw()
{
	for(auto it = enemys_.begin(); it != enemys_.end(); ++it){
		(*it)->ParticleDraw();
	}
}

void GameScene::AddFrontUIDraw()
{
	for(auto it = sp_.begin(); it != sp_.end(); ++it){
		(*it)->Draw();
	}
}

void GameScene::AddBackUIDraw()
{
}

void GameScene::AddObjectFinalize()
{
	for(auto it = sp_.begin(); it != sp_.end(); ++it){
		(*it)->Finalize();
	}

	for(auto it = enemys_.begin(); it != enemys_.end(); ++it){
		(*it)->Finalize();
	}

	for(auto it = obj_.begin(); it != obj_.end(); ++it){
		(*it)->Finalize();
	}
}

void GameScene::AddCommonFinalize()
{
	areaManager_->RandamAreaFinalize();
}


void GameScene::ActorCreateInitialize()
{
	for(int i = 0; i < slimePopNumMax_; i++){
		unique_ptr<BlueSlime> newObj = make_unique<BlueSlime>();
		newObj->Initialize("slime");
		enemys_.push_back(move(newObj));
	}
	for(int i = 0; i < skeltonPopNumMax_; i++){
		unique_ptr<Skelton> newObj = make_unique<Skelton>();
		newObj->Initialize("Skeleton");
		enemys_.push_back(move(newObj));
	}

	for(int i = 0; i < coinPopNumMax_; i++){
		unique_ptr<Coins> newObj = make_unique<Coins>();
		newObj->Initialize("Coins");
		obj_.push_back(move(newObj));
	}
}
