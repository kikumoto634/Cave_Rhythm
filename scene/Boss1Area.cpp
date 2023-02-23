#include "Boss1Area.h"
#include "HomeScene.h"
#include "SceneManager.h"

#include "../Engine/math/Easing/Easing.h"

using namespace std;

Boss1Area::~Boss1Area()
{
	Finalize();
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
	if(IsPrevSceneChange){

		//画面が開く
		{
			if(fadeColor.w <= 0){
				IsPrevSceneChange = false;
				fadeCurrentFrame = 0;
				//リズム
				rhythmManager->InitializeMeasurement(clock());
				return;
			}

			fadeColor.w = 
				Easing_Linear_Point2(1,0,Time_OneWay(fadeCurrentFrame, FadeSecond/2));
			fade->SetColor(fadeColor);
			fade->Update();
		}
	}
	//NextSceneへの移動
	else if(IsNextSceneChange || IsGameEnd){

		if(fadeColor.w >= 1){
			camera->Reset();
			if(IsNextSceneChange)NextSceneChange();
			else if(IsGameEnd)	SceneGameEnd();
		}

		fadeColor.w = 
			Easing_Linear_Point2(0,1,Time_OneWay(fadeCurrentFrame, FadeSecond));
		fade->SetColor(fadeColor);
		fade->Update();
	}
}

void Boss1Area::AddCommonInitialize()
{
	//ダンジョン
	areaManager->CSVAreaInitialize("Boss1");
}

void Boss1Area::AddObject3DInitialize()
{
	boss = make_unique<Boss1>();
	boss->Initialize("Skeleton");
	Vector3 lpos = areaManager->GetCSVObjectPopPosition(0);
	boss->Pop({lpos.x, -2.f,lpos.z});
}

void Boss1Area::AddObject2DInitialize()
{
}

void Boss1Area::AddCommonUpdate()
{
	areaManager->CSVAreaUpdate(camera, player->GetPosition());
}

void Boss1Area::AddObject3DUpdate()
{
	if(boss->GetIsDeadAudio()){
		gameManager->AudioPlay(2, 0.5f);
	}
	boss->Update(camera,player->GetPosition());
}

void Boss1Area::AddObject2DUpdate()
{
}

void Boss1Area::AddBeatEndUpdate()
{
	boss->IsBeatEndOn();
}

void Boss1Area::AddObject3DDraw()
{
	areaManager->CSVAreaDraw();

	boss->Draw();
}

void Boss1Area::AddParticleDraw()
{
	boss->ParticleDraw();
}

void Boss1Area::AddUIDraw()
{
}

void Boss1Area::AddObjectFinalize()
{
	boss->Finalize();
}

void Boss1Area::AddCommonFinalize()
{
	areaManager->CSVAreaFinalize();
	delete areaManager;
	areaManager = nullptr;
}