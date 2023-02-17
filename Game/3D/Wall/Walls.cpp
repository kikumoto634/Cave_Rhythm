#include "Walls.h"
#include "../../Collision/SphereCollider.h"
#include "../../Collision/CollisionSystem/CollisionManager.h"

using namespace DirectX;

Walls::~Walls()
{
	//Finalize();
}

void Walls::Initialize(ObjModelManager* model)
{
	BaseObjObject::Initialize(model);

	DigParticle = new ParticleObject();
	DigParticle->Initialize();
}

void Walls::Update(Camera *camera)
{
	this->camera = camera;
	IsDigSound = false;

	if(IsDig && !IsAlive){
		DigParticlePop();
		IsDig = false;
	}
	else if(IsDigApp){
		if(paricleApperanceFrame >= DigAppearanceFrame){
			IsDigApp = false;
			paricleApperanceFrame = 0;
		}
		paricleApperanceFrame++;
		DigParticle->Update(this->camera);
	}

	if(!IsAlive) return;
	Vector3 pos = PlayerPos - world.translation;
	distance = pos.length();

	if(-13 <= distance && distance <= 13)		{
		IsHide = true;

		if(!IsCollision){
			//コライダー追加
			MeshCollider* collider = new MeshCollider;
			SetCollider(collider);
			//属性セット
			collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
			collider->ConstructTriangles(colliderModel);
			IsCollision = true;
		}
	}
	else if(-13 > distance || distance > 13){
		IsHide = false;

		if(IsCollision){
			if(collider){
				//コリジョンマネージャーから登録を解除する
				CollisionManager::GetInstance()->RemoveCollider(collider);
			}
			IsCollision = false;
		}
	}
	

	if(!IsHide) return;
	BaseObjObject::Update(this->camera);
}

void Walls::Draw()
{
	if(!IsAlive) return;
	if(!IsHide) return;
	BaseObjObject::Draw();
}

void Walls::ParticleDraw()
{
	if(IsDigApp){
		DigParticle->Draw();
	}
}

void Walls::Finalize()
{
	DigParticle->Finalize();
	delete DigParticle;
	BaseObjObject::Finalize();
}

void Walls::OnCollision(const CollisionInfo &info)
{
	if(!IsAlive) return;
	if(!IsHide) return;

	if(info.collider->GetAttribute() == COLLISION_ATTR_WEAPONS){
		IsDigSound = true;
		
		IsAlive = false;
		IsDig = true;
		DigParticlePos = GetPosition();
		world.translation = {0,0,0};
		world.UpdateMatrix();
		collider->Update();
	}
}

void Walls::DigParticlePop()
{
	IsDigApp = true;
	for (int i = 0; i < 3; i++) {
		const float rnd_vel = 0.08f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = 0.06f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		Vector3 acc{};
		acc.y = -0.005f;

		DigParticle->ParticleSet(DigAppearanceFrame,DigParticlePos,vel,acc,0.4f,0.0f,1,{0.5f,0.3f,0.2f,1.f});
		DigParticle->ParticleAppearance();
	}
}
