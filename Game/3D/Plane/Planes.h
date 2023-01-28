#pragma once
#include "../BaseObjObject.h"
#include "../../Collision/MeshCollider.h"
#include "../../Collision/CollisionSystem/CollisionAttribute.h"

class Planes : public BaseObjObject
{
public:
	~Planes();

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

	/// <summary>
	/// �F�ω�
	/// </summary>
	void PlaneColorChange(bool IsSwitch,  bool IsColorChange);
	void PlaneColorReset()	{object->SetColor({1.f,1.f,1.f,1.f});}


	//Getter
	inline bool GetIsPlayerContact()	{return IsPlayerContact;}

	//Setter
	inline void SetIsPlayerContact(bool IsFlag)	{IsPlayerContact = IsFlag;}

private:
	bool IsPlayerContact = false;
};

