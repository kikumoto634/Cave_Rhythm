#include "GameManager.h"
#include "../../Engine/3D/ObjModelObject.h"

using namespace std;

void GameManager::Initialize()
{
	AudioInitialize();
	LightInitialize();
}

void GameManager::Finalize()
{
	delete lightGroup;
	lightGroup = nullptr;

	delete audio;
	audio = nullptr;
}

void GameManager::ComboIncrement()
{
	comboNum += 1;
}

void GameManager::ComboReset()
{
	comboNum = 0;
	AudioPlay(1,0.2f);
}

void GameManager::AudioAdd(int number, std::string path)
{
	string fullPath = "Resources/sound/" + path;

	audio->LoadWave(number, fullPath.c_str());
}

void GameManager::AudioPlay(int number, float volume, bool loop)
{
	audio->PlayWave(number, volume, loop);
}

void GameManager::LightUpdate()
{
	{	
		//�ۉe
		lightGroup->SetCircleShadowDir(0, {circleShadowDir.x, circleShadowDir.y, circleShadowDir.z, 0});
		lightGroup->SetCircleShadowAtten(0, circleShadowAtten);
		lightGroup->SetCircleShadowFactorAngle(0, circleShadowFactorAngle);
	}
	lightGroup->Update();
}

void GameManager::PlayerCircleShadowSet(Vector3 pos)
{
	//�v���C���[�A�ۉe���W
	lightGroup->SetCircleShadowCasterPos(0, pos);
}


void GameManager::AudioInitialize()
{
	audio = new Audio();
	audio->Initialize();
}

void GameManager::LightInitialize()
{
	//���C�g
	lightGroup = LightGroup::Create();
	//�F�ݒ�
	lightGroup->SetAmbientColor({0.15f, 0.15f, 0.15f});
	//3D�I�u�W�F�N�g(.obj)�ɃZ�b�g
	ObjModelObject::SetLight(lightGroup);

	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, false);
	lightGroup->SetDirLightActive(2, false);

	//�ۉe
	lightGroup->SetCircleShadowActive(0, true);
}
