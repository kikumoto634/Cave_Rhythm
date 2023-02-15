#include "GameScene.h"

using namespace std;

GameScene::GameScene(DirectXCommon *dxCommon, Window *window, int saveHP)
	:BaseBattleScene(
		dxCommon,
		window)
{
}

void GameScene::NextSceneChange()
{
}

void GameScene::AreaManagerInitialize()
{
	//ダンジョン
	//BaseBattleScene::AreaManagerInitialize();
	//areaManager->RandamAreaInitialize();
	areaManager->CSVAreaInitialize("Tutorial");
}

void GameScene::AddCommonInitialize()
{
}

void GameScene::AddObject3DInitialize()
{
	//スライム
	slime =make_unique<BlueSlime>();
	slime->Initialize("slime");

	coin=make_unique<Coins>();
	coin->Initialize("Coins");
}

void GameScene::AddObject2DInitialize()
{
}

void GameScene::AddCommonUpdate()
{
}

void GameScene::AddObject3DUpdate()
{
	if(slime->GetIsDeadAudio()){
		gameManager->AudioPlay(2, 0.5f);
		if(coin->PopPossible()){
			coin->Pop({slime->GetDeadParticlepos().x, -5, slime->GetDeadParticlepos().z});
		}
	}
	slime->Update(camera,player->GetPosition());

	if(coin->GetCoin()){
		gameManager->CoinIncrement();
		gameManager->AudioPlay(7,0.5f);
	}
	coin->Update(this->camera);
}

void GameScene::AddObject2DUpdate()
{
}

void GameScene::AddBeatEndUpdate()
{
	slime->IsBeatEndOn();
	if(!slime->GetIsNotApp()){
		Vector3 lpos;
		//lpos = areaManager->GetObjectPopPosition();

		int index = 0;
		if(areaManager->GetCSVObjectPopActive(index,false)) {
			lpos = areaManager->GetCSVObjectPopPosition(index);
			slime->Pop({lpos.x, -3.5f,lpos.z});
		}
	}

	if(coin->GetIsAlive()){
		coin->IsBeatEndOn();
	}
}

void GameScene::AddObject3DDraw()
{
	slime->Draw();
	coin->Draw();
}

void GameScene::AddParticleDraw()
{
	slime->ParticleDraw();
}

void GameScene::AddUIDraw()
{
}

void GameScene::AddObjectFinalize()
{
	slime->Finalize();
	coin->Finalize();
}

void GameScene::AddCommonFinalize()
{
}
