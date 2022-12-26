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
	void Update(Camera* camera);

	/// <summary>
	/// 3D�`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �㏈��
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	void OnCollision(const CollisionInfo& info) override;


	//Getter
	inline bool GetIsPlayerContact()	{return IsPlayerContact;}

	//Setter
	inline void SetIsScaleChange(bool IsFlag)	{IsScaleChange = IsFlag;}
	inline void SetIsPlayerContact(bool IsFlag)	{IsPlayerContact = IsFlag;}

private:
	float ScaleMax = 1;
	float ScaleMin = 0.7f;
	float scaleTime = 0.f;

	Vector3 scale = {ScaleMax, ScaleMax, ScaleMax};

	bool IsScaleChange = false;
	bool IsPlayerContact = false;
};
