#include "TouchableObject.h"
#include "../../Game/Collision/MeshCollider.h"
#include "../../Game/Collision/CollisionSystem/CollisionAttribute.h"

void TouchableObject::Initialize(std::string filePath, bool IsSmmothing)
{
	BaseObjObject::Initialize(filePath, IsSmmothing);

	SetModel(model);

	//�R���C�_�[�̒ǉ�
	MeshCollider* collider = new MeshCollider;
	SetCollider(collider);
	//�����Z�b�g(�����I�n�`)
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	collider->ConstructTriangles(model);
}
