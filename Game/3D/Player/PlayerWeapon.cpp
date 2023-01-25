#include "PlayerWeapon.h"
#include "../../Collision/CollisionSystem/CollisionManager.h"
#include "../../Collision/CollisionSystem/CollisionAttribute.h"

using namespace DirectX;

PlayerWeapon::~PlayerWeapon()
{
	//Finalize();
}

void PlayerWeapon::Initialize(std::string filePath, bool IsSmoothing)
{
	BaseObjObject::Initialize(filePath, IsSmoothing);

	object->SetColor({1.0f,0.0f,0.0f,1.0f});

	//�R���C�_�[�̒ǉ�
	float radius = 0.6f;
	//���a�������������畂�������W�����̒��S�ɂ���
	SetCollider(new SphereCollider(XMVECTOR{0,radius,0,0}, radius));

	//�����蔻�葮��
	collider->SetAttribute(COLLISION_ATTR_WEAPONS);

	//���R���C�_�[�擾
	sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);
}

void PlayerWeapon::Update(Camera *camera)
{
	if(!IsAppear) return;
	this->camera = camera;
	
	//�����t���[��
	if(aliveCurrentFrame >= AliveFrame) {
		aliveCurrentFrame = 0;

		world.translation = Vector3(0,-10,0);
		BaseObjObject::Update(this->camera);

		IsAppear = false;
		return;
	}
	aliveCurrentFrame++;

	//�R���C�_�[�X�V
	collider->Update();

	//�x�[�X�X�V
	BaseObjObject::Update(this->camera);
}

void PlayerWeapon::Draw()
{
#ifdef _DEBUG
	if(!IsAppear) return;

	BaseObjObject::Draw();
#endif // _DEBUG
}

void PlayerWeapon::Finalize()
{
	BaseObjObject::Finalize();
}

void PlayerWeapon::OnCollision(const CollisionInfo &info)
{
	if(info.collider->GetAttribute() == COLLISION_ATTR_ENEMYS){
		camera->ShakeStart();
	}
}

void PlayerWeapon::Attack()
{
	IsAppear = true;
	world.translation = Vector3(0,1,2.5);
}
