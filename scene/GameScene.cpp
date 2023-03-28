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
	this->saveHP = saveHP;
	this->floorValue = floorValue;
}

void GameScene::NextSceneChange()
{
	if(floorValue >= 5){
		sceneManager->SetNextScene(new Boss1Area(dxCommon,window,player->GetHP()));
	}
	else{
		sceneManager->SetNextScene(new GameScene(dxCommon,window,player->GetHP(), floorValue));
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
	areaManager->RandamAreaInitialize();

	//階層に応じた生成数
	switch (floorValue)
	{
	case 1:
		needCoin= one.needCoin;
		slimePopNumMax = one.slimne;
		skeltonPopNumMax = one.skelton;
		break;
	case 2:
		needCoin= two.needCoin;
		slimePopNumMax = two.slimne;
		skeltonPopNumMax = two.skelton;
		break;
	case 3:
		needCoin= three.needCoin;
		slimePopNumMax = three.slimne;
		skeltonPopNumMax = three.skelton;
		break;
	case 4:
		needCoin= four.needCoin;
		slimePopNumMax = four.slimne;
		skeltonPopNumMax = four.skelton;
		break;
	default:
		needCoin= four.needCoin;
		slimePopNumMax = four.slimne;
		skeltonPopNumMax = four.skelton;
		break;
	}
}

void GameScene::AddObject3DInitialize()
{
	ActorCreateInitialize();

	player->SetHp(saveHP);

	exit->SetExitOpenNeedCoin(needCoin);
	exit->NeedCoinSpriteUpdate();
}

void GameScene::AddObject2DInitialize()
{
	//階層テキスト
	{
		floorDepth = make_unique<BaseSprites>();
		floorDepth->Initialize(27);
		floorDepth->SetPosition({1100, 700});
		floorDepth->SetSize({100,25});
		floorDepth->SetAnchorPoint({0.5f,0.5f});
	}
	//階層値
	{
		floorValueTex = make_unique<BaseSprites>();
		int value = floorValue + numberTextBase;
		floorValueTex->Initialize(value);
		floorValueTex->SetPosition({1170, 700});
		floorValueTex->SetSize({15,25});
		floorValueTex->SetAnchorPoint({0.5f,0.5f});
		floorValue += 1;
	}
}

void GameScene::AddCommonUpdate()
{
	if(player == nullptr) return;
	areaManager->RandamAreaUpdate(camera, player->GetPosition());
}

void GameScene::AddObject3DUpdate()
{
	for(auto it = slime.begin(); it != slime.end(); it++){
		if((*it)->GetIsDeadAudio()){
			gameManager->AudioPlay(2, 0.5f);
			for(auto it2 = coin.begin(); it2!= coin.end(); it2++){
				if((*it2)->PopPossible()){
					(*it2)->Pop({(*it)->GetDeadParticlepos().x, -5, (*it)->GetDeadParticlepos().z});
					break;
				}
			}
		}
		(*it)->Update(camera,player->GetPosition());
	}

	for(auto it = skelton.begin(); it != skelton.end(); it++){
		if((*it)->GetIsDeadAudio()){
			gameManager->AudioPlay(2, 0.5f);
			for(auto it2 = coin.begin(); it2!= coin.end(); it2++){
				if((*it2)->PopPossible()){
					(*it2)->Pop({(*it)->GetDeadParticlepos().x, -5, (*it)->GetDeadParticlepos().z});
					break;
				}
			}
		}
		(*it)->Update(camera,player->GetPosition());
	}

	for(auto it = coin.begin(); it != coin.end(); it++){
		if((*it)->GetCoin()){
			gameManager->CoinIncrement();
			gameManager->AudioPlay(7,0.5f);
		}
		(*it)->Update(this->camera);
	}
}

void GameScene::AddObject2DUpdate()
{
	floorDepth->Update();
	floorValueTex->Update();
}

void GameScene::AddBeatEndUpdate()
{
	if(popCount >= PopCount){
		for(auto it = slime.begin(); it != slime.end(); it++){
			if(!(*it)->GetIsNotApp()){
				Vector3 lpos = areaManager->GetObjectPopPosition();
				(*it)->Pop({lpos.x, -3.5f,lpos.z});
				(*it)->CaveLightOn();
				break;
			}
		}
		for(auto it = skelton.begin(); it != skelton.end(); it++){
			if(!(*it)->GetIsNotApp()){
				Vector3 lpos = areaManager->GetObjectPopPosition();
				(*it)->Pop({lpos.x, -3.5f,lpos.z});
				(*it)->CaveLightOn();
				break;
			}
		}
		popCount = 0;
	}
	else{
		popCount++;
	}


	for(auto it = slime.begin(); it != slime.end(); it++){
		(*it)->IsBeatEndOn();
	}

	for(auto it = skelton.begin(); it != skelton.end(); it++){
		(*it)->IsBeatEndOn();
	}

	for(auto it = coin.begin(); it != coin.end(); it++){
		if((*it)->GetIsAlive()){
			(*it)->IsBeatEndOn();
		}
	}
}

void GameScene::AddObject3DDraw()
{
	areaManager->RandamAreaDraw();

	for(auto it = slime.begin(); it != slime.end(); it++){
		(*it)->Draw();
	}
	for(auto it = skelton.begin(); it != skelton.end(); it++){
		(*it)->Draw();
	}
	for(auto it = coin.begin(); it != coin.end(); it++){
		(*it)->Draw();
	}
}

void GameScene::AddParticleDraw()
{
	for(auto it = slime.begin(); it != slime.end(); it++){
		(*it)->ParticleDraw();
	}
	for(auto it = skelton.begin(); it != skelton.end(); it++){
		(*it)->ParticleDraw();
	}
}

void GameScene::AddFrontUIDraw()
{
	floorDepth->Draw();
	floorValueTex->Draw();
}

void GameScene::AddBackUIDraw()
{
}

void GameScene::AddObjectFinalize()
{
	floorDepth->Finalize();
	floorValueTex->Finalize();

	for(auto it = slime.begin(); it != slime.end(); it++){
		(*it)->Finalize();
	}
	for(auto it = skelton.begin(); it != skelton.end(); it++){
		(*it)->Finalize();
	}
	for(auto it = coin.begin(); it != coin.end(); it++){
		(*it)->Finalize();
	}
}

void GameScene::AddCommonFinalize()
{
	areaManager->RandamAreaFinalize();
}


void GameScene::ActorCreateInitialize()
{
	for(int i = 0; i < slimePopNumMax; i++){
		unique_ptr<BlueSlime> newObj = make_unique<BlueSlime>();
		newObj->Initialize("slime");
		slime.push_back(move(newObj));
	}

	for(int i = 0; i < skeltonPopNumMax; i++){
		unique_ptr<Skelton> newObj = make_unique<Skelton>();
		newObj->Initialize("Skeleton");
		skelton.push_back(move(newObj));
	}

	for(int i = 0; i < coinPopNumMax; i++){
		unique_ptr<Coins> newObj = make_unique<Coins>();
		newObj->Initialize("Coins");
		coin.push_back(move(newObj));
	}
}
