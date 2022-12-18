#pragma once

#include <DirectXMath.h>

class BaseObjObject;
class BaseCollider;

/// <summary>
/// �Փˏ��
/// </summary>
struct CollisionInfo
{
public:
	CollisionInfo(BaseObjObject* objObject, BaseCollider* collider, const DirectX::XMVECTOR& inter){
		this->objObject = objObject;
		this->collider = collider;
		this->inter = inter; 
	}

public:
	//�Փˑ���̃I�u�W�F�N�g
	BaseObjObject* objObject = nullptr;
	//�Փˑ���̃R���C�_�[
	BaseCollider* collider = nullptr;
	//�Փ˓_
	DirectX::XMVECTOR inter;
};