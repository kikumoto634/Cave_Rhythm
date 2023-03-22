#pragma once

#include "CollisionSystem/BaseCollider.h"
#include "CollisionSystem/CollisionPrimitive.h"

#include <DirectXMath.h>

/// <summary>
/// ���b�V���Փ˔���I�u�W�F�N�g
/// </summary>
class MeshCollider : public BaseCollider
{
public:

	MeshCollider(){
		//���b�V���`����Z�b�g
		shapeType = COLLISIONSHAPE_MESH;
	}

	/// <summary>
	/// �O�p�`�̔z����\�z����
	/// </summary>
	/// <param name="model"></param>
	void ConstructTriangles(ObjModelManager* model);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// ���Ƃ̓����蔻��
	/// </summary>
	/// <param name="sphere">��</param>
	/// <param name="inter">��_(�o�͗p)</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	bool CheckCollisionSphere(const Sphere& sphere, DirectX::XMVECTOR* inter = nullptr, DirectX::XMVECTOR* reject = nullptr);

	/// <summary>
	/// ���C�Ƃ̓����蔻��
	/// </summary>
	/// <param name="ray">���C</param>
	/// <param name="distance">����(�o�͗p)</param>
	/// <param name="inter">��_(�o�͗p)</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	bool CheckCollisionRay(const Ray& ray, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);

private:
	std::vector<Triangle> triangle = {};
	//���[���h�s��̋t�s��
	DirectX::XMMATRIX invMatWorld = {};
};

