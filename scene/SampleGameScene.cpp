#include "SampleGameScene.h"

#include "SceneManager.h"
#include "HomeScene.h"

using namespace std;

SampleGameScene::SampleGameScene(DirectXCommon *dxCommon, Window *window, int saveHP)
	: BaseGameScene(
		dxCommon,
		window)
{
	this->saveHP = saveHP;
}

void SampleGameScene::NextSceneChange()
{
	sceneManager->SetNextScene(new HomeScene(dxCommon,window, 5));
}

void SampleGameScene::AddCommonInitialize()
{
	//BGM�Đ�
	IsBGMStart = true;
}

void SampleGameScene::AddObject3DInitialize()
{
	coin = make_unique<Coins>();
	coin->Initialize("Coins");

	dummy = make_unique<TrainingDummy>();
	dummy->Initialize("Dummy");

	enemy = make_unique<Enemy>();
	enemy->Initialize("Slime");
	enemy2 = make_unique<Enemy2>();
	enemy2->Initialize("Skeleton");
}

void SampleGameScene::AddObject2DInitialize()
{
}

void SampleGameScene::AddInputUpdate()
{
}

void SampleGameScene::AddObject3DUpdate()
{
	//�_�~�[
	if(dummy->GetIsDeadAudioOnce())	{
		gameManager->AudioPlay(2,0.2f);
		if(coin->PopPossible()){
			coin->Pop({dummy->GetDeadParticlePos().x, -5 , dummy->GetDeadParticlePos().z});
		}
	}
	dummy->Update(camera);

	//�G�l�~�[
	if(!enemy->GetIsNotApp()){
		if(enemy->GetIsDeadAudioOnce())	{
			gameManager->AudioPlay(2,0.2f);
			if(coin->PopPossible()){
				coin->Pop({enemy->GetDeadParticlePos().x, -5 , enemy->GetDeadParticlePos().z});
			}
		}
		enemy->Update(camera);
	}

	//�G�l�~�[2
	if(!enemy2->GetIsNotApp()){
		if(enemy2->GetIsDeadAudioOnce())	{
			gameManager->AudioPlay(2,0.2f);
			if(coin->PopPossible()){
				coin->Pop({enemy2->GetDeadParticlePos().x, -5 , enemy2->GetDeadParticlePos().z});
			}
		}
		enemy2->Update(camera, player->GetPosition());
	}


	if(coin->GetCoin()){
		gameManager->CoinIncrement();
		gameManager->AudioPlay(7,0.5f);
	}
	coin->Update(camera);
}

void SampleGameScene::AddObject2DUpdate()
{
}

void SampleGameScene::AddCommonUpdate()
{
}

void SampleGameScene::AddBeatEndUpdate()
{
	if(coin->GetIsAlive())	coin->IsBeatEndOn();

	dummy->IsBeatEndOn();
	enemy->IsBeatEndOn();
	enemy2->IsBeatEndOn();
}

void SampleGameScene::AddObject3DDraw()
{
	dummy->Draw();
	enemy->Draw();
	enemy2->Draw();

	coin->Draw();
}

void SampleGameScene::AddObject2DDraw()
{
}

void SampleGameScene::AddParticleDraw()
{
	dummy->ParticleDraw();
	enemy->ParticleDraw();
	enemy2->ParticleDraw();
}

void SampleGameScene::AddUIDraw()
{
	dummy->Draw2D();
}

void SampleGameScene::AddObjectFinalize()
{
	dummy->Finalize();
	enemy->Finalize();
	enemy2->Finalize();

	coin->Finalize();
}

void SampleGameScene::AddCommonFinalize()
{
}

#ifdef _DEBUG
void SampleGameScene::AddDebugUpdate()
{
	//���W
	{
		ImGui::SetNextWindowPos(ImVec2{0,250});
		//�T�C�Y
		ImGui::SetNextWindowSize(ImVec2{300,300});
		ImGui::Begin("EnemyPop");

		ImGui::Text("POP:pos dir");
		ImGui::InputInt2("EnemyPos X,Z : 0~11", popPosition);
		ImGui::InputInt2("EnemyDir X,Z : -1~1", popDirection);

		//�G�̏o��
		ImGui::Text("EnemyPOP");
		//����
		if (ImGui::Button("POP1")) {
			//���W;
			Vector2 pos = {(-12.5f + (float)popPosition[0]*2.5f), (-12.5f + (float)popPosition[1]*2.5f)};
			//����
			if(popDirection[0] >= 1)popDirection[0] = 1;
			else if(popDirection[0] <= -1)popDirection[0] = -1;
			if(popDirection[1] >= 1)popDirection[1] = 1;
			else if(popDirection[1] <= -1)popDirection[1] = -1;
			Vector2 dir = {(float)popDirection[0], (float)popDirection[1]};
			//POP
			Enemy1Pop(pos, dir);
		}

		//�G2�̏o��
		ImGui::Text("Enemy2POP");
		//����
		if (ImGui::Button("POP2")) {
			//���W;
			Vector2 pos = {(-12.5f + (float)popPosition[0]*2.5f), (-12.5f + (float)popPosition[1]*2.5f)};
			//POP
			Enemy2Pop(pos);
		}
		ImGui::End();
	}
}

void SampleGameScene::AddDebugDraw()
{
}

void SampleGameScene::AddDebugFinalize()
{
}

#endif // _DEBUG

void SampleGameScene::Enemy1Pop(Vector2 pos, Vector2 dir)
{
	Vector3 lpos = {pos.x, -3.5f, pos.y};
	Vector3 ldir = {dir.x, 0, dir.y};

	if(enemy->GetIsNotApp()){
		enemy->SetPopPoasition(lpos);
		enemy->SetDirection(ldir);
		enemy->BeginAppearance();
	}
}

void SampleGameScene::Enemy2Pop(Vector2 pos)
{
	Vector3 lpos = {pos.x, -3.5f, pos.y};

	if(enemy2->GetIsNotApp()){
		enemy2->SetPopPoasition(lpos);
		enemy2->BeginAppearance();
	}
}