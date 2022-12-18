#pragma once

#include <DirectXMath.h>

class BaseObjObject;
class BaseCollider;

/// <summary>
///	�N�G���[�������𓾂�ׂ̍\����
/// </summary>
struct QueryHit
{
	//�Փˑ���̃I�u�W�F�N�g
	BaseObjObject* object = nullptr;
	//�Փˑ���̃R���C�_�[
	BaseCollider* collider = nullptr;
	//�Փ˓_
	DirectX::XMVECTOR inter;
	//�r�˃x�N�g��
	DirectX::XMVECTOR reject;
};


/// <summary>
/// �N�G���\�Ō��������o�������̓����z�肷��N���X
/// </summary>
class QueryCallback
{
public:
	QueryCallback() = default;
	virtual ~QueryCallback() = default;

	/// <summary>
	/// �������R�[���o�b�N
	/// </summary>
	/// <param name="info">�������</param>
	/// <returns>�N�G���\�𑱂���Ȃ�true, �ł��؂�Ȃ�false��Ԃ�</returns>
	virtual bool OnQueryHit(const QueryHit& info) = 0;
};