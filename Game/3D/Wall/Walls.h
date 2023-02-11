#pragma once
#include "../BaseObjObject.h"
#include "../../Collision/MeshCollider.h"
#include "../../Collision/CollisionSystem/CollisionAttribute.h"

class Walls : public BaseObjObject
{
public:
	~Walls();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns></returns>
	void Initialize(std::string filePath, bool IsSmmothing = false);

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

	void IsNotAlive()	{IsAlive = false;}

	//Getter
	inline bool GetIsAlive()	{return IsAlive;}
	inline bool GetIsDigSound()	{return IsDigSound;}

	//Setter
	inline void SetPlayerPos(Vector3 pos)	{PlayerPos = pos;}

private:
	Vector3 PlayerPos;
	float distance = 0;

	bool IsDigSound = false;

	//�G���A�Ŏg�p���Ȃ�
	bool IsAlive = true;
	//�����ɉ�������\��
	bool IsHide = false;

	//���b�V���R���C�_�[�p���f��
	ObjModelManager* colliderModel = nullptr;
};

