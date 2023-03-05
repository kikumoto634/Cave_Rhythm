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
	//ƒ_ƒ“ƒWƒ‡ƒ“
	areaManager->CSVAreaInitialize("Home");
}

void HomeScene::AddObject3DInitialize()
{
	exit->SetExitOpenNeedCoin(0);
	exit->NeedCoinSpriteUpdate();

	ActorCreateInitialize();
}

void HomeScene::AddObject2DInitialize()
{
	{
		exitText = make_unique<TutorialSp>();
		exitText->Initialize(25);
		Vector3 ltarget = exitTextPos;
		Vector2 lpos = exitText->ChangeTransformation(ltarget, this->camera);
		exitText->SetPosition(lpos);
		exitText->SetSize({200,40});
		exitText->SetAnchorPoint({0.5f,0.5f});
	}
	
	{
		trainingText = make_unique<TutorialSp>();
		trainingText->Initialize(26);
		Vector3 ltarget = trainingTextPos;
		Vector2 lpos = exitText->ChangeTransformation(ltarget, this->camera);
		trainingText->SetPosition(lpos);
		trainingText->SetSize({200,40});
		trainingText->SetAnchorPoint({0.5f,0.5f});
	}
}

void HomeScene::AddCommonUpdate()
{
	areaManager->CSVAreaUpdate(camera, player->GetPosition());
}

void HomeScene::AddObject3DUpdate()
{
	for(auto it = slime.begin(); it != slime.end(); it++){
		if((*it)->GetIsDeadAudio()){
			gameManager->AudioPlay(2, 0.5f);
		}
		(*it)->Update(camera,player->GetPosition());
	}
}

void HomeScene::AddObject2DUpdate()
{
	{
		Vector3 ltarget = exitTextPos;
		Vector2 lpos = exitText->ChangeTransformation(ltarget, this->camera);
		exitText->SetPosition(lpos);
		exitText->SetPlayerPos(player->GetPosition());
		exitText->Update();
	}
	{
		Vector3 ltarget = trainingTextPos;
		Vector2 lpos = trainingText->ChangeTransformation(ltarget, this->camera);
		trainingText->SetPosition(lpos);
		trainingText->SetPlayerPos(player->GetPosition());
		trainingText->Update();
	}
}

void HomeScene::AddBeatEndUpdate()
{
	int index = 0;
	Vector3 lpos;
	for(auto it = slime.begin(); it != slime.end(); it++){
		(*it)->IsBeatEndOn();
		if(!(*it)->GetIsNotApp()){
			if(areaManager->GetCSVObjectPopActive(index)) {
				lpos = areaManager->GetCSVObjectPopPosition(index);
				(*it)->Pop({lpos.x, -3.5f,lpos.z});
			}
			index++;
		}
	}
}

void HomeScene::AddObject3DDraw()
{
	areaManager->CSVAreaDraw();

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

void HomeScene::AddUIDraw()
{
	exitText->Draw();
	trainingText->Draw();
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
	areaManager->CSVAreaFinalize();
}

void HomeScene::ActorCreateInitialize()
{
	for(int i = 0; i < slimePopNumMax; i++){
		unique_ptr<BlueSlime> newObj = make_unique<BlueSlime>();
		newObj->Initialize("Slime");
		slime.push_back(move(newObj));
	}
}
