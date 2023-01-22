#include "EventScene.h"

#include "SceneManager.h"
#include "TitleScene.h"

using namespace std;
using namespace DirectX;

EventScene::EventScene(DirectXCommon *dxCommon, Window *window)
	: BaseScene(
		dxCommon,
		window)
{
}

void EventScene::Application()
{
	BaseScene::Application();
}

void EventScene::Initialize()
{
	BaseScene::Initialize();

	//�v���C���[
	playerobj = make_unique<BaseObjObject>();
	playerobj->Initialize("human2");
	playerobj->SetPosition({-5.5f, -2.f, 0.f});
	playerobj->SetRotation({0.f, XMConvertToRadians(121), 0.f});
	playerobj->SetScale({5,5,5});


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

void EventScene::Update()
{
	BaseScene::Update();

#pragma region Input Update
	//ENTER
	if(input->Trigger(DIK_RETURN)){
		sceneManager->SetNextScene(new TitleScene(dxCommon,window));
	}

#pragma endregion

#pragma region _3DObj Update
	playerobj->Update(camera);
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

#endif // _DEBUG


	BaseScene::EndUpdate();
}

void EventScene::Draw()
{

	BaseScene::Draw();

#pragma region _3DObj Draw
	playerobj->Draw();
#pragma endregion

#pragma region Common Draw

#pragma endregion

	BaseScene::EndDraw();
}

void EventScene::Finalize()
{
	delete lightGroup;
	lightGroup = nullptr;

	playerobj->Finalize();

	BaseScene::Finalize();
}
