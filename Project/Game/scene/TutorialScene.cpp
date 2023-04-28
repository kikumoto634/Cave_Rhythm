#include "TutorialScene.h"

#include "TitleScene.h"
#include "HomeScene.h"
#include "SceneManager.h"

using namespace std;

TutorialScene::~TutorialScene()
{
	Finalize();
}

TutorialScene::TutorialScene(DirectXCommon *dxCommon, Window *window, int saveHP)
	:BaseBattleScene(
		dxCommon,
		window)
{
}

void TutorialScene::NextSceneChange()
{
	sceneManager->SetNextScene(new HomeScene(dxCommon,window));
}

void TutorialScene::AddCommonInitialize()
{
	//ダンジョン
	areaManager->CSVAreaInitialize("Tutorial");
}

void TutorialScene::AddObject3DInitialize()
{
	slimePopNumMax = 2;
	coinPopNumMax = 2;
	ActorCreateInitialize();
}

void TutorialScene::AddObject2DInitialize()
{
	Vector2 lsize = {384,64};
	Vector2 lancher = {0.5f,0.5f};
	{
		moveSp = make_unique<TutorialSp>();
		moveSp->Initialize(23);
		Vector3 ltarget = moveSpPos;
		Vector2 lpos = moveSp->ChangeTransformation(ltarget, this->camera);
		moveSp->SetPosition(lpos);
		moveSp->SetSize(lsize);
		moveSp->SetAnchorPoint(lancher);
	}

	/*{
		attackSp = make_unique<TutorialSp>();
		attackSp->Initialize(24);
		Vector3 ltarget = attackSpPos;
		Vector2 lpos = attackSp->ChangeTransformation(ltarget, this->camera);
		attackSp->SetPosition(lpos);
		attackSp->SetSize(lsize);
		attackSp->SetAnchorPoint(lancher);
	}*/
}

void TutorialScene::AddCommonUpdate()
{
	areaManager->CSVAreaUpdate(camera, player->GetPosition());
}

void TutorialScene::AddObject3DUpdate()
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

	for(auto it = coin.begin(); it != coin.end(); it++){
		if((*it)->GetCoin()){
			gameManager->CoinIncrement();
			gameManager->AudioPlay(7,0.5f);
		}
		(*it)->Update(this->camera);
	}
}

void TutorialScene::AddObject2DUpdate()
{
	{
		Vector3 ltarget = moveSpPos;
		Vector2 lpos = moveSp->ChangeTransformation(ltarget, this->camera);
		moveSp->SetPosition(lpos);
		moveSp->SetPlayerPos(player->GetPosition());
		moveSp->Update();
	}

	/*{
		Vector3 ltarget = attackSpPos;
		Vector2 lpos = attackSp->ChangeTransformation(ltarget, this->camera);
		attackSp->SetPosition(lpos);
		attackSp->SetPlayerPos(player->GetPosition());
		attackSp->Update();
	}*/
}

void TutorialScene::AddBeatEndUpdate()
{
	int index = 0;
	for(auto it = slime.begin(); it != slime.end(); it++){
		(*it)->IsBeatEndOn();
		if(!(*it)->GetIsNotApp()){
			Vector3 lpos;

			if(areaManager->GetCSVObjectPopActive(index,false)) {
				lpos = areaManager->GetCSVObjectPopPosition(index);
				(*it)->Pop({lpos.x, -3.5f,lpos.z});
			}
			index++;
		}
	}

	for(auto it = coin.begin(); it != coin.end(); it++){
		if((*it)->GetIsAlive()){
			(*it)->IsBeatEndOn();
		}
	}
}

void TutorialScene::AddObject3DDraw()
{
	areaManager->CSVAreaDraw();

	for(auto it = slime.begin(); it != slime.end(); it++){
		(*it)->Draw();
	}
	for(auto it = coin.begin(); it != coin.end(); it++){
		(*it)->Draw();
	}
}

void TutorialScene::AddParticleDraw()
{
	for(auto it = slime.begin(); it != slime.end(); it++){
		(*it)->ParticleDraw();
	}
}

void TutorialScene::AddFrontUIDraw()
{
	moveSp->Draw();
	/*attackSp->Draw();*/
}

void TutorialScene::AddBackUIDraw()
{
}

void TutorialScene::AddObjectFinalize()
{
	//attackSp->Finalize();
	moveSp->Finalize();

	for(auto it = slime.begin(); it != slime.end(); it++){
		(*it)->Finalize();
	}

	for(auto it = coin.begin(); it != coin.end(); it++){
		(*it)->Finalize();
	}
}

void TutorialScene::AddCommonFinalize()
{
	areaManager->CSVAreaFinalize();
}


void TutorialScene::ActorCreateInitialize()
{
	for(int i = 0; i < slimePopNumMax; i++){
		unique_ptr<BlueSlime> newObj = make_unique<BlueSlime>();
		newObj->Initialize("slime");
		slime.push_back(move(newObj));
	}

	for(int i = 0; i < coinPopNumMax; i++){
		unique_ptr<Coins> newObj = make_unique<Coins>();
		newObj->Initialize("Coins");
		coin.push_back(move(newObj));
	}
}
