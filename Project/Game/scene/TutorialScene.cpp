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
	areaManager_->CSVAreaInitialize("Tutorial");
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
		moveSp->Initialize(tutorialText1_tex.number);
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
	areaManager_->CSVAreaUpdate(camera, player_->GetPosition());
}

void TutorialScene::AddObject3DUpdate()
{
	for(auto it = slime.begin(); it != slime.end(); it++){
		if((*it)->GetIsContactTrigger()){
			gameManager_->AudioPlay(damage_audio.number, damage_audio.volume);
			for(auto it2 = obj_.begin(); it2!= obj_.end(); it2++){
				if(!(*it2)->GetIsAlive()){
					(*it2)->Pop({(*it)->GetPopPosition().x, -5, (*it)->GetPopPosition().z});
					break;
				}
			}
		}
		(*it)->Update(camera,player_->GetPosition());
	}

	for(auto it = obj_.begin(); it != obj_.end(); it++){
		if((*it)->GetIsContactTrigger()){
			gameManager_->CoinIncrement();
			gameManager_->AudioPlay(coinGet_audio.number,coinGet_audio.volume);
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
		moveSp->SetPlayerPos(player_->GetPosition());
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
		if((*it)->GetIsPopsPmposibble_()){
			Vector3 lpos;

			if(areaManager_->GetCSVObjectPopActive(index,false)) {
				lpos = areaManager_->GetCSVObjectPopPosition(index);
				(*it)->Pop({lpos.x, -3.5f,lpos.z});
			}
			index++;
		}
	}

	for(auto it = obj_.begin(); it != obj_.end(); it++){
		if((*it)->GetIsAlive()){
			(*it)->IsBeatEndOn();
		}
	}
}

void TutorialScene::AddObject3DDraw()
{
	areaManager_->CSVAreaDraw();

	for(auto it = slime.begin(); it != slime.end(); it++){
		(*it)->Draw();
	}
	for(auto it = obj_.begin(); it != obj_.end(); it++){
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

	for(auto it = obj_.begin(); it != obj_.end(); it++){
		(*it)->Finalize();
	}
}

void TutorialScene::AddCommonFinalize()
{
	areaManager_->CSVAreaFinalize();
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
		obj_.push_back(move(newObj));
	}
}
