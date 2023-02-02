#include "HomeScene.h"

#include "SceneManager.h"
#include "HomeScene.h"
#include "GameScene.h"

using namespace std;


HomeScene::HomeScene(DirectXCommon *dxCommon, Window *window, int saveHP)
	:BaseGameScene{
	dxCommon,
	window}
{
	//HPの引継ぎはしない
	//this->saveHP = saveHP;
}

void HomeScene::NextSceneChange()
{
	sceneManager->SetNextScene(new GameScene(dxCommon,window));
}

void HomeScene::AddCommonInitialize()
{
}

void HomeScene::AddObject3DInitialize()
{
	coin = make_unique<Coins>();
	coin->Initialize("Coins");

	dummy = make_unique<TrainingDummy>();
	dummy->Initialize("Dummy");
}

void HomeScene::AddObject2DInitialize()
{
	for(int i = 0; i < 4; i++){
		buttonSp[i] = make_unique<BaseSprites>();
		buttonSp[i]->Initialize(20 + i);
		buttonSp[i]->SetAnchorPoint({0.5f,0.5f});
		buttonSp[i]->SetSize({50,50});
		IsButtonPush[i] = false;
	}
	buttonSp[0]->SetPosition({1150,300});
	buttonSp[1]->SetPosition(buttonSp[0]->GetPosition() - Vector2{60,0});
	buttonSp[2]->SetPosition(buttonSp[0]->GetPosition() - Vector2{0,60});
	buttonSp[3]->SetPosition(buttonSp[0]->GetPosition() - Vector2{-60,0});
}

void HomeScene::AddInputUpdate()
{
	//おまけボタン色つけ
	if(input->Push(DIK_DOWN)){
		IsButtonPush[0] = true;
	}
	else if(input->Push(DIK_LEFT)){
		IsButtonPush[1] = true;
	}
	else if(input->Push(DIK_UP)){
		IsButtonPush[2] = true;
	}
	else if(input->Push(DIK_RIGHT)){
		IsButtonPush[3] = true;
	}
}

void HomeScene::AddObject3DUpdate()
{
	//ダミー
	if(dummy->GetIsDeadAudioOnce())	{
		gameManager->AudioPlay(2,0.2f);
		if(coin->PopPossible()){
			coin->Pop({dummy->GetDeadParticlePos().x, -5 , dummy->GetDeadParticlePos().z});
		}
	}
	dummy->Update(camera);
	{
		Vector3 target = dummy->GetPosition() + Vector3{-1, 2, 0};
		Vector2 pos = dummy->ChangeTransformation(target);
		dummy->SetButtonSpPosition(pos);
	}

	if(coin->GetCoin()){
		gameManager->CoinIncrement();
		gameManager->AudioPlay(7,0.5f);
	}
	coin->Update(camera);
}

void HomeScene::AddObject2DUpdate()
{
	for(int i= 0; i < 4; i++){
		buttonSp[i]->Update();
		
		if(IsButtonPush[i]){
			buttonSp[i]->SetColor({0.2f,0.2f,0.2f,1.0f});
		}
		else{
			buttonSp[i]->SetColor({1.0f,1.0f,1.0f,1.0f});
		}
		IsButtonPush[i] = false;
	}
}

void HomeScene::AddCommonUpdate()
{
}

void HomeScene::AddBeatEndUpdate()
{
	if(coin->GetIsAlive())	coin->IsBeatEndOn();
	dummy->IsBeatEndOn();
}

void HomeScene::AddObject3DDraw()
{
	dummy->Draw();
	coin->Draw();

}

void HomeScene::AddObject2DDraw()
{
	dummy->Draw2D();

	for(int i = 0; i < 4; i++){
		buttonSp[i]->Draw();
	}
}

void HomeScene::AddParticleDraw()
{
	dummy->ParticleDraw();
	rock->ParticleDraw();
}

void HomeScene::AddUIDraw()
{
}

void HomeScene::AddObjectFinalize()
{
	dummy->Finalize();
	coin->Finalize();

	for(int i = 0; i < 4; i++){
		buttonSp[i]->Finalize();
	}
}

void HomeScene::AddCommonFinalize()
{
}

void HomeScene::AddDebugUpdate()
{
}

void HomeScene::AddDebugDraw()
{
}

void HomeScene::AddDebugFinalize()
{
}
