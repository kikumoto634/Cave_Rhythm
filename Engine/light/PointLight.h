#pragma once
#include <DirectXMath.h>

#include "../math/Vector/Vector2.h"
#include "../math/Vector/Vector3.h"
#include "../math/Vector/Vector4.h"

/// <summary>
/// ������
/// </summary>
class PointLight
{
//�T�u�N���X
public:
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		Vector3 lightpos;
		float pad1;
		Vector3 lightcolor;
		float pad2;
		Vector3 lightatten;
		unsigned int active;
	};

//�����o�֐�
public:

	//Getter
	inline const Vector3& GetLightPos()	{return lightpos;}
	inline const Vector3& GetLightColor()	{return lightcolor;}
	inline const Vector3& GetLightAten()	{return lightAtten;}
	inline bool GetIsActive()	{return IsActive;}

	//Setter
	inline void SetLightPos(const Vector3& lightpos)	{this->lightpos = lightpos;}
	inline void SetLightColor(const Vector3& lightcolor)	{this->lightcolor = lightcolor;}
	inline void SetLightAtten(const Vector3& lightAtten)	{this->lightAtten = lightAtten;}
	inline void SetIsActive(bool IsActive)	{this->IsActive = IsActive;}

//�����o�ϐ�
private:
	//���C�g���W
	Vector3 lightpos = {0,0,0};
	//���C�g�F
	Vector3 lightcolor = {1,1,1};
	//���C�g���������W��
	Vector3 lightAtten = {1.0f, 1.0f, 1.0f};
	//�L���t���O
	bool IsActive = false;
};

