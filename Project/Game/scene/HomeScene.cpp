#include "HomeScene.h"

#include "TitleScene.h"
#include "GameScene.h"
#include "SceneManager.h"
using namespace std;

HomeScene::~HomeScene()
{
	Finalize();
}

HomeScene::HomeScene(DirectXCommon *dxCommon, Window *window, int saveHP)
	:BaseBattleScene(
		dxCommon,
		window)
{
}

void HomeScene::NextSceneChange()
{
	sceneManager->SetNextScene(new GameScene(dxCommon,window));
}

void HomeScene::AddCommonInitialize()
{
	//ダンジョン
	areaManager_->CSVAreaInitialize("Home");
}

void HomeScene::AddObject3DInitialize()
{
	exit_->SetExitOpenNeedCoin(0);
	exit_->NeedCoinSpriteUpdate();

	ActorCreateInitialize();
}

void HomeScene::AddObject2DInitialize()
{
	{
		exitText = make_unique<TutorialSp>();
		exitText->Initialize(goText.number);
		Vector3 ltarget = exitTextPos;
		Vector2 lpos = exitText->ChangeTransformation(ltarget, this->camera);
		exitText->SetPosition(lpos);
		exitText->SetSize({200,40});
		exitText->SetAnchorPoint({0.5f,0.5f});
	}
	
	{
		trainingText = make_unique<TutorialSp>();
		trainingText->Initialize(training_tex.number);
		Vector3 ltarget = trainingTextPos;
		Vector2 lpos = exitText->ChangeTransformation(ltarget, this->camera);
		trainingText->SetPosition(lpos);
		trainingText->SetSize({200,40});
		trainingText->SetAnchorPoint({0.5f,0.5f});
	}
}

void HomeScene::AddCommonUpdate()
{
	areaManager_->CSVAreaUpdate(camera, player_->GetPosition());
}

void HomeScene::AddObject3DUpdate()
{
	for(auto it = slime.begin(); it != slime.end(); it++){
		if((*it)->GetIsDeadTrigger()){
			gameManager_->AudioPlay(damage_audio.number, damage_audio.volume);
		}
		(*it)->Update(camera,player_->GetPosition());
	}
}

void HomeScene::AddObject2DUpdate()
{
	{
		Vector3 ltarget = exitTextPos;
		Vector2 lpos = exitText->ChangeTransformation(ltarget, this->camera);
		exitText->SetPosition(lpos);
		exitText->SetPlayerPos(player_->GetPosition());
		exitText->Update();
	}
	{
		Vector3 ltarget = trainingTextPos;
		Vector2 lpos = trainingText->ChangeTransformation(ltarget, this->camera);
		trainingText->SetPosition(lpos);
		trainingText->SetPlayerPos(player_->GetPosition());
		trainingText->Update();
	}
}

void HomeScene::AddBeatEndUpdate()
{
	int index = 0;
	Vector3 lpos;
	for(auto it = slime.begin(); it != slime.end(); it++){
		(*it)->IsBeatEndOn();
		if((*it)->GetIsPosImposibble_()){
			if(areaManager_->GetCSVObjectPopActive(index)) {
				lpos = areaManager_->GetCSVObjectPopPosition(index);
				(*it)->Pop({lpos.x, -3.5f,lpos.z});
			}
			index++;
		}
	}
}

void HomeScene::AddObject3DDraw()
{
	areaManager_->CSVAreaDraw();

	for(auto it = slime.begin(); it != slime.end(); it++){
		(*it)->Draw();
	}

}

void HomeScene::AddParticleDraw()
{
	for(auto it = slime.begin(); it != slime.end(); it++){
		(*it)->ParticleDraw();
	}
}

void HomeScene::AddFrontUIDraw()
{
	exitText->Draw();
	trainingText->Draw();
}

void HomeScene::AddBackUIDraw()
{
}

void HomeScene::AddObjectFinalize()
{
	exitText->Finalize();
	trainingText->Finalize();

	for(auto it = slime.begin(); it != slime.end(); it++){
		(*it)->Finalize();
	}
}

void HomeScene::AddCommonFinalize()
{
	areaManager_->CSVAreaFinalize();
}

void HomeScene::ActorCreateInitialize()
{
	for(int i = 0; i < slimePopNumMax; i++){
		unique_ptr<BlueSlime> newObj = make_unique<BlueSlime>();
		newObj->Initialize("Slime");
		slime.push_back(move(newObj));
	}
}
