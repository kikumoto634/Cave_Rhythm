#pragma once
#include "../BaseObjObject.h"
#include "../ParticleObject.h"
#include "../../Collision/MeshCollider.h"
#include "../../Collision/CollisionSystem/CollisionAttribute.h"

class Walls : public BaseObjObject
{
private:
	//�`��͈�
	const int DrawingRange = 11;
	//�`��͈�(�Â߂̕\��)
	const int DrawingRange_Half = 7;

public:
	~Walls();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns></returns>
	void Initialize(ObjModelManager* model, ObjModelManager* collider);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(Camera* camera) override;

	/// <summary>
	/// 3D�`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �㏈��
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	void OnCollision(const CollisionInfo& info) override;

	inline void IsNotAlive()	{IsAlive = false;}

	inline void CaveLightOn()	{IsCaveLight = true;}
	inline void CaveLightOff()	{IsCaveLight = false;}

	//Getter
	inline bool GetIsAlive()	{return IsAlive;}
	inline bool GetIsDIg()	{return IsDig;}
	inline Vector3 GetDigPosition()	{return digPosition;}

	//Setter
	void SetPlayerPos(Vector3 pos)	{PlayerPos = pos;}

private:
	Vector3 PlayerPos;
	float distance = 0;

	bool IsDig = false;
	Vector3 digPosition = {};

	//�G���A�Ŏg�p���Ȃ�
	bool IsAlive = true;
	//�����ɉ�������\��
	bool IsHide = false;

	//���b�V���R���C�_�[�p���f��
	ObjModelManager* colliderModel = nullptr;
	bool IsCollision = false;

	//���C�e�B���O
	bool IsCaveLight = false;
};

