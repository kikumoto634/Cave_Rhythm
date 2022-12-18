#include "CollisionManager.h"
#include "BaseCollider.h"
#include "Collision.h"
#include "../MeshCollider.h"

using namespace DirectX;

CollisionManager *CollisionManager::GetInstance()
{
	static CollisionManager instance;
	return &instance;
}

bool CollisionManager::Raycast(const Ray &ray, unsigned short attribute, RaycastHit *hitinfo, float maxDistance)
{
	bool result = false;
	//�����p�̃C�e���[�^
	std::forward_list<BaseCollider*>::iterator it;
	//���܂ōł��߂��R���C�_�[���L�^����ׂ̃C�e���[�^
	std::forward_list<BaseCollider*>::iterator it_hit;
	//���܂łōł��߂��R���C�_�[�̋������L�^����ϐ�
	float distance = maxDistance;
	//���܂łōł��߂��R���C�_�[�Ƃ̌�_���L�^����ϐ�
	XMVECTOR inter;

	//���ׂăR���C�_�[�Ƒ�������`�F�b�N
	it = colliders.begin();
	for(; it != colliders.end(); ++it){
		BaseCollider* colA = *it;

		//����������Ȃ���΃X�L�b�v
		if (!(colA->attribute & attribute)) {
			continue;
		}

		//���R���C�_�[�̏ꍇ
		if(colA->GetShapeType() == COLLISIONSHAPE_SPHERE){
			Sphere* sphere = dynamic_cast<Sphere*>(colA);
			float tempDistance;
			XMVECTOR tempinter;
			//������Ȃ���Ώ��O
			if(!Collision::CheckRay2Sphere(ray, *sphere, &tempDistance, &tempinter)) continue;
			//�������ŏ��łȂ���Ώ��O
			if(tempDistance >= distance) continue;
			//���܂łōł��߂��̂ŋL�^���Ƃ�
			result = true;
			distance = tempDistance;
			inter = tempinter;
			it_hit = it;
		}
		else if(colA->GetShapeType() == COLLISIONSHAPE_MESH){
			MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);

			float tempDistance;
			DirectX::XMVECTOR tempInter;
			if(!meshCollider->CheckCollisionRay(ray, &tempDistance, &tempInter)) continue;
			if(tempDistance >= distance) continue;

			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = it;
		}
	}

	//�ŏI�I�ɂȂɂ��ɓ������Ă����猋�ʂ���������
	if(result && hitinfo){
		hitinfo->distance = distance;
		hitinfo->inter = inter;
		hitinfo->collider = *it_hit;
		hitinfo->object = hitinfo->collider->GetObjObject();
	}

	return result;
}

bool CollisionManager::Raycast(const Ray &ray, RaycastHit *hitinfo, float maxDistance)
{
	//�S�����L���ɂ��đ����ł����s
	return Raycast(ray, 0xffff, hitinfo, maxDistance);
}

void CollisionManager::CheckAllCollisions()
{
	std::forward_list<BaseCollider*>::iterator itA;
	std::forward_list<BaseCollider*>::iterator itB;

	//���ׂĂ̑g�����ɂ��đ�������`�F�b�N
	itA = colliders.begin();
	for(; itA != colliders.end(); ++itA){
		itB = itA;
		++itB;

		for(; itB != colliders.end(); ++itB){
			BaseCollider* colA = *itA;
			BaseCollider* colB = *itB;

			//�Ƃ��ɋ�
			if(colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
				colB->GetShapeType() == COLLISIONSHAPE_SPHERE){
				Sphere* SphereA = dynamic_cast<Sphere*>(colA);
				Sphere* SphereB = dynamic_cast<Sphere*>(colB);
				DirectX::XMVECTOR inter;
				if(Collision::CheckSphere2Sphere(*SphereA, *SphereB, &inter)){
					colA->OnCollision(CollisionInfo(colB->GetObjObject(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetObjObject(), colA, inter));
				}
			}
			else if(colA->GetShapeType() == COLLISIONSHAPE_MESH && 
				colB->GetShapeType() == COLLISIONSHAPE_SPHERE){
				MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
				Sphere* sphere = dynamic_cast<Sphere*>(colB);
				DirectX::XMVECTOR inter;
				if(meshCollider->CheckCollisionSphere(*sphere, &inter)){
					colA->OnCollision(CollisionInfo(colB->GetObjObject(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetObjObject(), colA, inter));
				}
			}
			else if(colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
				colB->GetShapeType() == COLLISIONSHAPE_MESH){
				MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
				Sphere* sphere = dynamic_cast<Sphere*>(colA);
				DirectX::XMVECTOR inter;
				if(meshCollider->CheckCollisionSphere(*sphere, &inter)){
					colA->OnCollision(CollisionInfo(colB->GetObjObject(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetObjObject(), colA, inter));
				}
			}
		}
	}
}

void CollisionManager::QuerySphere(const Sphere &sphere, QueryCallback *callback, unsigned short attribute)
{
	assert(callback);

	std::forward_list<BaseCollider*>::iterator it;

	//���ׂẴR���C�_�[�Ƒ�������`�F�b�N
	it = colliders.begin();
	for(; it != colliders.end(); ++it){
		BaseCollider* col = *it;

		//����������Ȃ���΃X�L�b�v
		if(!(col->attribute & attribute)){
			continue;
		}

		//��
		if(col->GetShapeType() == COLLISIONSHAPE_SPHERE){
			Sphere* sphereB = dynamic_cast<Sphere*>(col);

			XMVECTOR tempInter;
			XMVECTOR tempReject;
			if(!Collision::CheckSphere2Sphere(sphere, *sphereB, &tempInter, &tempReject)) continue;

			//���������Z�b�g
			QueryHit info;
			info.collider = col;
			info.object = col->GetObjObject();
			info.inter = tempInter;
			info.reject = tempReject;

			//�N�G���\�R�[���o�b�N�Ăяo��
			if(!callback->OnQueryHit(info)){
				//�߂�l��false�̏ꍇ�A�p�������I��
				return ;
			}
		}
		//���b�V��
		else if(col->GetShapeType() == COLLISIONSHAPE_MESH){
			MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(col);

			XMVECTOR tempInter;
			XMVECTOR tempReject;
			if(!meshCollider->CheckCollisionSphere(sphere, &tempInter, &tempReject)) continue;

			//���������Z�b�g
			QueryHit info;
			info.collider = col;
			info.object = col->GetObjObject();
			info.inter = tempInter;
			info.reject = tempReject;

			//�N�G���[�R�[���o�b�N�Ăяo��
			if(!callback->OnQueryHit(info)){
				//�߂�l��false�̏ꍇ�A�p�������I��
				return ;
			}
		}
	}
}
