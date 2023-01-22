#include "AreaOutRock.h"

AreaOutRock::~AreaOutRock()
{
	//Finalize();
}

void AreaOutRock::Initialize(std::string filePath, bool IsSmmothing)
{
	BaseObjObject::Initialize(filePath, IsSmmothing);

	SetModel(model);

	//初期化
	fallV = {};
	IsAlive = false;
	ReSpawnTime();
	ScaleRandom();

	//パーティクル
	PopParticle = new ParticleObject();
	PopParticle->Initialize();
}

void AreaOutRock::Update(Camera *camera)
{
	this->camera = camera;

	//死亡
	if(!IsAlive){
		spawnFame++;

		//パーティクル出現
		if(spawnFame % 2 == 0){
			//パーティクル出現
			CreateParticle();
		}

		//岩の出現
		if(spawnFame >= MaxSpawnFrame){
			RandomSpawn();
			ReSpawnTime();
			ScaleRandom();
			fallV = {};
			spawnFame = 0;

			IsAlive = true;
		}
	}


	//落下地点測定
	if(world.translation.y <= LowestPoint){
		IsAlive = false;
	}

	//回転
	world.rotation.x += DirectX::XMConvertToRadians(5.f);
	world.rotation.y += DirectX::XMConvertToRadians(5.f);
	world.rotation.z += DirectX::XMConvertToRadians(5.f);

	//重力
	GravityFall();

	PopParticle->Update(this->camera);
	BaseObjObject::Update(this->camera);
}

void AreaOutRock::Draw()
{
	if(!IsAlive) return;

	BaseObjObject::Draw();
}

void AreaOutRock::ParticleDraw()
{
	PopParticle->Draw();
}

void AreaOutRock::Finalize()
{
	PopParticle->Finalize();
	BaseObjObject::Finalize();
}

void AreaOutRock::GravityFall()
{
	if(!IsAlive) return;

	//下向き加速度
	const float fallAcc = -0.01f;
	const float fallVYMin = -0.5f;
	//加速
	fallV.y = max(fallV.y + fallAcc, fallVYMin);
	//移動
	world.translation.x += fallV.x;
	world.translation.y += fallV.y;
	world.translation.z += fallV.z;
}

void AreaOutRock::RandomSpawn()
{
	//座標
	int randamX = rand()%70 - 35;//-35 ~ 35
	int randamZ = rand()%20 + 15;//15 ~ 35
	world.translation.x = -static_cast<float>(randamX);
	world.translation.y = 15;
	world.translation.z = static_cast<float>(randamZ);

	//回転
	world.rotation = {0,0,0};
}

void AreaOutRock::ReSpawnTime()
{
	MaxSpawnFrame = rand()%200 + 100;
}

void AreaOutRock::ScaleRandom()
{
	int scalerand = rand()%101;
	float scale = (float)scalerand/100.f;
	world.scale = {scale,scale,scale};
}

void AreaOutRock::CreateParticle()
{
		// X,Y,Z全て[-35.0f,+35.0f]でランダムに分布
		const float rnd_pos = 70.0f;
		Vector3 pos{};
		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y = 15;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		//移動速度
		const float rnd_vel = 0.015f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = -0.025f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		//加速度
		Vector3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		PopParticle->ParticleSet(300, pos,vel,acc, 0.1f,0.1f, 1, {0.6f,0.3f,0.2f,0.4f});
		PopParticle->ParticleAppearance();
}
