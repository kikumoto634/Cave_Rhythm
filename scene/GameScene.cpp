#include "GameScene.h"

#include "SceneManager.h"
#include "HomeScene.h"

using namespace std;

void GameScene::NextSceneChange()
{
	sceneManager->SetNextScene(new HomeScene(dxCommon,window,player->GetHP()));
}

GameScene::GameScene(DirectXCommon *dxCommon, Window *window, int saveHP)
	: BaseGameScene(
		dxCommon,
		window)
{
	//
	//this->saveHP = saveHP;
}

void GameScene::AddCommonInitialize()
{
	//BGM再生
	IsBGMStart = true;
}

void GameScene::AddObject3DInitialize()
{
	for(int i = 0; i < IniCreateEnemyNum; i++){
		EnemyInitPop();
	}

	for(int i = 0; i < IniCreateCoinNum; i++){
		CoinInitPop();
	}

	exit->SetExitOpenNeedCoin(10);
	exit->NeedCoinSpriteUpdate();
}

void GameScene::AddObject2DInitialize()
{
}

void GameScene::AddInputUpdate()
{
}

void GameScene::AddObject3DUpdate()
{
	//エネミー
	for(auto it = enemy.begin(); it != enemy.end(); it++){
		if(!(*it)->GetIsNotApp()){
			if((*it)->GetIsDeadAudioOnce())	{
				gameManager->AudioPlay(2,0.2f);
				for(auto it2 = coin.begin(); it2!= coin.end(); it2++){
					if((*it2)->PopPossible()){
						(*it2)->Pop({(*it)->GetDeadParticlePos().x, -5, (*it)->GetDeadParticlePos().z});
						break;
					}
				}
			}
			(*it)->Update(camera);
		}
	}
	for(auto it = enemy2.begin(); it != enemy2.end(); it++){
		if(!(*it)->GetIsNotApp()){
			if((*it)->GetIsDeadAudioOnce())	{
				gameManager->AudioPlay(2,0.2f);
				for(auto it2 = coin.begin(); it2!= coin.end(); it2++){
					if((*it2)->PopPossible()){
						(*it2)->Pop({(*it)->GetDeadParticlePos().x, -5, (*it)->GetDeadParticlePos().z});
						break;
					}
				}
			}
			(*it)->Update(camera,player->GetPosition());
		}
	}
	//コイン
	for(auto it = coin.begin(); it != coin.end(); it++){
		if((*it)->GetCoin()){
			gameManager->CoinIncrement();
			gameManager->AudioPlay(7,0.5f);
		}
		(*it)->Update(camera);
	}
}

void GameScene::AddObject2DUpdate()
{
}

void GameScene::AddCommonUpdate()
{
}

void GameScene::AddBeatEndUpdate()
{
	//各オブジェクト処理
	if(!player->GetIsDead())player->IsBeatEndOn();
	for(auto it = enemy.begin(); it != enemy.end(); it++){
		(*it)->IsBeatEndOn();
	}
	for(auto it = enemy2.begin(); it != enemy2.end(); it++){
		(*it)->IsBeatEndOn();
	}
	for(auto it = coin.begin(); it != coin.end(); it++){
		if((*it)->GetIsAlive())(*it)->IsBeatEndOn();
	}

	int randomPop = rand()%3;
	//敵生成
	if(randomPop == 0 || randomPop == 1){
		for(int i = 0; i < gameManager->EnemyPopTurnCount(); i++){
			//座標;
			Vector2 lpos = gameManager->EnemyRandomPos(DIV_NUM, Plane_Size);
			//方向
			Vector2 ldir = gameManager->EnemyRandomDir(lpos);
			//POP
			EnemyPop(lpos, ldir);
		}
	}
	else{
		for(int i = 0; i < gameManager->EnemyPopTurnCount(); i++){
			//座標;
			Vector2 lpos = gameManager->EnemyRandomPos(DIV_NUM, Plane_Size);
			//POP
			Enemy2Pop(lpos);
		}
	}
}

void GameScene::AddObject3DDraw()
{
	for(auto it = enemy.begin(); it != enemy.end(); it++){
		(*it)->Draw();
	}
	for(auto it = enemy2.begin(); it != enemy2.end(); it++){
		(*it)->Draw();
	}
	for(auto it = coin.begin(); it != coin.end(); it++){
		(*it)->Draw();
	}
}

void GameScene::AddObject2DDraw()
{
}

void GameScene::AddParticleDraw()
{
	for(auto it = enemy.begin(); it != enemy.end(); it++){
		(*it)->ParticleDraw();
	}
	for(auto it = enemy2.begin(); it != enemy2.end(); it++){
		(*it)->ParticleDraw();
	}
}

void GameScene::AddUIDraw()
{
}

void GameScene::AddObjectFinalize()
{
	for(auto it = enemy.begin(); it != enemy.end(); it++){
		(*it)->Finalize();
	}
	for(auto it = enemy2.begin(); it != enemy2.end(); it++){
		(*it)->Finalize();
	}
	for(auto it = coin.begin(); it != coin.end(); it++){
		(*it)->Finalize();
	}
}

void GameScene::AddCommonFinalize()
{
}

#ifdef _DEBUG
void GameScene::AddDebugUpdate()
{
	//座標
	{
		ImGui::SetNextWindowPos(ImVec2{0,250});
		//サイズ
		ImGui::SetNextWindowSize(ImVec2{300,300});
		ImGui::Begin("EnemyPop");

		ImGui::Text("POP:pos dir");
		ImGui::InputInt2("EnemyPos X,Z : 0~11", popPosition);
		ImGui::InputInt2("EnemyDir X,Z : -1~1", popDirection);

		//敵の出現
		ImGui::Text("EnemyPOP");
		//生成
		if (ImGui::Button("POP1")) {
			//座標;
			Vector2 pos = {(-12.5f + (float)popPosition[0]*2.5f), (-12.5f + (float)popPosition[1]*2.5f)};
			//方向
			if(popDirection[0] >= 1)popDirection[0] = 1;
			else if(popDirection[0] <= -1)popDirection[0] = -1;
			if(popDirection[1] >= 1)popDirection[1] = 1;
			else if(popDirection[1] <= -1)popDirection[1] = -1;
			Vector2 dir = {(float)popDirection[0], (float)popDirection[1]};
			//POP
			EnemyPop(pos, dir);
		}

		//敵2の出現
		ImGui::Text("Enemy2POP");
		//生成
		if (ImGui::Button("POP2")) {
			//座標;
			Vector2 pos = {(-12.5f + (float)popPosition[0]*2.5f), (-12.5f + (float)popPosition[1]*2.5f)};
			//POP
			Enemy2Pop(pos);
		}
		ImGui::End();
	}
}

void GameScene::AddDebugDraw()
{
	debugText->Printf(1000,0,1.0f, "EnemyNum : %d", enemy.size());
	debugText->Printf(1000,20,1.0f, "Enemy2Num : %d", enemy2.size());
	debugText->Printf(1000,40,1.0f, "CoinNum : %d", coin.size());
}

void GameScene::AddDebugFinalize()
{
}

#endif // _DEBUG

void GameScene::EnemyInitPop()
{
	unique_ptr<Enemy> newObj = make_unique<Enemy>();
	newObj->Initialize("slime");
	enemy.push_back(move(newObj));

	unique_ptr<Enemy2> newObj2 = make_unique<Enemy2>();
	newObj2->Initialize("Skeleton");
	enemy2.push_back(move(newObj2));
}

void GameScene::EnemyPop(Vector2 pos, Vector2 dir)
{
	Vector3 lpos = {pos.x, -3.5f, pos.y};
	Vector3 ldir = {dir.x, 0, dir.y};

	for(auto it = enemy.begin(); it != enemy.end(); it++){
		if((*it)->GetIsNotApp()){
			(*it)->SetPopPoasition(lpos);
			(*it)->SetDirection(ldir);
			(*it)->BeginAppearance();
			break;
		}
	}
}

void GameScene::Enemy2Pop(Vector2 pos)
{
	Vector3 lpos = {pos.x, -3.5f, pos.y};

	for(auto it = enemy2.begin(); it != enemy2.end(); it++){
		if((*it)->GetIsNotApp()){
			(*it)->SetPopPoasition(lpos);
			(*it)->BeginAppearance();
			break;
		}
	}
}

void GameScene::CoinInitPop()
{
	unique_ptr<Coins> newObj = make_unique<Coins>();
	newObj->Initialize("Coins");
	coin.push_back(move(newObj));
}