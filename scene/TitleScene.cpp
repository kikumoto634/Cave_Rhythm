#include "TitleScene.h"

#include "SceneManager.h"
#include "GameScene.h"

using namespace std;
using namespace DirectX;

TitleScene::TitleScene(DirectXCommon *dxCommon, Window *window)
	: BaseScene(
		dxCommon,
		window)
{
}

void TitleScene::Application()
{
	BaseScene::Application();
}

void TitleScene::Initialize()
{
	BaseScene::Initialize();

	//�v���C���[
	playerobj = make_unique<BaseObjObject>();
	playerobj->Initialize("human2");
	playerobj->SetPosition({-5.5f, -2.f, 0.f});
	playerobj->SetRotation({0.f, XMConvertToRadians(121), 0.f});
	playerobj->SetScale({5,5,5});

	//�G�l�~�[
	enemyobj = make_unique<BaseObjObject>();
	enemyobj->Initialize("slime");
	enemyobj->SetPosition({7.f, 0.f, 0.f});
	enemyobj->SetRotation({0.f, XMConvertToRadians(220), 0.f});
	enemyobj->SetScale({3,3,3});


	//���C�g
	lightGroup = LightGroup::Create();
	//�F�ݒ�
	lightGroup->SetAmbientColor({0.15f, 0.15f, 0.15f});
	//3D�I�u�W�F�N�g(.obj)�ɃZ�b�g
	ObjModelObject::SetLight(lightGroup);

	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, true);
}

void TitleScene::Update()
{
	BaseScene::Update();

#pragma region Input Update
	//ENTER
	if(input->Trigger(DIK_RETURN)){
		sceneManager->SetNextScene(new GameScene(dxCommon,window));
	}

#pragma endregion

#pragma region _3DObj Update
	playerobj->Update(camera);
	enemyobj->Update(camera);
#pragma endregion

#pragma region Common Update
	lightGroup->Update();
#pragma endregion

#ifdef _DEBUG
	//�v���C���[
	{
		//���W
		ImGui::SetNextWindowPos(ImVec2{0,100});
		//�T�C�Y
		ImGui::SetNextWindowSize(ImVec2{300,100});
		ImGui::Begin("Player");
		{
			//���W
			Vector3 lpos = playerobj->GetPosition();
			float pos[3] = {lpos.x, lpos.y, lpos.z};
			ImGui::DragFloat3("Position", pos, 0.05f);
			lpos = {pos[0],pos[1],pos[2]};
			playerobj->SetPosition(lpos);
		}
		{
			//��]
			Vector3 lrot = playerobj->GetRotation();
			float rot[3] = {XMConvertToDegrees(lrot.x), XMConvertToDegrees(lrot.y), XMConvertToDegrees(lrot.z)};
			ImGui::DragFloat3("Rotation", rot);
			lrot = {XMConvertToRadians(rot[0]),XMConvertToRadians(rot[1]),XMConvertToRadians(rot[2])};
			playerobj->SetRotation(lrot);
		}
		{
			//�X�P�[��
			Vector3 lscale = playerobj->GetScale();
			float scale[3] = {lscale.x, lscale.y, lscale.z};
			ImGui::InputFloat3("Scale", scale);
			lscale = {scale[0],scale[1],scale[2]};
			playerobj->SetScale(lscale);
		}
		ImGui::End();
	}

	//�G�l�~�[
	{
		//���W
		ImGui::SetNextWindowPos(ImVec2{0,200});
		//�T�C�Y
		ImGui::SetNextWindowSize(ImVec2{300,100});
		ImGui::Begin("Enemy");
		{
			//���W
			Vector3 lpos = enemyobj->GetPosition();
			float pos[3] = {lpos.x, lpos.y, lpos.z};
			ImGui::DragFloat3("Position", pos, 0.05f);
			lpos = {pos[0],pos[1],pos[2]};
			enemyobj->SetPosition(lpos);
		}
		{
			//��]
			Vector3 lrot = enemyobj->GetRotation();
			float rot[3] = {XMConvertToDegrees(lrot.x), XMConvertToDegrees(lrot.y), XMConvertToDegrees(lrot.z)};
			ImGui::DragFloat3("Rotation", rot);
			lrot = {XMConvertToRadians(rot[0]),XMConvertToRadians(rot[1]),XMConvertToRadians(rot[2])};
			enemyobj->SetRotation(lrot);
		}
		{
			//�X�P�[��
			Vector3 lscale = enemyobj->GetScale();
			float scale[3] = {lscale.x, lscale.y, lscale.z};
			ImGui::InputFloat3("Scale", scale);
			lscale = {scale[0],scale[1],scale[2]};
			enemyobj->SetScale(lscale);
		}
		ImGui::End();
	}

#endif // _DEBUG


	BaseScene::EndUpdate();
}

void TitleScene::Draw()
{

	BaseScene::Draw();

#pragma region _3DObj Draw
	playerobj->Draw();
	enemyobj->Draw();
#pragma endregion

#pragma region Common Draw

#pragma endregion

	BaseScene::EndDraw();
}

void TitleScene::Finalize()
{
	delete lightGroup;
	lightGroup = nullptr;

	enemyobj->Finalize();
	playerobj->Finalize();

	BaseScene::Finalize();
}
