#include "TrainingDummy.h"
#include "../../Collision/SphereCollider.h"

#include "../../Collision/CollisionSystem/CollisionManager.h"
#include "../../Collision/CollisionSystem/CollisionAttribute.h"

using namespace DirectX;
using namespace std;

TrainingDummy::~TrainingDummy()
{
	//Finalize();
}

void TrainingDummy::Initialize(std::string filePath, bool IsSmoothing)
{
	BaseObjObject::Initialize(filePath, IsSmoothing);

	window = Window::GetInstance();

	//サイズ変更の最小値変更
	ScaleMin = {0.7f, 0.7f, 0.7f};

	SetPosition({-7.5f,-3.5f, -2.5f});
	SetRotation({0,XMConvertToRadians(180.f),0});

	//コライダー
	float radius = 0.6f;
	SetCollider(new SphereCollider(XMVECTOR{0,0.0,0,0}, radius));
	collider->SetAttribute(COLLISION_ATTR_DUMMY);

	//パーティクル
	DeadParticle = new ParticleObject();
	DeadParticle->Initialize();

	buttonSp = make_unique<BaseSprites>();
	buttonSp->Initialize(17);
	buttonSp->SetSize({35,35});
	buttonSp->SetAnchorPoint({0.5f,0.5f});
}

void TrainingDummy::Update(Camera *camera)
{
	this->camera = camera;
	
	if(IsDead){
		if(IsDeadAudioOnce){
			IsDeadAudioOnce = false;
			appearanceResetFrame = 0;
		}
		DeadParticleApp();

		if(appearanceResetFrame >= AppearanceResetFrame){
			IsDead = false;
			SetPosition(RespawnPos);
			return;
		}
		appearanceResetFrame++;
	}
	else if(!IsDead){
		//拍終了
		if(IsBeatEnd){
			//スケール
			IsScale  = true;
			//拍終了
			IsBeatEnd = false;
		}

		//スケール遷移
		if(IsScale){
			if(ScaleChange(ScaleMax, ScaleMin, scaleEndTime)){
				IsScale = false;
			}
		}
	}
	DeadParticle->Update(this->camera);
	buttonSp->Update();
	//行列、カメラ更新
	BaseObjObject::Update(this->camera);
	//コライダー更新
	collider->Update();
}

void TrainingDummy::Draw()
{
	if(IsDead) return;

	BaseObjObject::Draw();
}

void TrainingDummy::Draw2D()
{
	buttonSp->Draw();
}

void TrainingDummy::ParticleDraw()
{
	if(IsDead){
		DeadParticle->Draw();
	}
}

void TrainingDummy::Finalize()
{
	buttonSp->Finalize();

	DeadParticle->Finalize();
	BaseObjObject::Finalize();
}

void TrainingDummy::OnCollision(const CollisionInfo &info)
{
	if(IsDead) return;

	if(info.collider->GetAttribute() == COLLISION_ATTR_WEAPONS){
		IsDead = true;
		IsDeadAudioOnce = true;
		RespawnPos = GetPosition();
		SetPosition(DeadPos);
		DeadParticlePos = info.objObject->GetPosition();
		IsDeadParticleOnce = true;
	}
	else if(info.collider->GetAttribute() == COLLISION_ATTR_ALLIES){
		RespawnPos = GetPosition();
		SetPosition(DeadPos);
		IsDead = true;
	}
}

void TrainingDummy::DeadParticleApp()
{
	if(!IsDeadParticleOnce) return;

	for (int i = 0; i < 10; i++) {
		const float rnd_vel = 0.08f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = 0.06f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		Vector3 acc{};
		acc.y = -0.005f;

		DeadParticle->ParticleSet(AppearanceResetFrame,DeadParticlePos,vel,acc,0.4f,0.0f,1,{1.f,0.0f,0.0f,1.f});
		DeadParticle->ParticleAppearance();
	}

	IsDeadParticleOnce = false;
}

Vector2 TrainingDummy::ChangeTransformation(Vector3 targetpos)
{
	DirectX::XMMATRIX matViewport = 
	{
		(float)window->GetWindowWidth()/2, 0								  , 0, 0,
		0								 , -((float)window->GetWindowHeight())/2, 0, 0,
		0								 , 0								  , 1, 0, 
		(float)window->GetWindowWidth()/2, (float)window->GetWindowHeight()/2 , 0, 1,
	};
	DirectX::XMMATRIX matViewProjectionViewPort = camera->GetMatView() * camera->GetMatProjection() * matViewport;
	Vector3 positionreticle = Vector3Transform(targetpos, matViewProjectionViewPort);
	return Vector2{positionreticle.x, positionreticle.y};
}

Vector3 TrainingDummy::Vector3Transform(Vector3 &v, DirectX::XMMATRIX &m)
{
	float w = v.x * m.r[0].m128_f32[3] + v.y * m.r[1].m128_f32[3] + v.z * m.r[2].m128_f32[3] + m.r[3].m128_f32[3];

	Vector3 result
	{
		(v.x*m.r[0].m128_f32[0] + v.y*m.r[1].m128_f32[0] + v.z*m.r[2].m128_f32[0] + m.r[3].m128_f32[0])/w,
		(v.x*m.r[0].m128_f32[1] + v.y*m.r[1].m128_f32[1] + v.z*m.r[2].m128_f32[1] + m.r[3].m128_f32[1])/w,
		(v.x*m.r[0].m128_f32[2] + v.y*m.r[1].m128_f32[2] + v.z*m.r[2].m128_f32[2] + m.r[3].m128_f32[2])/w
	};

	return result;
}
