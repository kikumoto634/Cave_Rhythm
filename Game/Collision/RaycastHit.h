#pragma once
#include "CollisionSystem/BaseCollider.h"
#include "../3D/BaseObjObject.h"

#include <DirectXMath.h>


class BaseObjObject;

/// <summary>
/// ���C�L���X�g�ɂ����𓾂�ׂ̍\����
/// </summary>
struct RaycastHit
{
	//�Փˑ���̃I�u�W�F�N�g
	BaseObjObject* object = nullptr;
	//�Փˑ���̃R���C�_�[
	BaseCollider* collider = nullptr;
	//�Փ˓_
	DirectX::XMVECTOR inter;
	//�Փ˓_�܂ł̋���
	float distance = 0.0f;
};

