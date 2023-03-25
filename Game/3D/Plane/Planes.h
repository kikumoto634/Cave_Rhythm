#pragma once
#include "../BaseObjObject.h"
#include "../../Collision/MeshCollider.h"
#include "../../Collision/CollisionSystem/CollisionAttribute.h"

class Planes : public BaseObjObject
{
private:
	//�`��͈�
	const int DrawingRange = 15;

public:
	~Planes();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns></returns>
	void Initialize(ObjModelManager* model);

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

	/// <summary>
	/// �F�ω�
	/// </summary>
	void PlaneColorChange(bool IsSwitch,  bool IsColorChange);
	inline void PlaneColorReset()	{object->SetColor({1.f,1.f,1.f,1.f});}

	inline void IsNotAlive()	{IsAlive = false;}

	//Getter
	inline bool GetIsPlayerContact()	{return IsPlayerContact;}

	//Setter
	inline void SetIsPlayerContact(bool IsFlag)	{IsPlayerContact = IsFlag;}
	inline void SetPlayerPos(Vector3 pos)	{PlayerPos = pos;}

private:
	bool IsPlayerContact = false;

	Vector3 PlayerPos;
	float distance = 0;

	//�G���A�Ŏg�p���Ȃ�
	bool IsAlive = true;
	//�����ɉ�������\��
	bool IsHide = false;

	bool IsCollision = false;
};

